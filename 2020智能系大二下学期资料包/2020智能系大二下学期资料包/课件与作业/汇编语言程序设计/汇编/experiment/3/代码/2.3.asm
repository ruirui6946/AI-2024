; multi-segment executable file template.

data segment
    ; add your data here!
    mess1 db "ABcdEfGabFZ",'$'   
    len1 equ $-mess1
    string3 db len1 dup(0)      
ends

code segment
start:
    mov ax, data
    mov ds, ax
    mov es, ax
    
    mov cx,len1-1   ;除去结束符
    lea si,mess1
    lea di,string3   ;预处理
    mov al,223      ;用此作以小写字母变大写 
    mov ah,32;      ;用此作以大写字母变小写
loop1:  
    CMP [si],97     ;判断
    JS  s;
    and [si],al;    ;小写变大写
    jmp later; 
s:
    or [si],ah      ;大写变小写
later:
    inc si;         ;手动加一
    loop loop1      ;循环处理
    
    mov cx,len1     
    lea si,mess1    ;预处理
    rep movsb       ;复制字符串
    
    lea dx,string3
    mov ah,9
    int 21h          ;输出mess2内容
        
ends

end start ; set entry point and stop the assembler.
