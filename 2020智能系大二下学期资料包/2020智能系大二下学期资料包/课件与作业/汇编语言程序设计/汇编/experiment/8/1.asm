; multi-segment executable file template.

data segment
    ; add your data here!
   k dw 10
   mess1 db 2 dup(0)  
   temp dw 0 
ends

stack segment
    dw   128  dup(0)
ends

code segment
start:
; set segment registers:
    mov ax, data
    mov ds, ax
    mov es, ax

main proc near
    mov ax,2 
loop1: 
    mov temp,ax
    cmp ax,101
    jz exit
    call is_prime   
    mov ax, temp
    inc ax;
    jmp loop1    
exit: 
     ; wait for any key....    
    mov ah, 1
    int 21h
    
    mov ax, 4c00h ; exit to operating system.
    int 21h 
main endp 
 

is_prime proc near  
    mov bx,2;
    mov dx,0
    mov cx,ax
loop2:
    mov dx,0 
    cmp bx,ax
    jz prin                                 
    idiv bx      
    cmp dx,0
    jz exit1
    mov ax,cx
    inc bx
    jmp loop2
prin:
    call print
exit1:
     ret            
is_prime endp

   
print proc near    
;转换十六进制为十进制，并以字符形式存储在mess1中
    mov cx, 0
    lea di,mess1
    mov dx, 0    
loop3:
    cmp ax,0
    jz cout         ;脱离循环的为一条件
    div k           ;
    add dl,48       ;dl中存储的是余数，将数字形式转换为字符形式存储到首地址为mees1的字符串中
    mov es:[di], dl
    mov dl,0
    inc di          ;字符串指针移向下一个空位
    inc cx          ;代表十进制位数多1
    jmp loop3
;倒序逐个输出mess1中的内容   
cout:
   dec di           ;此时di实际指向在有意义的字符的下一位，所以要减一
loop4:
   cmp cx,0         ;cx是之前创建的计数器
   jz exit2
   mov dl, es:[di]  ;逐位输出
   mov ah,02h
   int 21h
   dec cx           ;
   dec di
   jmp loop4 
exit2:   
    mov dl,' '
    mov ah,02h
    int 21h
    
    ret                             
print endp
   
ends

end start ; set entry point and stop the assembler.
