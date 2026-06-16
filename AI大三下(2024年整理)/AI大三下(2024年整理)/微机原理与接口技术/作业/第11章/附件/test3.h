MOV DX,220H     ;低4位寄存器地址
MOV CL,4
MOV BX,BUFF
SHL BX,CL       ;左移4位
MOV AL,BH       ;高8位数据
OUT DX,AL       ;输出高8位

INC DX          ;低4位寄存器地址
MOV AL,BL       ;低4位数据
OUT DX,AL       ;输出低4位数据

MOV DX,222H     ;DAC寄存器
OUT DX,AL       ;启动12位数据转换
