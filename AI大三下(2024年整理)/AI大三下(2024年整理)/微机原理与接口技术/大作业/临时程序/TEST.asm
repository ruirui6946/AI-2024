DSEG    SEGMENT
MESS    DB  01H,02H,03H,33H,0D4H,0F5H,67H,00H
O_MESS  DB  00H,00H,04H,34H,00H,0FFH,00H,04H
TEM_CH  DB  00H,00H,00H,00H,00H,00H,00H,00H ;00H不变，01H升高，02H降低
FIRST   DB  08H                 ;第一组不需要输出气温比较
P_8253  DW  304H,305H,306H,307H ;8253通道地址
P_8259A DW  308H,309H           ;8259A通道地址
P_8251A DW  312H,313H           ;8251A通道地址
INT_NUM DB  50H                 ;初始中断类型号
TEM1    DB  'The temperature of channel $'
TEM2    DB  ' is $'
TEM3    DB  ' Celsius. $'
N_LINE  DB  0DH,0AH,'$'
UP      DB  'The temperature is rising.',0DH,0AH,'$'
DOWN    DB  'The temperature is decreasing.',0DH,0AH,'$'
SAME    DB  'No change in temperature.',0DH,0AH,'$'
OVER    DB  'WARNING! The temperature of this channel exceeds 60 degrees Celsius',0DH,0AH,'$'
DSEG    ENDS

SSEG    SEGMENT PARA STACK 
        DW  256 DUP(?)
SSEG    ENDS

CSEG    SEGMENT
        ASSUME  CS:CSEG,DS:DSEG
BEGIN:  MOV AX,DSEG
        MOV DS,AX
        
        MOV CX,0008H                ;接收8组数据
        CLD
        MOV BX,0000H                ;从0号端口出发,同时作为一个偏移
        LEA DI,MESS                 ;新数据
        LEA SI,O_MESS               ;旧数据 
T_CMP:  MOV AH,[DI]
        MOV AL,[SI]
        CMP AH,AL
        JE T_SAME
        CMP AH,AL
        JA T_UP
        MOV TEM_CH[BX],02H
        JMP T_END
T_UP:   MOV TEM_CH[BX],01H
        JMP T_END
T_SAME: MOV TEM_CH[BX],00H
T_END:  MOV [SI],AH
        INC BX
        INC DI
        INC SI
        LOOP T_CMP
        
        ;MOV AX,0098H
        ;CALL TEM_GET
        ;CALL PRINT10
        
        ;打印信息
        MOV CX,0008H                ;接收8组数据
        CLD
        MOV BX,0000H                ;从0号端口出发,同时作为一个偏移
        LEA DI,MESS                 ;温度数据
        LEA SI,TEM_CH               ;温度变化信息
P_LOOP: MOV DX,OFFSET TEM1
        CALL PRINT_MESS
        MOV AX,BX
        CALL PRINT10
        MOV DX,OFFSET TEM2
        CALL PRINT_MESS
        MOV AL,[DI]
        MOV AH,0
        CALL TEM_GET
        CALL PRINT10
        MOV DX,OFFSET TEM3
        CALL PRINT_MESS
        
        MOV AL,FIRST
        CMP AL,08H
        JB PRINTL                   ;目前是第一次输出
        MOV AL,[SI]
        CMP AL,01H                  ;温度升高
        JE P_UP
        CMP AL,02H
        JE P_DOWN
        MOV DX,OFFSET SAME
        JMP P_END
P_UP:   MOV DX,OFFSET UP
        JMP P_END
P_DOWN: MOV DX,OFFSET DOWN
        JMP P_END       
PRINTL: INC AL
        MOV FIRST,AL
        MOV DX,OFFSET N_LINE
P_END:  CALL PRINT_MESS
        MOV AL,[DI]
        CALL TEM_GET
        CMP AL,3CH                  ;即60度
        JB R_LOOP
        MOV DX,OFFSET OVER
        CALL PRINT_MESS
R_LOOP: INC DI
        INC SI
        INC BX
        LOOP P_LOOP
        

        
        INT 21H
        MOV AH,4CH
        INT 21H
        
;将数字量转换为温度数值
;本应该是除以2.56，不使用浮点数计算的情况下，先乘25，后除以64
;为了安全起见，结果向上取整
TEM_GET PROC
        PUSH BX
        MOV BL,19H                  ;即25
        MUL BL
        MOV BH,40H                  ;即64
        DIV BH
        MOV AH,0
        INC AL
        POP BX
        RET
TEM_GET ENDP


;10进制输出函数
;将AL的数据打印出来（注意AH尽量给00H，否则可能发生错误）
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
        PUSH DX         ;除以10，压栈
        CMP AL,0        ;除尽，开始输出
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
        ;数字输出结束后，可以添加一些间隔符
        ;MOV DL,' '
        ;MOV AH,2
        ;INT 21H
    
        POP AX
        POP DX
        RET
PRINT10 ENDP

;打印信息函数，将DX指向的数据串打印出来
PRINT_MESS PROC
        PUSH AX
        MOV AH,9
        INT 21H
        POP AX
        RET
PRINT_MESS ENDP  
      
CSEG    ENDS
        END  BEGIN
