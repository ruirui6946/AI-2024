; multi-segment executable file template.

data segment
    ; add your data here!
    MEM db 50 dup(12,0,15,0,0,-100,6,7,5,10,0,5,8,34,0,24,0,11,7,8,9,32,17,0,1,5,6,0,4,0,5,0,8,0,9,0,2,0,7,0,64,0,3,0,7,4,0,5,0,8)
    
ends
 

code segment
start:
; set segment registers:
    mov ax, data
    mov ds, ax
    mov es, ax

    mov bx,0            ;通过比较，倘若MEM[bx]的值为零则和MEM[di]的值交换
    mov di,49
loop1:    
    cmp bx,di           ;比较两个指针，倘若两个会和，则bx左边都是非零，di右边都是0
    jz exit
    cmp MEM[bx],0
    jnz continue        
    mov al,MEM[bx]      ;交换 MEM[bx]   MEM[di]
    xchg al,MEM[di]
    mov MEM[bx],al
    dec di
    jmp later           ;注意，由于交换后的值不知道是不是0.因此此时bx指针不可以改变
continue:
    inc bx    
later:   
    jmp loop1
    
exit:
  
        
    
ends

end start ; set entry point and stop the assembler.
