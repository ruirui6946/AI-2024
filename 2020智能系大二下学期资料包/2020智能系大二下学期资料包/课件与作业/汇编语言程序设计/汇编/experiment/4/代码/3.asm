; multi-segment executable file template.

data segment
    ; add your data here!
    mess1 db "end",13,10,'$'
   ;存放输入内容
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
    
   mov ah,07h       ;利用dos系统，从键盘读取数据存储在al中
   int 21h                                                
   cmp al,0Dh       ;如果输入的是回车，那么直接退出程序
   jz exit
   cmp al,49        ;输入不是回车且小于49
   js R
   cmp al,58        ;输入在49-57，是数字字符
   js N
   cmp al,65        ;输入在58-64，是其他字符
   js R 
   cmp al,91        ;输入在65-90，是大写字母字符
   js C1
   cmp al,97        ;输入在91到96，是其他字符
   js R
   cmp al,123       ;输入在97到122，是小写字母字符 
   js c
   jmp R
   
c:
    mov dl,'c'
    mov ah,02h
    int 21h
    jmp exit
   
C1:
    mov dl,'C'
    mov ah,02h
    int 21h
    jmp exit 
N:
    mov dl,'N'
    mov ah,02h
    int 21h
    jmp exit   
R:
    mov dl,'R'
    mov ah,02h
    int 21h
    jmp exit
exit:
   lea dx,mess1
   mov ah,09h
   int 21h 
ends

end start ; set entry point and stop the assembler.
