; multi-segment executable file template.

data segment
    ; add your data here!
    a db 1 dup(1)
    b dw 1 dup(2)
    c dd 2 dup(012345678H)
    pkey db 5 dup(0),"$" 

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

    ; add your code here
    IRP REG,<AX,BX,CX,DX>
    PUSH REG
    ENDM        
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
