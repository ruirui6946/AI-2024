;PROGRAM TITLE GOES HERE--Rank
;****************************************************************************
datarea segment                 ;define data segment
grade   dw      88,75,95,63,98,78,87,73,90,60
rank    dw      10 dup (?)
arrange dw      20 dup (?) , '$'
datarea ends
;****************************************************************************
prognam segment                 ;define code segment
;----------------------------------------------------------------------------
main    proc    far             ;main part of program
        assume  cs:prognam , ds:datarea
start:                          ;main part of prognam
;set up stack for return
        push    ds              ;save old data segment
        sub     ax , ax         ;put zero in AX
        push    ax              ;save it in stack
;set DS register to current data segment
        mov     ax , datarea    ;datarea segment addr
        mov     ds , ax         ;       into DS register
;MAIN PART OF PROGRAM GOSE HERE
        mov     di , 10                                
        mov     bx , 0
loop1:
        mov     ax , grade[bx]
        mov     dx , 0
        mov     cx , 10
        lea     si , grade
next:
        cmp     ax , [si]
        jg      no_count
        inc     dx
no_count:
        add     si , 2
        loop    next
        mov     rank[bx] , dx
        dec     dx
        shl     dx , 1
        shl     dx , 1
        mov     si , dx
        mov     dl , 10
        div     dl
        add     al , 30h
        mov     BYTE PTR arrange[si] , al
        inc     si
        add     ah , 30h
        mov     BYTE PTR arrange[si] , ah
        inc     si
        mov     BYTE PTR arrange[si] , ','
        inc     si
        mov     BYTE PTR arrange[si] , ' '
        add     bx , 2
        dec     di
        jne     loop1
        lea     dx , arrange
        mov     ah , 09
        int     21h
        ret                     ;return to DOS
main    endp                    ;end of main part of program
;----------------------------------------------------------------------------
prognam ends                    ;end of code segment
;****************************************************************************
        end start               ;end assembly
