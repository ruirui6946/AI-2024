; multi-segment executable file template.

data segment
    ; add your data here!
    mess1 db "ABcdEfGabFZ",'$'   
    len1 equ $-mess1
    string2 db len1 dup(0)         
ends

code segment
start:
    mov ax, data
    mov ds, ax
    mov es, ax
    
    mov cx,len1-1   ;除去结束符
    lea si,mess1
    lea di,string2    ;预处理
    mov ax,32       ;用此作以筛选 
    
loop1:
    or [si],al;     ;or处理
    inc si;         ;手动加一
    loop loop1      ;循环处理
    
    mov cx,len1     
    lea si,mess1    ;预处理
    rep movsb       ;复制字符串
    
    lea dx,string2
    mov ah,9
    int 21h          ;输出mess2内容
        
ends

end start ; set entry point and stop the assembler.
