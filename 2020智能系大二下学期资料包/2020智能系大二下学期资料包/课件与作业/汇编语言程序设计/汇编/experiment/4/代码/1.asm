; multi-segment executable file template.

data segment
    ; add your data here!
   s5 db '0',13,10,'$'
   s6 db '0',13,10,'$'
   s7 db '0',13,10,'$'
   s8 db '0',13,10,'$'
   s9 db '0',13,10,'$'
   data1 db 10 dup(62,92,87,75,77,98,83,66,30,89)   ;初始数据
ends

code segment
start:
; set segment registers:
    mov ax, data
    mov ds, ax
    mov es, ax

    lea si,data1    ;初始化
    mov cx,10       ;      

loop1:    
    cmp [si],90     ;大于等于90
    JBE nine
    cmp [si],80     ;大于等于80
    JBE EIGHT
    cmp [si],70     ;大于等于70
    JBE seven
    cmp [si],60     ;大于等于60
    JBE six
    inc s5          ;不及格
    jmp exit
six:
    inc s6 
    jmp exit
seven:
    inc s7
    jmp exit
eight:
    inc s8
    jmp exit
nine:
    inc s9
    jmp exit
exit:   
    inc si          ;si加1
    loop loop1      ;循环
   
   lea dx,s5        ;输出结果
   mov ah,9
   int 21h
   
    lea dx,s6
   mov ah,9
   int 21h
   
   lea dx,s7
   mov ah,9
   int 21h
   
   lea dx,s8
   mov ah,9
   int 21h
   
   lea dx,s9
   mov ah,9
   int 21h  
ends

end start ; set entry point and stop the assembler.
