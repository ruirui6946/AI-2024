DATA SEGMENT
DBUF DB 8*5000 DUP(?)   ;8*5000字节
DATA ENDS

;...堆栈段等

;数据采集子程序
CODE SEGMENT
     ASSUME CS:CODE,DS:DATA
AD_8 PROC FAR
    ;8253初始化编程，通道0，方式2
    ;先写低字节，后写高字节，BCD，时间常数4000
        MOV DX,303H
        MOV AL,00110101B
        OUT DX,AL
        MOV DX,300H         ;通道0
        MOV AX,4000H        ;时间常数4000
        OUT DX,AL           ;低字节
        MOV AL,AH
        OUT DX,AL           ;高字节

    ;8359A设置屏蔽字，仅允许8259A和键盘中断，其余禁止
        MOV AL,11111001B    ;屏蔽字
        MOV DX,305H
        OUT DX,AL

    ;数据有关服务
        MOV SI,OFFSET DBUF  ;指向数据缓冲区
        MOV BX,5000
        STI                 ;开中断
AGAIN:  CMP BX,0
        JNZ AGAIN           ;一直循环到采集完
        MOV AL,11111101B    ;完成，禁止IR2中断
        MOV DX,305H
        MOV AH,4CH          ;退出中断
        INT 21H
        RET
AD_8 ENDP

;中断服务子程序，对每个通道均采集一个数据，5000次，存入DBUF
ADINT PROC NEAR
        MOV CX,0008H        ;8次
        MOV DX,308H
NEXT:   OUT DX,AL
        PUSH DX             ;保存通道号
        MOV DX,306H         ;状态口
POLL:   IN AL,DX            ;读入EOC
        TEST AL,80H
        JNZ POLL            ;循环等待
NO_END: IN AL,DX
        TEST AL,80H
        JZ NO_END
        POP DX
        IN AL,DX
        MOV [SI],AL
        INC DX              ;下一通道
        INC SI
        LOOP NEXT
        DEC BX              ;采集完一轮，BX-1
        MOV AL,20H
        MOV DX,304H
        OUT DX,AL
        STI
        IRET
ADINT ENDP
CODE  ENDS
      END

