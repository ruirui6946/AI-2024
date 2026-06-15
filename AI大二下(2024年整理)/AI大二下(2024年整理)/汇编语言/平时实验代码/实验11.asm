DATAS SEGMENT
    msg1 DB '10 numbers:','$'
    msg2 DB 'After Sort',0ah,0dh,'$'
    msg3 DB ' ','$'
    numbers DW 10 DUP(?)
DATAS ENDS

STACKS SEGMENT
    DW 256 dup(?)
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    MOV SI, 0
    MOV CX, 10
    MOV DX, OFFSET msg1
    MOV AH, 9
    INT 21H
;输入
Loop1:
    CALL Input
    ADD SI, 2
    Loop Loop1

    CALL CRLF

    MOV CX, 9
    MOV DI, 0
;排序
FOR1:
    CALL Sort
    ADD DI, 2
    LOOP FOR1

    CALL CRLF
    MOV DX, OFFSET msg2
    MOV AH, 9
    INT 21H
;输出
    MOV CX, 10
    MOV DI, 0
FOR2:
    CALL Print
    CALL Space
    ADD DI , 2
    LOOP FOR2
    CALL CRLF

    MOV AH, 4CH
    INT 21H

Input PROC Near
    push AX
    push BX
    push CX
    push DX

    MOV BX, 0
    CLC
    MOV DX, 0

    LOOP01:
        MOV AH, 1
        INT 21H
        CMP AL, 20H ;空格
        JE OVVVVVER
    
        SUB AL, 30H
        MOV AH, 0
        XCHG AX, BX
        MOV CX, 10
        MUL CX
        ADD AX , BX
        XCHG AX, BX 
        JMP LOOP01
    OVVVVVER:
        MOV DX, 0
        MOV numbers[SI], BX ;
        POP DX
        POP CX
        POP BX
        POP AX
        RET
        Input ENDP

CRLF PROC Near
    push AX
    push DX
    MOV DL, 0ah
    MOV AH, 2
    INT 21H
    pop DX
    pop AX
    RET
    CRLF ENDP

Space PROC Near
    push AX
    push DX
    MOV DX, OFFSET msg3
    MOV AH, 9
    INT 21H
    pop DX
    pop AX
    RET 
    Space ENDP

Print PROC Near
    PUSH AX
    PUSH BX
	PUSH CX
	PUSH DX 

    MOV CX, 0
    MOV BX, 10
    MOV AX, numbers[DI]
    DDDDIV:
        MOV DX, 0
        DIV BX
        PUSH DX;放入dx，等下输出
        INC CX
        CMP AX, 0;看看有没有除尽
        JNZ DDDDIV
    PPPPRINT:
        POP DX
        OR DX, 30H
        MOV AH, 2
        INT 21H
        LOOP PPPPRINT
        POP  DX
        POP  CX
	    POP  BX
	    POP  AX
	    RET
        Print ENDP

SORT PROC NEAR
    PUSH BX
    PUSH DX
    MOV  SI,DI

LOOOOP:
    ADD  SI,2  
    MOV  BX,numbers[DI]
    CMP  BX,numbers[SI]
    JA   SWAP
    JMP  NEXT
SWAP:    
    MOV  DX,numbers[SI]
    MOV  numbers[DI],DX
    MOV  numbers[SI],BX 
NEXT:
    CMP SI,18
    JL   LOOOOP

    POP  DX
    POP  BX
    RET
    SORT ENDP   

CODES ENDS
    END START
