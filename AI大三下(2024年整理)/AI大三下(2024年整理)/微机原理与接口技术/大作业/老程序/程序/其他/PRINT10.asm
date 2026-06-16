DATAS SEGMENT
    ;此处输入数据段代码  
DATAS ENDS

STACKS SEGMENT
    ;此处输入堆栈段代码
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    
    MOV AL,6CH
    MOV AH,0
    
    CALL PRINT10
    
    MOV AH,4CH
    INT 21H
    
PRINT10 PROC
    PUSH DX
    PUSH AX
    
    MOV DX,10
    PUSH DX
    
PUSH_AGAIN:

    MOV DL,10
    DIV DL
    MOV DL,AH
    MOV DH,0
    PUSH DX
    CMP AL,0
    JE POP_AGAIN
    MOV AH,0
    JMP PUSH_AGAIN
    
POP_AGAIN:
    
    POP DX
    CMP DX,10
    JE PRINT_END
    ADD DX,30H
    MOV AH,2
    INT 21H
    JMP POP_AGAIN
        
    
PRINT_END:    
    
    MOV DL,' '
    MOV AH,2
    INT 21H
    
    POP AX
    POP DX
    RET
        ENDP    
    
CODES ENDS
    END START