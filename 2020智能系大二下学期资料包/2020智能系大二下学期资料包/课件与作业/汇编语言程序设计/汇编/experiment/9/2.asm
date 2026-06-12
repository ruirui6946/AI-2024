; multi-segment executable file template.

data segment
    ; add your data here!
    k dw 10           ;一个系数，方便运算
    data1 dw 3 dup(0) ;存储各个数字的各个位数在转换为三次方的值
    mess1 db 3 dup(0) ;存储ax中的各个十进制字符表示的逆序    
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
    mov cx,100     ;cx中存储着每一次待判定的数字
mainloop1:
    cmp cx,1000
    jz mainexit
    call judge_daffodil ;负责判断是否是水仙花数
    inc cx
    jmp mainloop1
mainexit:
    ; wait for any key....    
    mov ah, 1
    int 21h
    
    mov ax, 4c00h ; exit to operating system.
    int 21h      
main endp
  
judge_daffodil proc near
    push ax
    push dx  
    push di
    mov dx,0
    mov ax,cx      ;将cx中的数据放到ax中
    mov di,0;
jloop1:
    cmp ax,0       ;将ax中的各个数位取出来，放到data1数组中    
    jz jcalcu
    idiv k
    mov data1[di],dx
    inc di
    inc di
    mov dx,0
    jmp jloop1
jcalcu:
    mov di,0
jloop2:
    cmp di,6       ;将ax中各个数位求立方，再次返还到data1数组中
    jz jadd
    mov ax,data1[di]
    imul word ptr data1[di]
    imul word ptr data1[di]
    mov  word ptr data1[di],ax
    inc di
    inc di
    jmp jloop2 
jadd:
    mov ax,0      ;将data1数组的三个数字相加
    mov di,0
jloop3:
    cmp di,6
    jz jcompare
    add ax,word ptr data1[di]
    inc di
    inc di
    jmp jloop3
jcompare:
    cmp ax,cx      ;判定是否为水仙花数
    jz jprint      ;是的话输出
    jmp jnext      ;否的话返回主程序 
jprint:
    call prin      ;调用输出，将ax中的数字以十进制形式输出出来
jnext: 
    pop di     
    pop dx
    pop ax
    ret
    
judge_daffodil endp
 
prin proc near
    push dx
    push di
    mov dx,0
    mov di,0
prinloop1:
    cmp ax,0       ;将ax中的各个数位取出来以字符形式放到mess1中
    jz pnext
    idiv k
    add dl,48      ;dl中是余数
    mov mess1[di],dl
    mov dl,0
    inc di
    jmp prinloop1
pnext:
  
prinloop2:          ;将mess1中的内容倒序输出
    cmp di,0 
    jz prexit
    dec di
    mov dl,mess1[di]
    mov ah,02
    int 21h
    jmp prinloop2
prexit:
    mov dl,13      ;输出回车换行，以相互区分
    mov ah,02
    int 21h
    mov dl,10
    mov ah,02
    int 21h 
    pop di
    pop dx
    ret 
prin endp   
ends

end start ; set entry point and stop the assembler.
