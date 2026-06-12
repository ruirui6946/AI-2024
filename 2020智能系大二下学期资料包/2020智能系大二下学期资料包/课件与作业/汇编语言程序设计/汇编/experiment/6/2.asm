; multi-segment executable file template.

data segment
    ; add your data here!
    mess db 'Print a number less than 8',10,13,'$'
    mess2 db 13,10,'Result is: ','$'
    k dw 10 
    mess1 db 0      ;用来储存十进制数的倒序
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
    
    
    lea dx,mess  ;输出提示信息
    mov ah,09h
    int 21h 
    
    mov ax,0
    
    mov ah,01    ;从键盘读取输入
    int 21h
    
    cbw
    sub ax,48    ;将字符转换为数字
    mov cx,ax
    mov bx,ax
loop3:           ;累乘循环
    dec bx
    cmp bx,0
    jz output
    mul bx
    jmp loop3
    
output:          ;十六进制转换为10进制
    cmp ax,0
    jz zero         ;如果ax是零的话直接预先处理
    mov cx,0        ;cx 用来记录转换后的十进制的位数，辅助输出
    lea di,mess1
;转换十六进制为十进制，并以字符形式存储在mess1中    
loop1:
    cmp ax,0
    jz prin
         ;脱离循环的为一条件
    div k           ;
    add dl,48       ;dl中存储的是余数，将数字形式转换为字符形式存储到首地址为mees1的字符串中
    mov es:[di], dl
    mov dl,0
    inc di          ;字符串指针移向下一个空位
    inc cx          ;代表十进制位数多1
    jmp loop1
;倒序逐个输出mess1中的内容   
prin:
   lea dx,mess2
   mov ah,09h
   int 21h
   dec di           ;到达prin模块，必定是经历了loop1至少1轮，即ax!=0,所以此时di实际指向在有意义的字符的下一位，所以要减一
loop2:
   cmp cx,0         ;cx是之前创建的计数器
   jz exit
   mov dl, es:[di]  ;逐位输出
   mov ah,02h
   int 21h
   dec cx           ;
   dec di
   jmp loop2 
   
;ax=0 的情况单独处理   
zero:
    mov dl,'0'
    mov ah,02h
    int 21h
    jmp exit
exit:
    mov dl,'D'
    mov ah,02h
    int 21h 
ends

end start ; set entry point and stop the assembler.
