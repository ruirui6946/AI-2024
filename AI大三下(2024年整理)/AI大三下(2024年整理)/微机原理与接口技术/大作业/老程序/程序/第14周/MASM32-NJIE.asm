.386
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
    MUL BL
    DEC BL
    
    NJIE BL
    
PRINT0:
    MOV DL,'1'
    MOV AH,2
    INT 21H  
ENDM

    MOV AX,DATAS
    MOV DS,AX
    
    MOV BL,0
    NJIE BL
    MOV AL,BL
    MOV AH,2
    INT 21H
    
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START