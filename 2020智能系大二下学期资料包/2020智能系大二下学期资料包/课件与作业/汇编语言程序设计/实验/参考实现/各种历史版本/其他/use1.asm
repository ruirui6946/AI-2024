DATAS SEGMENT
    ;此处输入数据段代码
    DATA DB 100 DUP(22H)
    DATA2 DB 0DH,0AH,'HELLO WORLD',0DH,0AH,'hello world!','$'
    DATA3 DB 0DH,0AH,'$'  
DATAS ENDS

STACKS SEGMENT
    ;此处输入堆栈段代码
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    
    LEA AX,[DATA2 +2]
    MOV CX,OFFSET DATA2
    MOV BL,80H
    XCHG AX,BX
    
    MOV BYTE PTR DATA2[2],'h'
    MOV WORD PTR DATA2[3],'le'
    
    MOV AX,93H
    MOV BX,DS:[80]
    
    ;带符号扩展
    CBW
    ;AL->AX
    CWD
    ;AX->DX:AX
    
    MOV BX,OFFSET DATA2
    MOV AL,4
    XLAT
    
    LEA DX,DATA3
    MOV AH,9
    INT 21H
    
    ;MOV DL,AL
    MOV DL,6CH
    MOV AH,2
    INT 21H
    
    MOV DL,'S'
    INT 21H
    
    MOV DX,OFFSET DATA2
    MOV AH,9
    INT 21H
       
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START