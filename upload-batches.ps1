param(
    [string]$Remote = "origin",
    [string]$Branch = "main",
    [int]$BatchSizeMB = 250,
    [int]$MaxFilesPerBatch = 500,
    [int]$LfsThresholdMB = 95,
    [int]$DelaySeconds = 2,
    [int]$MaxBatches = 0,
    [switch]$PlanOnly
)

$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot
$repoRoot = (Get-Location).Path

function Invoke-Git {
    param([string[]]$GitArgs)

    & git @GitArgs
    if ($LASTEXITCODE -ne 0) {
        throw "git $($GitArgs -join ' ') failed with exit code $LASTEXITCODE"
    }
}

function Get-UntrackedFiles {
    $paths = @(& git -c core.quotepath=false ls-files --others --exclude-standard)
    if ($LASTEXITCODE -ne 0) {
        throw "Unable to list untracked files."
    }

    $files = foreach ($relativePath in $paths) {
        if ([string]::IsNullOrWhiteSpace($relativePath)) {
            continue
        }

        $fullPath = Join-Path $repoRoot $relativePath
        try {
            $item = Get-Item -LiteralPath $fullPath -Force -ErrorAction Stop
        }
        catch {
            Write-Warning "Skipping inaccessible file: $relativePath"
            continue
        }

        if (-not $item.PSIsContainer) {
            [pscustomobject]@{
                RelativePath = $relativePath.Replace("\", "/")
                Length       = [long]$item.Length
            }
        }
    }

    return @($files | Sort-Object RelativePath)
}

function Write-NulPathspecFile {
    param(
        [string]$Path,
        [object[]]$Files
    )

    $content = [string]::Join([char]0, @($Files | ForEach-Object { $_.RelativePath })) + [char]0
    [System.IO.File]::WriteAllText($Path, $content, [System.Text.UTF8Encoding]::new($false))
}

function Push-CurrentCommit {
    Invoke-Git -GitArgs @("push", $Remote, "HEAD:$Branch")
}

Invoke-Git -GitArgs @("config", "core.longpaths", "true")
Invoke-Git -GitArgs @("lfs", "install", "--local")

if ($PlanOnly) {
    $files = Get-UntrackedFiles
    $totalBytes = ($files | Measure-Object Length -Sum).Sum
    $largeFiles = @($files | Where-Object { $_.Length -gt ($LfsThresholdMB * 1MB) })

    Write-Host "Untracked files : $($files.Count)"
    Write-Host ("Untracked size  : {0:N2} GiB" -f ($totalBytes / 1GB))
    Write-Host "LFS candidates  : $($largeFiles.Count)"
    Write-Host "Batch limits    : $BatchSizeMB MiB / $MaxFilesPerBatch files"
    exit 0
}

Invoke-Git -GitArgs @("fetch", "--filter=blob:none", "--no-tags", $Remote, $Branch)
& git merge-base --is-ancestor "$Remote/$Branch" HEAD
if ($LASTEXITCODE -ne 0) {
    throw "Remote branch advanced. Rebase or merge before continuing."
}

$trackedChanges = @(& git status --porcelain --untracked-files=no)
if ($LASTEXITCODE -ne 0) {
    throw "Unable to inspect tracked changes."
}
if ($trackedChanges.Count -gt 0) {
    throw "Tracked or staged changes already exist. Commit or restore them before running this uploader."
}

$files = Get-UntrackedFiles
$largeFiles = @($files | Where-Object { $_.Length -gt ($LfsThresholdMB * 1MB) })

foreach ($file in $largeFiles) {
    $attribute = & git check-attr filter -- $file.RelativePath
    if ($LASTEXITCODE -ne 0) {
        throw "Unable to inspect LFS attributes for $($file.RelativePath)"
    }
    if ($attribute -notmatch "filter: lfs$") {
        Invoke-Git -GitArgs @("lfs", "track", "--", $file.RelativePath)
    }
}

& git diff --quiet -- .gitattributes
if ($LASTEXITCODE -ne 0) {
    Invoke-Git -GitArgs @("add", ".gitattributes")
    Invoke-Git -GitArgs @("commit", "-m", "Configure Git LFS for remaining large files")
    Push-CurrentCommit
}

$batchNumber = 0
$batchLimitBytes = [long]$BatchSizeMB * 1MB

while ($true) {
    $files = Get-UntrackedFiles
    if ($files.Count -eq 0) {
        Write-Host "All currently untracked files have been uploaded."
        break
    }

    if ($MaxBatches -gt 0 -and $batchNumber -ge $MaxBatches) {
        Write-Host "Stopped after $MaxBatches batches as requested."
        break
    }

    $batch = [System.Collections.Generic.List[object]]::new()
    [long]$batchBytes = 0

    foreach ($file in $files) {
        $wouldExceedBytes = $batch.Count -gt 0 -and ($batchBytes + $file.Length) -gt $batchLimitBytes
        $wouldExceedCount = $batch.Count -ge $MaxFilesPerBatch
        if ($wouldExceedBytes -or $wouldExceedCount) {
            break
        }

        $batch.Add($file)
        $batchBytes += $file.Length
    }

    $batchNumber++
    $pathspecFile = Join-Path ([System.IO.Path]::GetTempPath()) "ai-2024-upload-paths-$PID.bin"

    try {
        Write-NulPathspecFile -Path $pathspecFile -Files $batch
        Invoke-Git -GitArgs @("--literal-pathspecs", "add", "--pathspec-from-file=$pathspecFile", "--pathspec-file-nul")
        Invoke-Git -GitArgs @(
            "commit",
            "-m",
            ("Upload batch {0:D3} ({1} files, {2:N1} MiB)" -f $batchNumber, $batch.Count, ($batchBytes / 1MB))
        )
        Push-CurrentCommit
    }
    finally {
        Remove-Item -LiteralPath $pathspecFile -Force -ErrorAction SilentlyContinue
    }

    Write-Host ("Uploaded batch {0}: {1} files, {2:N1} MiB" -f $batchNumber, $batch.Count, ($batchBytes / 1MB))
    if ($DelaySeconds -gt 0) {
        Start-Sleep -Seconds $DelaySeconds
    }
}

Invoke-Git -GitArgs @("status", "--short", "--branch")
