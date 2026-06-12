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
    LOCAL PRINT0,MULADD,DECBX
    MOV BL,NA
    CMP BL,0
    JE PRINT0
    CMP BL,1
    JE PRINT0
    CMP DX,0
    JNE MULADD
    MUL BX
    JMP DECBX
MULADD:
    PUSH CX
    PUSH AX
    MOV AX,DX
    MUL BX
    MOV DX,AX
    POP AX
    PUSH DX
    MUL BX
    POP CX
    ADD DX,CX
    POP CX 
    
DECBX:
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

    NJIE 8;调用宏
    
    ;;
    ;MOV DX,1
    ;MOV AX,0
    ;;
    
    MOV BX,10
    PUSH BX
    CMP DX,0
    JE PUSH_AGAIN
    MOV CX,100;10阶会溢出
    DIV CX
    ;PUSH AX
    ;MOV AX,DX
    ;CMP AX,10
    ;JAE DIV10
    ;POP AX
    ;PUSH DX
    ;JMP PUSH_AGAIN
;DIV10:
    ;MOV CL,10
    ;DIV CL
    ;MOV CX,AX
    ;POP AX
    ;MOV DH,0
    ;MOV DL,CH
    ;PUSH DX
    ;MOV DL,CL
    ;PUSH DX
    
    ;;;;
    MOV DX,0
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

    JMP POP_AGAIN
    
END_OUT: 
       
    MOV AH,4CH
    INT 21H
    
CODES ENDS
    END START

