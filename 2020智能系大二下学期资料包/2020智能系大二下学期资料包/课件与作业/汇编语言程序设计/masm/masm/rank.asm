datarea segment
  grade dw 88,75,95,63,98,78,87,73,90,60
  rank dw 10 dup(?)
datarea ends
prognam segment
main proc far
     assume cs:prognam,ds:datarea
start:
     push ds
     sub ax,ax
     push ax
     mov ax,datarea
     mov ds,ax
     mov di,10
     mov bx,0
loop:
     mov ax,grade[bx]
     mov cl,10
     div cl
     push bx
     mov bx , ax
     add bx , 3030h
     mov dl,bl
     mov ah,2
     int 21h
     mov dl,bh
     mov ah,2
     int 21h
     pop bx
        mov dl , 24h
        mov ah , 02h
        int 21h
     
     mov dx,0
     mov cx,10
     lea si,grade
next:
     cmp ax,[si]
     jg no_count
     inc dx
no_count:
     add si,2
     loop next
     mov rank[bx],dx
    
     
    ; mov ah,09
    ; int 21h
     add bx,2
     dec di
     jne loop

   ;  mov cx,10
   ;  mov bx,0
;again:
    ; mov ax,rank[bx]
    ; mov dx,grade[ax]
    ; mov ah,09
    ; int 21h
    ; add bx,2
    ; loop again
     ret
main endp
prognam ends
      end start
