DSEG    SEGMENT
MESS    DB   'ABCDEFGHIGKLMNOPQRSTUVWXYZ',0DH,0AH,24H
DSEG    ENDS

SSEG    SEGMENT PARA STACK 
        DW  256 DUP(?)
SSEG    ENDS

CSEG    SEGMENT
        ASSUME  CS:CSEG,DS:DSEG  
        
        
BEGIN:  MOV AX,DSEG
        MOV DS,AX 
        
        MOV BX,OFFSET MESS
        MOV AL,23
        XLAT 
        
        MOV DL,AL  
        MOV AH,2
        
        INT 21H
        MOV AH,4CH
        INT 21H
CSEG    ENDS
        END  BEGIN