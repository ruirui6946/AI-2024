.386
.model flat,stdcall
.data
.code

START:
NJIE MACRO NA  
    LOCAL PRINT0
    MOV BL,NA
    CMP BL,0
    JE PRINT0
    CMP BL,1
    JE PRINT0
    MUL EBX
    DEC BL
    
    IF NA	
    NJIE NA-1
    ENDIF
    
PRINT0:    
ENDM

    
    MOV EAX,1
    MOV EBX,0

    NJIE 0
    ;MOV DL,AL
    ;ADD DL,30H
    ;MOV AH,2
    ;INT 21H
    
    MOV DX,10
    PUSH DX
    MOV EDX,0
PUSH_AGAIN:
    MOV ECX,10
    DIV ECX
    MOV EBX,EDX
    PUSH BX
    CMP EAX,0
    JE POP_AGAIN
    MOV EDX,0
    JMP PUSH_AGAIN
    
POP_AGAIN:
    POP DX
    CMP DX,10
    JE END_OUT
    ADD DX,30H
    MOV AH,2
    INT 21H
    JMP POP_AGAIN
    
END_OUT:
    MOV AH,1
    INT 21H    
    MOV AH,4CH
    INT 21H
    
    END START
