; multi-segment executable file template.

data segment
    ; add your data here!
    str 4 dup(0),10,13,'$'
ends
 
code segment
start:
; set segment registers:
    mov ax, data
    mov ds, ax
    mov es, ax
    
main    proc    far
    mov dx,0A123H         
    lea bx,str            ; bx指针指向str，之后通过bx在str中存储字符
    call trans           
    
    lea dx,str            ;输出str中的内容，也就是dx中的内容的ascii形式
    mov ah, 9
    int 21h
    
    mov ah,1
    int 21h
    
    mov ax,4c00h
    int 21h
main    endp    

trans proc near          ;trans程序的作用在于，将dx中的内容以字符形式存储在str中
    mov cx,4
trans1:
    rol dx,4             ;每次循环左移4位，将想要输出的高位保存在最低四位
    mov al,dl
    call htoasc           
    mov [bx],al          ;使用bx指针，在str中存储
    inc bx
    loop trans1          ;bx中一共有4*4个二进制位
    ret
trans endp  

htoasc proc near         ;htoasc的作用在于将al的最低四位的内容转换为ascii形式
    and al,0fh
    add al,30H
    cmp al,39H           ;字母和数字的处理不同
    jbe exit
    add al, 7
exit:
    ret
htoasc endp
ends

end start ; set entry point and stop the assembler.
