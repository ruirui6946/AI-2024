data segment
    msg db 'time:',0ah,0dh,'$'
    sign db ':','$'
    tuige db 08h,'$'
data ends
stack segment 
    dw 256 dup(?)
stack ends
code segment 
assume ds:data,cs:code 
start:
    mov ax,data  
    mov ds,ax 
    mov ax,stack 
    mov ss,ax

    mov dx,offset msg 
    mov ah,9
    int 21h 

loop_0:
    mov ah,2ah
    int 21h 

    mov bx,cx ;输出日期
    call print
    push dx 
    mov dx,offset sign
    mov ah,9
    int 21h
    pop dx 
    mov bl,dh 
    call print_tiny 
    push dx 
    mov dx,offset sign
    mov ah,9
    int 21h
    pop dx 
    mov bl,dl  
    call print_tiny
    push dx 
    mov dx,offset sign
    mov ah,9
    int 21h
    pop dx 

    mov ah,2ch
    int 21h 
    
    ;输出时间
    mov bl,ch 
    call print_tiny 
    push dx 
    mov dx,offset sign
    mov ah,9
    int 21h
    pop dx 
    mov bl,cl 
    call print_tiny 
    push dx 
    mov dx,offset sign
    mov ah,9
    int 21h
    pop dx 
    mov bl,dh 
    call print_tiny 
    mov cx,22
loop_2:
    mov dx,offset tuige 
    mov ah,9
    int 21h
    loop loop_2
    jmp loop_0

    mov ah,4ch 
    int 21h

print proc near;bx传参
    push ax 
    push cx 
    push dx 
    mov cx,0
    mov ax,bx
    mov bx,10 
loop1:
    mov dx,0 
    div bx 
    push dx 
    inc cx 
    cmp ax,0
    jne loop1 

loop2:
    pop dx 
    xor dx,30h 
    mov ah,2 
    int 21H
    loop loop2
    pop dx 
    pop cx 
    pop ax     
    ret
print endp
print_tiny proc near;bl传参
    push ax 
    push cx 
    push dx 
    mov cx,0
    mov al,bl
    mov bl,10 
loop01:
    mov ah,0 
    div bl
    push ax 
    inc cx 
    cmp al,0
    jne loop01 

loop02:
    pop ax
    xor ah,30h
    mov dl,ah 
    mov ah,2 
    int 21H
    loop loop02
    pop dx 
    pop cx 
    pop ax     
    ret
print_tiny endp
code ends
end start