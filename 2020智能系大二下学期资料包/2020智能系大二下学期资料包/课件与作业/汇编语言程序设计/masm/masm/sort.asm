datarea segment
  grade dw 88,75,95,63,98,78,87,73,90,60
  rank dw 10 dup(?)
  result dw 10 dup(?)
  temp db ?
;*************
prognam segment
main proc far
    assume cs:prognam,ds:datarea
start:
      push ds
      sub ax,ax
      push ax
;set DS register to current data segment
      mov ax,datarea
      mov ds,ax
;main part
      mov di,10
      mov bx,0
loop1:
      mov ax,grade[bx]
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
      add bx,2
      dec di
      jne loop1
      mov cx,10
      mov bx,0
      lea si,grade
      lea di,rank
loop2:
      mov bx,[di]
      dec bx
      add bx,bx
      mov dx,[si]
      mov result[bx],dx
      add di,2
      add si,2
      loop loop2
      mov cx,10
      mov bx,0
print:
      mov ax,result[bx]
      mov dl,10
      div dl
      add al,'0'
      mov dl,al
      mov temp,ah
      mov ah,02
      int 21h
      mov dl,temp
      add dl , 30h
      mov ah,02
      int 21h
      mov dl,','
      mov ah,02
      int 21h
      inc bx
      inc bx
      loop print
      ret
main endp
prognam ends
end start
