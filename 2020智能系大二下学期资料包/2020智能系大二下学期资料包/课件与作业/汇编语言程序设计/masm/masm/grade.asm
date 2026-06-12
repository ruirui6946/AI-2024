datarea segment                 ;define data segment
  grade         dw         88,75,95,63,98,78,87,73,90,60
  rank          dw         10 dup(?)
 
  ten           dw         10
datarea ends
;***************************************************************************
prognam segment                 ;define code segment
;---------------------------------------------------------------------------
main    proc    far             ;main part of segment
        assume cs:prognam,ds:datarea
start:                          ;starting excution address
;set up stack for return
        push ds                 ;save old data segment
        sub  ax,ax              ;put zero into AX
        push ax                 ;save it on stack

;set DS register to current data segment
        mov      ax,datarea
        mov      ds,ax           ;into DS register

;MAIN PART OF PROGRAM GOES HERE
        mov     di,10
        mov     bx,0

loop1:
        mov     ax,grade[bx]
        mov     dx,0
        mov     cx,10
        lea     si,grade
next:
        cmp     ax,[si]
        jg      no_count
        inc     dx
no_count:
        add     si,2
        loop    next
        mov     sp,bx
        mov     bx,dx
        add     bx,bx
        sub     bx,2
        mov     rank[bx],ax
        mov     bx,sp
        add     bx,2
        dec     di
        jne     loop1
        lea     si,rank
        mov     cx,10
        mov     bx,0
print:
        mov     ax,rank[bx]
        cwd
        div     ten
        add     ax,'0'
        add     dx,'0'
        mov     mess,al
        mov     mess+1,dl
        lea     dx,mess
        mov     ah,09
        int     21h
        add     bx,2
        loop    print
        ret                     ;return to DOS

main    endp                    ;end of main part of prognam
;----------------------------------------------------------------------------
prognam ends                    ;end of code segment
;****************************************************************************
        end     start           ;end assembly

