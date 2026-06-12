 ; multi-segment executable file template.

data segment
    ; add your data here!
    k dw 16
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

    mov ax,1234H   ;预先设置ax中的内容
    cmp ax,0
    jz zero         ;如果ax是零的话直接预先处理
    mov cx,0        ;cx 用来记录转换后的十进制的位数，辅助输出
    lea di,mess1
;转换十六进制为十进制，并以字符形式存储在mess1中    
loop1:
    cmp ax,0
    jz prin         ;脱离循环的为一条件
    div k 
    cmp dl,10       ;判断是字母还是数字的输出，采取不同的字符转换方是
    js num
    jmp char          ;
num:
    add dl,48       ;dl中存储的是余数，将数字形式转换为字符形式存储到首地址为mees1的字符串中
    mov es:[di], dl
    jmp later
char:
    add dl,55
    mov es:[di], dl
later:
    mov dl,0
    inc di          ;字符串指针移向下一个空位
    inc cx          ;代表十进制位数多1
    jmp loop1
;倒序逐个输出mess1中的内容   
prin:
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
   mov dl,'H'
   mov ah,02h
   int 21h  
ends

end start ; set entry point and stop the assembler.
