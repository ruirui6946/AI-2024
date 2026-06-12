datasg segment
mess1 db 'enter keyword:','$'
mess2 db 'enter sentence:','$'
mess3 db 'match at location:',13,10,'$'
mess4 db 'no match',13,10,'$'
;mess5 db 'match',13,10,'$'
crlf db 0dh,0ah,'$'
stokins label byte
max1 db 20
act1 db ?
stokn1 db 20 dup(?)
;
stokink label byte
max2 db 4
act2 db ?
stokn2 db 4 dup(?)
;
datasg ends

prognam segment
;
main proc far
          assume ds:datasg,cs:prognam,es:datasg
start:
     push ds
     sub ax,ax
     push ax
     sub bx,bx
     mov ax,datasg
     mov ds,ax
     mov es,ax
;main
     lea dx,mess1
     mov ah,09
     int 21h
     lea dx,stokink
     mov ah,0ah
     int 21h
     cmp act2,0
     je exit ;is 0 tuichu
a10:
     lea dx,crlf;huiche,huanhang
     mov ah,09
     int 21h
     lea dx,mess2
     mov ah,09
     int 21h
     lea dx,stokins
     mov ah,0ah
     int 21h
     cmp act1,0
     je nmatch
     mov al,act2
     cbw
     mov cx,ax
     
     mov al,act1  ;keyword-sentence
     sub al,act2
     js nmatch
     push cx
     lea bx,stokins
     mov di,0
     mov si,0
     inc al
;
a20:
     mov ah,[bx+di];from first cmpare
     cmp ah,stokink[si]
     jne a21 ;!=,so jmp to cmppare next
     inc si
     inc di
     dec cx ;cx li is keyword lentgh
     cmp cx,0
     je match
     jmp a20
a21:
     inc bx
     dec al
     cmp al,0 ;compare finish ,nomatch
     je nmatch
     pop cx
     push cx
     mov di,0
     mov si,0
     jmp a20
exit:
     lea dx,crlf
     mov ah,09
     int 21h
     ret

match:
    ; lea dx,mess5
    ; mov ah,09
    ; int 21h
     lea dx,crlf
     mov ah,09
     int 21h
     lea dx,mess3
     mov ah,09
     int 21h
     sub bx,offset stokins
     inc bx
     call tra
     jmp a10
nmatch:
     lea dx,crlf
     mov ah,09
     int 21h
     lea dx,mess4
     mov ah,09
     int 21h
     jmp a10
tra proc near
     mov ch,4
rotate:
     mov cl,4
     rol bx,cl
     mov al,bl
     and al,0fh
     add al,30h
     cmp al,3ah
     jl  printf
     add al,07h
printf:
     mov dl,al
     mov ah,2
     int 21h
     dec ch
     jnz rotate
     ret
tra endp
main endp
prognam ends
end start

