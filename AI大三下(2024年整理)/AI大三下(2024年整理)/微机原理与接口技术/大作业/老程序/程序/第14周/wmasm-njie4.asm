DATAS SEGMENT
    ;此处输入数据段代码  
DATAS ENDS

STACKS SEGMENT
    ;此处输入堆栈段代码
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
NJIE MACRO NA  
    LOCAL PRINT0
    MOV BL,NA
    CMP BL,0
    JE PRINT0
    CMP BL,1
    JE PRINT0
    MUL BX
    DEC BL
    
    IF NA	
    NJIE NA-1
    ENDIF
    
PRINT0:    
ENDM

    MOV AX,DATAS
    MOV DS,AX
    
    MOV AX,1
    MOV BX,0

    NJIE 8
    ;MOV DL,AL
    ;ADD DL,30H
    ;MOV AH,2
    ;INT 21H
    
    ;MOV EAX,1111FFFEH
    ;ADD EAX,1
    ;MOV EBX,EAX
    ;MOV EAX,EBX
    ;DEC EAX
    
    MOV DX,10
    PUSH DX
    MOV DX,0
    ;;;;
PUSH_AGAIN:

    MOV CX,10
    DIV CX
    MOV BX,DX
    PUSH BX
    CMP AX,0
    JE POP_AGAIN
    MOV DX,0
    JMP PUSH_AGAIN
    
POP_AGAIN:
    
    POP DX
    CMP DX,10
    JE  END_OUT
    ADD DX,30H
    MOV AH,2
    INT 21H
    ;;
    ;MOV AH,1
    ;INT 21H
    ;;
    JMP POP_AGAIN
    
END_OUT: 
    ;MOV AH,1
    ;INT 21H   
    MOV AH,4CH
    INT 21H
    
CODES ENDS
    END START

