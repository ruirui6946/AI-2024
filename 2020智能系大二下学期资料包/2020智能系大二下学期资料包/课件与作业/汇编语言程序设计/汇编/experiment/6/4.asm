; multi-segment executable file template.

data segment
    ; add your data here!
    k dw 16
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

    mov cx,4            ;由题总计输入4位十六进制数
    mov bx,0            ;bx作为临时存储位置
loop1:    
    mov ah,01           ;读取输入
    int 21h
    mov ah,0
    sub ax,48           ;输入的字符转为数字
    cmp ax,10
    js num              ;如果输入的是字母还需要进一步的处理
    sub ax,7 
num:   
    xchg ax, bx         ;bx存储的是往期的值，因此最终结果应是 bx = bx*16 +ax 
    mul k
    add bx,ax
    loop loop1
    
    mov dl,13           ;输出回车，换行
    mov ah,02
    int 21h
    mov dl,10
    mov ah,02
    int 21h
    
    mov cx,16
loop2:            
     
    shl bx,1            ;利用逻辑左移，从高位开始逐个判定
    jb one
    jmp zero            ;根据 cf的值来判定输出1还是0
one: 
    mov dl,49
    mov ah,02
    int 21h
    jmp continue
zero:
    mov dl,48
    mov ah,02
    int 21h
    jmp continue
continue:
    loop loop2          ;循环16次
exit:       
ends

end start ; set entry point and stop the assembler.
