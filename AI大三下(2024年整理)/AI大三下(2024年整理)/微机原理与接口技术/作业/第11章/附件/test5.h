AD_SUB  PROC NEAR
            MOV CX,8            ;计数器
            CLD                 ;清方向标志
            MOV BL,00H          ;通道号，从0开始
            LEA DI,D_BUF        ;缓冲区偏移地址
NEXT_IN:    MOV DX,3C2H         ;C口地址
            MOV AL,BL
            OUT DX,AL           ;输入通道号
            MOV DX,3C3H         ;控制口地址
            MOV AL,00000111B    ;PC3置1
            OUT DX,AL           ;输出通道号
            NOP
            NOP
            NOP
            MOV AL,00000110B    ;PC3复位
            OUT DX,AL
            MOV DX,3C2H
NO_CONV:    IN AL,DX
            TEST AL,80H
            JNZ NO_CONV         ;PC7=1,循环等待
NO_EOC:     IN AL,DX            ;PC7=1,已经启动转换
            TEST AL,80H
            JZ NO_EOC           ;PC7=0,转换未结束，循环等待
            MOV DX,3C0H
            IN AL,DX            ;读入数据
            STOS D_BUF          ;存数据
            INC BL              ;下个通道
            LOOP NEXT_IN        ;循环下个通道直到结束
            RET
AD_SUB  ENDP
