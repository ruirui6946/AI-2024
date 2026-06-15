data segment
data ends

code segment
assume  cs:code,ds:data
start:
    mov ax,data
    mov ds,ax

begin:
    mov ah,7
    int 21H
    cmp al,13
    jz over
    cmp al,122
    ja qita
    cmp al,97
    jae daxie
    cmp al,90
    ja qita 
    cmp al,65
    jnb xiaoxie 
    jmp qita

daxie:
    mov dl,'C'
    mov ah,2
    int 21h
    jmp begin
xiaoxie:
    mov dl,'c'
    mov ah,2
    int 21h
    jmp begin
qita:
    mov dl,'R'
    mov ah,2
    int 21h
    jmp begin
over:
    mov ah,4ch
    int 21h
code ends
end start