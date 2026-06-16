DATAS SEGMENT
    DATA1 BYTE 22H,'f',03H,04H,05H,06H,07H,08H,09H,0AH
          BYTE 0BH,0CH,0DH,0EH,0FH,10H,11H,12H,13H,14H
    DATA2 BYTE 'A','B','C','D','E','F'
          BYTE 'G','H','I','J','K','L'
          BYTE 'M','N','O','P','Q','R'
    array BYTE 'A','B','C','D','E'
          BYTE 'F','G','H','I','J'
          BYTE 'K','L','M','N','O'
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
    
    ;寄存器间接寻址
    ;MOV BX,OFFSET DATA1
    ;MOV AL,[BX]
    ;MOV DL,AL
    
    ;寄存器相对寻址
    ;MOV SI,1H
    ;MOV AL,DATA1[SI]
    ;MOV DL,AL
    
    ;MOV BX,OFFSET DATA2
    ;MOV SI,09H
    
    ;MOV BYTE PTR [BX+SI],'d'
    ;MOV AL,[BX+SI]
    ;MOV DL,AL
    
    ;相对基址变址寻址
    MOV BX,5H
    MOV SI,2H
    
    ;MOV BYTE PTR array[BX][SI],'a'
    ;MOV AL,array[BX][SI]
    ;MOV DL,AL
    
    MOV AH,02H
    
    INT 21H
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START
