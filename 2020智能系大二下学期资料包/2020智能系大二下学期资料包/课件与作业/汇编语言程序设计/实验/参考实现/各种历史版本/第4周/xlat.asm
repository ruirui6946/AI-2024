DSEG    SEGMENT
MESS    DB   'my name is hsxz' ,0DH,0AH,
             'my student ID is 22920172204211',0DH,0AH,
             'What we cannot speak about we must pass over in silence.',
             0DH,0AH,24H
DSEG    ENDS

SSEG    SEGMENT PARA STACK 
        DW  256 DUP(?)
SSEG    ENDS

CSEG    SEGMENT
        ASSUME  CS:CSEG,DS:DSEG
BEGIN:  MOV AX,DSEG
        MOV DS,AX
        MOV DX,OFFSET MESS
        MOV AH,9
        
        INT 21H
        MOV AH,4CH
        INT 21H
CSEG    ENDS
        END  BEGIN
