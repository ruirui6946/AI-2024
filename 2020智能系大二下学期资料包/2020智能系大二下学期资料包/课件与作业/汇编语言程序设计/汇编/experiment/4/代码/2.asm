; multi-segment executable file template.

data segment
    ; add your data here!
   a db 1
   b db -10
   c dw 21
   temp1 dw 0
   temp2 dw 0
   tag db 0
   k db 4
   sign db -1
ends

code segment
start:
; set segment registers:
    mov ax, data
    mov ds, ax
    mov es, ax
    
    cmp b,0 ;对比b的值是否为零
    jz bzero ;b的值为零则单独拿出来讨论
    cmp a,0  ;否则则讨论a的取值
    jz azero1
    js am1
    jmp ap1
    
azero1:         ;b不为0，a为0
    jmp great
    
am1:            ;b不为零，a为负
    cmp c,0
    jns great   ;异号检测
    mov ax,c
    imul sign
    mov c,ax
    mov al,a
    imul sign
    mov a,al
    jmp cal 
ap1:             ;b不为零，a为正
    cmp c,0
    js great     ;异号检测
    jmp cal
    
cal:           ;delta的计算
    mov ax,0
    mov al,b
    imul b
    mov temp1,ax
    
    mov ax,0
    mov dx,0
    mov al,a
    mul k
    mul c 
    
    cmp dx,0
    jnz exit  
    
    cmp ax,temp1
    jle great
    jmp exit
        

bzero:            ;b为零
    cmp a,0
    jz azero
    js am
    jmp ap   
    
am:              ;b为零 a为负
    cmp c,0
    jnl great
    jmp exit
    
ap:
    cmp c,0      ;b为零 a为正
    jl great
    jmp exit
    
azero:           ;b为零 a为零
    jmp exit
     
great:           ;有实根
    mov tag,1
                 ;无实根
exit:
 
     
      
    
ends

end start ; set entry point and stop the assembler.
