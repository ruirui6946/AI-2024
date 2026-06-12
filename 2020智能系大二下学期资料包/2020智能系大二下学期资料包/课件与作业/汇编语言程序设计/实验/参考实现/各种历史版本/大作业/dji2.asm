DATAS SEGMENT
    NODENAME DB 6 DUP(00H)
    LEN EQU $-NODENAME
    LEN1 DB 00H
    LEND DD 00H,00H
    
    STARTNODE DB 00H
    STARTNUM DB 00H

    ENDNODE DB 00H
    ENDNUM DB 00H
    
    CHECK DB LEN DUP(00H)
    
    PATH DB 36 DUP(00H)
         
    DIS DB LEN DUP(0FFH)
    FATHER DB LEN DUP(0FFH)
    
    OUTPATH DB LEN DUP(0FFH)
    
    CANNOT DB 'cannot find a way',0DH,0AH,'$'
    MAKEIT DB 'find a way:',0DH,0AH,'$'
    COST DB 0DH,0AH,'cost:',0DH,0AH,'$'
    INNUM DB 'please give the number of nodes',0DH,0AH,'$'
    INNODE DB 'please give the name of nodes, each node need only one acsii',0DH,0AH,'$'
    INSTART DB 'please give me the name of start node name',0DH,0AH,'$'
    INEND DB 'please give me the name of end node name',0DH,0AH,'$'
    INCOST1 DB 'please give the cost from $'
    INCOST2 DB ' to $'
    INCOST3 DB ':',0DH,0AH,'$'
DATAS ENDS

STACKS SEGMENT
    ;此处输入堆栈段代码
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    
    
     ;MOV AL,LEN
;输入层
     ;输入节点数
     LEA DX,INNUM
     MOV AH,9
     INT 21H
     CALL CIN
     MOV BYTE PTR LEN1,DL
     MOV BYTE PTR LEND,DL
     
     ;输入节点
     MOV SI,0
     LEA DX,INNODE
     MOV AH,9
     INT 21H
NODE_IN:
     CMP SI,WORD PTR LEND
     JE NODE_IN_END
     
     MOV AH,1
     INT 21H
     MOV BYTE PTR NODENAME[SI],AL
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
     
     ;输入cost
     MOV BX,1
     MOV CX,0
     
COST_AGAIN1:
     CMP BL,BYTE PTR LEN1
     JE NEXT_COST1
     MOV CL,0
COST_AGAIN2:
     CMP CL,BL
     JE NEXT_COST2
     MOV DI,CX
     MOV SI,BX
     
     LEA DX,INCOST1
     MOV AH,9
     INT 21H
     
     MOV DL,BYTE PTR NODENAME[DI]
     MOV AH,2
     INT 21H
     
     LEA DX,INCOST2
     MOV AH,9
     INT 21H
     
     MOV DL,BYTE PTR NODENAME[SI]
     MOV AH,2
     INT 21H
     
     LEA DX,INCOST3
     MOV AH,9
     INT 21H
     
     CALL CIN
     
     MOV DH,BYTE PTR LEN1
     MOV AX,DI
     MUL DH
     PUSH SI
     ADD SI,AX
     MOV BYTE PTR PATH[SI],DL
     POP SI
      
     MOV AX,SI
     MUL DH
     ADD DI,AX
     MOV BYTE PTR PATH[DI],DL
     
     INC CL
     JMP COST_AGAIN2
     
NEXT_COST2:
     INC BL
     JMP COST_AGAIN1

NEXT_COST1:
    
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
    MOV BYTE PTR STARTNUM,BL
    
    ;得到目标节点标号
    MOV CL,BYTE PTR ENDNODE
    MOV BX,0
FIND_E:
    CMP CL,BYTE PTR NODENAME[BX]
    JE SET_E
    INC BX
    JMP FIND_E

SET_E:
    MOV BYTE PTR ENDNUM,BL
    
    ;寻找路径
    MOV CH,BYTE PTR STARTNUM
    MOV BL,BYTE PTR STARTNUM
    MOV BH,0
    MOV BYTE PTR CHECK[BX],01H;初始节点已经被走过
    MOV BYTE PTR DIS[BX],00H;自己到自己距离为零
    MOV BYTE PTR FATHER[BX],CH;自己是自己的父亲，结束标志
    
AGAIN:
    MOV BL,CH
    MOV BH,00H
    CMP BYTE PTR DIS[BX],0FFH
    JE FALSE;最短路径为无穷大
    
    CMP CH,BYTE PTR ENDNUM
    JE TOOUT;
    
    MOV DL,BYTE PTR DIS[BX];把从初始节点到当前节点的长度读入DL
    
    MOV DI,0;初始化变址
    
    MOV AL,BYTE PTR LEN1
    MUL CH
    MOV BX,AX;得到所需的一组路径
SET_DIS:
    CMP DI,WORD PTR LEND
    JE FIND_MIN;
    CMP BYTE PTR CHECK[DI],01H
    JE NEXT
    
    CMP BYTE PTR PATH[BX][DI],0FFH
    JE NEXT;不连通，直接跳过
    
    MOV AL,BYTE PTR DIS[DI];之前找到的最短路径长度
    PUSH DX
    ADD DL,BYTE PTR PATH[BX][DI];从当前节点走的最短路径
    CMP AL,DL
    JBE NEXT_POP
    MOV BYTE PTR DIS[DI],DL
    MOV BYTE PTR FATHER[DI],CH
    
NEXT_POP:
    POP DX
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
    MOV CL,BYTE PTR STARTNUM
    
    MOV BL,BYTE PTR ENDNUM
    MOV DI,0    
OUT_AGAIN:
    CMP BL,CL
    JE PRINT_OUT
    MOV BYTE PTR OUTPATH[DI],BL
    MOV BL,BYTE PTR FATHER[BX]
    INC DI
    JMP OUT_AGAIN
    
PRINT_OUT:
    MOV DL,BYTE PTR STARTNODE
    MOV AH,2
    INT 21H
    
    DEC DI
PRINT_OUT2:
    CMP DI,0
    JL PRINT_COST
    MOV DL,'-'
    INT 21H
    MOV DL,'>'
    INT 21H
    
    MOV BH,0
    MOV BL,BYTE PTR OUTPATH[DI]
    MOV DL,BYTE PTR NODENAME[BX]
    INT 21H
    DEC DI
    JMP PRINT_OUT2
    
    ;JMP ENDD

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


FALSE:
    LEA DX,CANNOT
    MOV AH,9
    INT 21H
ENDD:  
    MOV AH,4CH
    INT 21H
    
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
    PUSH DX
    CMP AL,0
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
    
    MOV DL,' '
    MOV AH,2
    INT 21H
    
    POP AX
    POP DX
    RET
ENDP

CIN PROC
    PUSH AX
    PUSH BX
    
    MOV AL,0
    MOV DH,10
    MOV DL,0

IN_AGAIN:
    
    MOV AH,1
    INT 21H
    SUB AL,30H
    
    CMP AL,0
    JB IN_END
    CMP AL,9
    JA IN_END
    
    MOV BL,AL
    XCHG AL,DL
    MUL DH
    ADD AL,DL
    MOV DL,AL
    MOV AL,BL
    JMP IN_AGAIN
    
IN_END:
    POP BX
    POP AX
    RET
    ENDP
    
CODES ENDS
    END START