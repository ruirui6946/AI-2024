;汉诺塔

data segment
    TO DB 'to$'
    FLOOR DB 'please give me the floors of the hanno( 1 to 6 ):',0DH,0AH,'$'
    ENTER DB 0DH,0AH,'the steps are as follows:',0DH,0AH,'$'
ends

stack segment
    dw   128  dup(0)
ends

code segment
start:
    MOV AX,data
    MOV DS,AX
  
    MOV BH,'A';初始柱
    MOV BL,'C';目标柱
    MOV CL,'B';借助的柱子
    ;CH用来交换
    
    LEA DX,FLOOR
    MOV AH,9
    INT 21H
    MOV AH,1
    INT 21H
    
    MOV DH,AL
    SUB DH,30H
    
    PUSH DX
    LEA DX,ENTER
    MOV AH,9
    INT 21H
    POP DX
    
    
    ;MOV DH,3;层数
    
    CALL HANOI;调用汉诺塔函数，即将DH个盘子从初始柱移到目标柱并且借助借助的柱子
    
    MOV AX,4C00H
    INT 21H
    
HANOI PROC;汉诺塔函数
    PUSHA
    CMP DH,1;递归基
    JE DIGUIJI
    
    DEC DH;层数减一

    ;目标柱和借助的柱子交换
    MOV CH,BL
    MOV BL,CL
    MOV CL,CH

    CALL HANOI;调用汉诺塔函数，即将DH-1个盘子从初始柱移到借助的柱子并且借助目标柱
    
    ;换回
    MOV CH,BL
    MOV BL,CL
    MOV CL,CH
    
    JMP PRINT
    
NEXTMOV:

    ;将初始柱和借助的柱子交换
    MOV CH,BH
    MOV BH,CL
    MOV CL,CH
    
    CALL HANOI;调用汉诺塔函数，即将DH-1个盘子从借助的柱子移到目标柱并且借助初始柱

    JMP END_HANOI;结束
    
PRINT:;直接输出路径，即将一个柱子从初始柱移到目的柱
    CALL PATHPRINT
    JMP NEXTMOV
    
DIGUIJI:;递归基，直接将一个盘子从初始柱移动到目标柱
    CALL PATHPRINT
END_HANOI:

    POPA
    RET
HANOI ENDP

PATHPRINT PROC;路径输出函数
    PUSH DX
    
    MOV DL,BH 
    MOV AH,2
    INT 21H
    LEA DX,TO
    MOV AH,9
    INT 21H
    MOV DL,BL
    MOV AH,2
    INT 21H
    MOV DL,' '
    INT 21H
    
    POP DX

    RET
PATHPRINT ENDP


ends

end start ; set entry point and stop the assembler.