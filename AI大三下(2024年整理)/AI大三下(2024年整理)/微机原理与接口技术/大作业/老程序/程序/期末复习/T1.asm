; multi-segment executable file template.

data segment
    ; add your data here!
    pkey db "press any key...$"
    DATA1 DB '123456'
    DATA2 DB '332434'
ends

stack segment
    dw   128  dup(?)
ends

code segment
start:
; set segment registers:
    mov ax, data
    mov ds, ax
    mov es, ax
    
    
    ADD BX,(6-1)*2
    
    AND AX,AX
    
    MOV AX,0
    SUB AX,1
    ADD AX,1
    
    AND AX,AX
    
    LEA SI,DATA1
    LEA DI,DATA2
    MOV CX,5
    REPNE CMPSB
    
    
    ;MOV AL,1
    ;MOV AH,2
    
    JMP LL2
    
    LL1:
    INC AX
    LL2:
    MOV AX,LL2
    
    MOV AX,-90
    NEG AX
    NEG AX
    
    MOV AX,0
    NEG AX
    
    IDIV BYTE PTR 9
    
    MOV AL,8FH
    ADD AL,1
    DAA
    
    MOV AL,8FH
    INC AL
    DAA
    
    
    
    
LEBAL1:
    ;PUSH AX
    ;PUSH AX
    
    ;MOV DX,offset pkey
    ;MOV AL,BYTE PTR 4[BX+(3-1)*2][DI]
    
    ;CALL PRO

    ; add your code here
            
    lea dx, pkey
    mov ah, 9
    int 21h        ; output string at ds:dx
    
    ; wait for any key....    
    mov ah, 1
    int 21h
    
    mov ax, 4c00h ; exit to operating system.
    int 21h
    
    PRO PROC
        ADD AX,1
        
        RET
    ENDP
    
ends

end start ; set entry point and stop the assembler.
