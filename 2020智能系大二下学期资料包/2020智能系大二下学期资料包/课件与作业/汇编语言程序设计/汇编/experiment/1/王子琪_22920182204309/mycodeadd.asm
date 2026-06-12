; multi-segment executable file template.

data segment
    Data1 DW 0F865H
    Data2 DW 360CH 
    pkey db "press any key...$"
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
    
    MOV AX,Data1
    ADD AX,Data2
    MOV DS:[2800H],AX 
    MOV AX,0
    ADC AX,0
    MOV [2802H] ,AX
            
    lea dx, pkey
    mov ah, 9
    int 21h        ; output string at ds:dx
    
    ; wait for any key....    
    mov ah, 1
    int 21h
    
    mov ax, 4c00h ; exit to operating system.
    int 21h    
ends

end start ; set entry point and stop the assembler.
