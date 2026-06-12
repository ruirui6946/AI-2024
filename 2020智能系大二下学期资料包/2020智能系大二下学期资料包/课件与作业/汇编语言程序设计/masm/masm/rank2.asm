datarea segment
  grade         dw      88,75,95,63,98,78,87,73,90,60
  rank          dw      10 dup(?)
datarea ends
;
prognam segment
;
main    proc    far
        assume cs:prognam,ds:datarea
start:
        push    ds
        sub     ax,ax
        push    ax

        mov     ax,datarea
        mov     ds,ax
;MAIN PART OF PROGRAM GOES HERE
        mov     di,10
        mov     bx,0
loop:
        mov     ax,grade[bx]
       
        mov     cx,10
        lea     si,grade
        lea     bp,grade
        add     si,2
next:
        cmp     ax,[si]
        jnb      no_count
        mov     ax,[si]
        mov     dx,[bp]
        mov     [si],dx
        mov     [bp],ax
        
 
no_count:
        add     si,2
        add     bp,2
        mov     ax,[bp]
        loop    next
       
        
        dec     di
        jne     loop
        ret

        mov     dx,3
        mov     ah,02
        int     21h
print:   mov     cx,0
        lea     si,grade
        lea     dx,[si]
        add     si,2
        mov     ah,02
        int     21h
        inc     cx
        cmp     cx,10
        jl      print
        ret

main    endp
prognam ends
        end     start
