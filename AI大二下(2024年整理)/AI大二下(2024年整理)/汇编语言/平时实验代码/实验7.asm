dataseg segment
    three db 3
    mess db  'month:',13,10,'$'
    monin label byte
        max db 3
        act db ?
        mon db 3 dup(?)
    alfmon db 3 dup(0),13,10,'$'
    montab db 'JAN','FEB','MAR','APR','MAY','JUN','JUL','AUG','SEP','OCT','NOV','DEC'
dataseg ends

codeseg segment
    assume cs:codeseg, ds:dataseg,es:dataseg
start: 
    push ds
    sub ax,ax
    push ax
    mov ax , dataseg
    mov ds , ax
    mov es , ax


    lea dx,mess
    mov ah,09
    int 21h
    lea dx,monin
    mov ah,0ah
    int 21h
    mov dl,13
    mov ah,02
    int 21h
    mov dl, 10
    mov ah,02
    int 21h
    cmp act,0
    je exit

    mov ah,30h

    cmp act,2
    je two
    mov al,mon
    jmp conv

two:
    mov al,mon+1
    mov ah,mon
conv:
    xor ax,3030h
    cmp ah,0 
    jz loc 
    sub ah,ah
    add al,10

loc: 
    lea si,montab
    dec al
    mul three
    add si,ax
    mov cx,3
    cld
    lea di,alfmon
    rep movsb
    
    lea dx,alfmon
    mov ah,9
    int 21h 
    jmp start

exit: mov ah,4ch
      int 21h 

codeseg ends
    end start 
