; multi-segment executable file template.

data segment
    ; add your data here!
    res db 5 dup(0)
    k dw 10
ends

code segment
start:
; set segment registers:
    mov ax, data
    mov ds, ax
    mov es, ax

main PROC FAR  
    mov cx,8    ;此处循环系数正好也是8，通过此循环，求得8！+7！+6！+...+1
loop1:    
    mov ax,cx   ;
    call calcu
    add word ptr res,ax   ;将ax中的结果保存在res
    adc (res+2),0         ;如果进位则存储在这里，实际上并没有
    loop loop1
    
    mov ax,word ptr res
    call convert          ;转换数字形式
    
    dec di                 
    call prin             ;倒序输出res中的内容
        
    mov ah, 1
    int 21h
    
    mov ax, 4c00h ; exit to operating system.
    int 21h    
main endp


;倒序输出res中的内容
prin proc near
     
loop4:
    mov dl,res[di]
    mov ah,02h               ;配合指针的移动，和02号中断，逐个输出
    int 21h
    cmp di,0
    jz exit3
    dec di                   ;指针从后往前
    jmp loop4
exit3:
    ret
prin endp 

;转换，将ax中的数字转换为十进制形式，以字符形式倒序存储在首地址为res的数组里
convert proc near
    mov di,0 
loop2:
    cmp ax,0
    jz exit2
    div k       
    add dl,48           ;转换为字符形式
    mov res[di],dl      ;存储在相应位置
    inc di              ;指针加一，存在下一个位置
    mov dl,0
    jmp loop2 
exit2:  
    ret
convert endp 

;累乘循环 该模块负责的是将ax中的内容求阶乘并保留在ax中
calcu proc near
    mov bx,ax
loop3:            
    dec bx       ;bx初始值为[ax]-1,之后每次减一
    cmp bx,0             
    jz  exit
    mul bx
    jmp loop3
exit: 
    ret
calcu endp
    
ends

end start ; set entry point and stop the assembler.
