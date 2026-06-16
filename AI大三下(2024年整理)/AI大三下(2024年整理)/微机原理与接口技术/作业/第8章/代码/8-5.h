;通道0初始化
MOV AL,36H   ;方式3，先写低8位,2进制
OUT 43H,AL   ;写控制端口

MOV AL,00H
OUT 40H,AL   ;写通道0端口
MOV AL,00H
OUT 40H,AL

;通道1初始化
MOV AL,55H   ;方式2，只写低8位,BCD
OUT 43H,AL   ;写控制端口

MOV AL,18H
OUT 41H,AL   ;写通道1端口

;通道2初始化
MOV AL,B7H   ;方式3，先写低8位,BCD
OUT 43H,AL   ;写控制端口

MOV AL,96H
OUT 42H,AL   ;写通道2端口
MOV AL,05H
OUT 42H,AL

IN AL,PORT_B    ;取8255AB端口的当前值
MOV AH,AL		;保存端口的值
OR AL,03H		;使PB0和PB1均置1
OUT PORT_B,AL	;接通扬声器
