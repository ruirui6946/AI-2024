DATAS SEGMENT
    NODENAME DB 10 DUP(00H) ;储存节点名称的数组NODENAME
    LEN EQU $-NODENAME ;最大存储容量
    LEN1 DB 00H      ;字节型存储节点数的变量LEN1
    LEND DD 00H,00H  ;字型存储节点数的变量LEND
    
    STARTNODE DB 00H ;存储起始节点的变量STARTNODE
    STARTNUM DB 00H  ;其在NODENAME中的标号STARTNUM

    ENDNODE DB 00H   ;存储目标节点的变量ENDNODE
    ENDNUM DB 00H    ;其在NODENAME中的标号ENDNUM
    
    CHECK DB LEN DUP(00H) ;检查节点是否已经在S集合（已经求得最短路径的节点）中的数组CHECK（在S中，为01H；不在为00H）
    
    PATH DB 100 DUP(00H)  ;记录节点间路径长度的数组PATH（字节型，0FFH代表无穷大）
         
    DIS DB LEN DUP(0FFH)  ;存放目前找到的从起始节点到各节点的最短路径数组DIS（初始均为无穷大，即0FFH）
    FATHER DB LEN DUP(0FFH)  ;存放当前找到路径的父节点数组FATHER（初始00H，并规定节点名称对应的ASCII码值不能为00H）
    
    OUTPATH DB LEN DUP(0FFH);输出路径，辅助用，可看做小堆栈
    
    ;输出信息相关
    CANNOT DB 'cannot find a way',0DH,0AH,'$'  ;输出找不到路径信息
    MAKEIT DB 'find a way:',0DH,0AH,'$'        ;输出找到路径信息
    COST DB 0DH,0AH,'cost:',0DH,0AH,'$'        ;输出cost提示信息
    INNUM DB 'please give the number of nodes',0DH,0AH,'$'  ;提示输入节点数
    INNODE DB 'please give the name of nodes, each node need only one acsii',0DH,0AH,'$' ;提示输入节点名称
    INSTART DB 0DH,0AH,'please give me the name of start node',0DH,0AH,'$' ;提示输入初始节点
    INEND DB 0DH,0AH,'please give me the name of end node',0DH,0AH,'$'     ;提示输入目标节点
    INCOST1 DB 'please give the cost from $'   ;提示输入节点间权值1
    INCOST2 DB ' to $'                         ;提示输入节点间权值2
    INCOST3 DB ':',0DH,0AH,'$'                 ;提示输入节点间权值3
DATAS ENDS

STACKS SEGMENT
    ;此处输入堆栈段代码
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    
    ;CIN为十进制输入函数，当输入不在0-9之间是将之前的输入的十进制保存在DL中
    ;PRINT10为十进制输出函数，将AL以十进制输出
    
     ;MOV AL,LEN
;输入层
     ;输入节点数
     LEA DX,INNUM
     MOV AH,9
     INT 21H
     CALL CIN
     MOV BYTE PTR LEN1,DL;保存在LEN1中
     MOV BYTE PTR LEND,DL;保存在LEND中
     
     ;输入节点
     ;一个字符代表一个节点，原则上不允许相同字符输入
     MOV SI,0
     LEA DX,INNODE
     MOV AH,9
     INT 21H
NODE_IN:
     CMP SI,WORD PTR LEND;控制循环次数
     JE NODE_IN_END
     
     MOV AH,1
     INT 21H
     MOV BYTE PTR NODENAME[SI],AL;将节点名依次放入NODENAME
     INC SI
     JMP NODE_IN
     
NODE_IN_END:
     
     ;输入初始节点
     LEA DX,INSTART
     MOV AH,9
     INT 21H
     
     MOV AH,1
     INT 21H
     MOV BYTE PTR STARTNODE,AL
     
     ;输入目标节点
     LEA DX,INEND
     MOV AH,9
     INT 21H
     
     MOV AH,1
     INT 21H
     MOV BYTE PTR ENDNODE,AL
     
     ;一个换行
     MOV DL,0DH
     MOV AH,2
     INT 21H
    
     MOV DL,0AH
     MOV AH,2
     INT 21H
     
     ;输入cost
     MOV BX,1
     MOV CX,0
     
     ;将所有不同节点间的权值输入
COST_AGAIN1:;外循环
     CMP BL,BYTE PTR LEN1
     JE NEXT_COST1
     MOV CL,0
COST_AGAIN2:;内循环
     CMP CL,BL
     JE NEXT_COST2
     MOV DI,CX
     MOV SI,BX
     ;DI存放节点1（目前想知道的两个节点--节点1和节点2--间的权值）在NODENAME中的标号
     ;SI存放节点2（目前想知道的两个节点--节点1和节点2--间的权值）在NODENAME中的标号
     
     ;输出提示信息
     LEA DX,INCOST1
     MOV AH,9
     INT 21H
     
     ;输出节点1
     MOV DL,BYTE PTR NODENAME[DI]
     MOV AH,2
     INT 21H
     
     LEA DX,INCOST2
     MOV AH,9
     INT 21H
     
     ;输出节点2
     MOV DL,BYTE PTR NODENAME[SI]
     MOV AH,2
     INT 21H
     
     LEA DX,INCOST3
     MOV AH,9
     INT 21H
     
     ;输入节点1到节点2的权值
     CALL CIN
     
     ;要将权值放入PATH[DI][SI]中（C语言表示法），即要将权值放入PATH[DI*LEN1+SI]中
     MOV DH,BYTE PTR LEN1
     MOV AX,DI
     MUL DH;将DI*LEN1保存在AX中
     
     ;将SI的值放入堆栈，保存
     PUSH SI
     ADD SI,AX;将DI*LEN1加到SI中，即SI=DI*LEN1+SI
     MOV BYTE PTR PATH[SI],DL
     ;释放SI
     POP SI
     
     ;反向（有向图，权值有方向）
     ;和上述代码基本一样
     
     LEA DX,INCOST1
     MOV AH,9
     INT 21H
     
     MOV DL,BYTE PTR NODENAME[SI]
     MOV AH,2
     INT 21H
     
     LEA DX,INCOST2
     MOV AH,9
     INT 21H
     
     MOV DL,BYTE PTR NODENAME[DI]
     MOV AH,2
     INT 21H
     
     LEA DX,INCOST3
     MOV AH,9
     INT 21H
     
     CALL CIN
      
     MOV DH,BYTE PTR LEN1
     MOV AX,SI
     MUL DH
     ADD DI,AX
     MOV BYTE PTR PATH[DI],DL
     
     INC CL
     JMP COST_AGAIN2
     
NEXT_COST2:;内循环结束
     INC BL
     JMP COST_AGAIN1

NEXT_COST1:;外循环结束，输入权值结束
    
;计算层
    ；
    MOV AL,BYTE PTR LEN1
    MOV BYTE PTR LEND,AL
    ;得到初始节点标号
    MOV CL,BYTE PTR STARTNODE
    MOV BX,0
FIND_S:
    CMP CL,BYTE PTR NODENAME[BX]
    JE SET_S
    INC BX
    JMP FIND_S

SET_S:
    MOV BYTE PTR STARTNUM,BL;初始节点标号写入STARTNUM
    
    ;得到目标节点标号
    MOV CL,BYTE PTR ENDNODE
    MOV BX,0
FIND_E:
    CMP CL,BYTE PTR NODENAME[BX]
    JE SET_E
    INC BX
    JMP FIND_E

SET_E:
    MOV BYTE PTR ENDNUM,BL;目标节点标号写入ENDNUM
    
    ;寻找路径
    MOV CH,BYTE PTR STARTNUM;存放初始节点,比较用
    MOV BL,BYTE PTR STARTNUM;存放初始节点在BX中，寻址用
    MOV BH,0
    MOV BYTE PTR CHECK[BX],01H;初始节点已经被走过
    MOV BYTE PTR DIS[BX],00H;自己到自己距离为零
    MOV BYTE PTR FATHER[BX],CH;自己是自己的父亲，结束标志
    
AGAIN:
    MOV BL,CH;当前节点已经在CH中
    MOV BH,00H;存放当前节点在BX中，寻址用
    CMP BYTE PTR DIS[BX],0FFH
    JE FALSE;最短路径为无穷大,说明剩余节点均不可导通
    
    CMP CH,BYTE PTR ENDNUM
    JE TOOUT;找到目标节点且最短路径不为无穷大，则输出路径与权值信息
    
    MOV DL,BYTE PTR DIS[BX];把从初始节点到当前节点的长度读入DL
    
    MOV DI,0;初始化变址
    
    MOV AL,BYTE PTR LEN1
    MUL CH
    MOV BX,AX;得到所需的一组路径
SET_DIS:
    CMP DI,WORD PTR LEND
    JE FIND_MIN;已经将所有非S集合节点遍历过，转到寻找新的当前节点的步骤
    CMP BYTE PTR CHECK[DI],01H
    JE NEXT;已经在S集合中，不需要再次访问，直接跳过
    
    CMP BYTE PTR PATH[BX][DI],0FFH
    JE NEXT;不连通，直接跳过
    
    MOV AL,BYTE PTR DIS[DI];之前找到的最短路径长度
    ;保护DX
    PUSH DX
    ADD DL,BYTE PTR PATH[BX][DI];从当前节点走的最短路径
    CMP AL,DL
    JBE NEXT_POP
    ;从当前节点走的最短路径小的话则更新路径
    MOV BYTE PTR DIS[DI],DL
    ;并且更新其父节点
    MOV BYTE PTR FATHER[DI],CH
    
NEXT_POP:
    POP DX;释放DX
NEXT:
    INC DI
    JMP SET_DIS
    
    ;1.找到最短点，给CH
    ;2.CH的CHECK设1
FIND_MIN:
    MOV DI,0
    MOV CL,CH
    MOV SI,00H
    MOV DH,0FFH
AGAIN_MIN:
    CMP DI,WORD PTR LEND
    JE SET_CH;遍历完成
    
    CMP BYTE PTR CHECK[DI],01H
    JE NEXT_MIN;已经被检查
    
    CMP DH,BYTE PTR DIS[DI]
    JB NEXT_MIN;不需要修改
    
    MOV DH,BYTE PTR DIS[DI]
    MOV SI,DI

NEXT_MIN:
    INC DI
    JMP AGAIN_MIN    
    
SET_CH:
    MOV AX,SI
    MOV CH,AL;1.找到最短点，给CH
    MOV BYTE PTR CHECK[SI],01H;2.CH的CHECK设1
    
    JMP AGAIN

;输出层

TOOUT:
    LEA DX,MAKEIT
    MOV AH,9
    INT 21H
    
    MOV BH,0
    MOV CL,BYTE PTR STARTNUM;初始节点给CL
    
    MOV BL,BYTE PTR ENDNUM;目标节点给BL
    MOV DI,0    
OUT_AGAIN:
    CMP BL,CL
    JE PRINT_OUT;已经回溯到父节点（父节点的父节点和自身相同），开始输出
    MOV BYTE PTR OUTPATH[DI],BL;将其压栈（OUTPATH作为临时输出栈）
    MOV BL,BYTE PTR FATHER[BX];取其父节点
    INC DI
    JMP OUT_AGAIN
    
;输出路径
PRINT_OUT:
    ;先输出开始节点
    MOV DL,BYTE PTR STARTNODE
    MOV AH,2
    INT 21H
    
    DEC DI
PRINT_OUT2:
    CMP DI,0;栈空则停止输出
    JL PRINT_COST
    MOV DL,'-'
    INT 21H
    MOV DL,'>'
    INT 21H
    
    MOV BH,0
    MOV BL,BYTE PTR OUTPATH[DI];取栈顶之下第一个元素
    MOV DL,BYTE PTR NODENAME[BX];取栈顶之下第一个元素代表的节点
    INT 21H
    DEC DI;弹栈
    JMP PRINT_OUT2
    
    ;JMP ENDD

;输出最终的cost值
PRINT_COST:
    
    LEA DX,COST
    MOV AH,9
    INT 21H
    
    MOV BH,0
    MOV BL,BYTE PTR ENDNUM
    MOV CH,0
    MOV CL,BYTE PTR DIS[BX]
    MOV AX,CX
    
    CALL PRINT10
    
    JMP ENDD


;未找到路径情况
FALSE:
    LEA DX,CANNOT
    MOV AH,9
    INT 21H
ENDD:  
    MOV AH,4CH
    INT 21H
    
;十进制输出函数
PRINT10 PROC
    PUSH DX
    PUSH AX
    
    MOV DX,10
    PUSH DX
    ;将10压栈
    
PUSH_AGAIN:
    
    ;AX除以10，将余数压栈
    MOV DL,10
    DIV DL
    MOV DL,AH
    MOV DH,0
    PUSH DX
    
    ;商为0则结束压栈操作
    CMP AL,0
    JE POP_AGAIN
    MOV AH,0
    JMP PUSH_AGAIN
    
POP_AGAIN:
    
    POP DX
    ;当弹出10则停止输出操作
    CMP DX,10
    JE PRINT_END
    ;弹栈并进行输出
    ADD DX,30H
    MOV AH,2
    INT 21H
    JMP POP_AGAIN
        
    
PRINT_END:    
    
    ;输出完后打印空格
    MOV DL,' '
    MOV AH,2
    INT 21H
    
    POP AX
    POP DX
    RET
ENDP

;十进制输入函数
CIN PROC
    PUSH AX
    PUSH BX
    
    MOV AL,0
    MOV DH,10
    MOV DL,0

IN_AGAIN:
    
    ;输入单个字符
    MOV AH,1
    INT 21H
    SUB AL,30H
    
    ;不在0-9之前，则结束输入操作
    CMP AL,0
    JB IN_END
    CMP AL,9
    JA IN_END
    
    ;之前得到的数乘10，再加上新输入的数
    MOV BL,AL
    XCHG AL,DL
    MUL DH
    ADD AL,DL
    MOV DL,AL
    MOV AL,BL
    JMP IN_AGAIN
    
IN_END:
    PUSH DX
    
    ;输出换行
    MOV DL,0DH
    MOV AH,2
    INT 21H
    
    MOV DL,0AH
    MOV AH,2
    INT 21H
    
    POP DX
    
    POP BX
    POP AX
    RET
    ENDP
    
CODES ENDS
    END START