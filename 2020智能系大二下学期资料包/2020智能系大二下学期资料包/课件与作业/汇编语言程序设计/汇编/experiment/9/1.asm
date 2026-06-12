; multi-segment executable file template.

data segment
    ; add your data here!
    year dw 0  ;存储年份数值
    month dw 0
    day dw 0
    days dw 0  ;存储总计天数
    k dw 1000  ;用于协助字符数值转换的参数
    sign dw 10 ;用于协助字符数值转换的参数 
    month_table db 12 dup(31,28,31,30,31,30,31,31,30,31,30,31);月份表
    mess1 db 5 dup(0)  
ends

stack segment
    dw   128  dup(0)
ends

code segment
start:
; set segment registers:
    mov ax, data
    mov ds, ax
    mov es, ax

main proc near  
    call cin         ;负责从键盘读取输入输入格式为YYYY/MM/DD，并以数值形式存储在变量中
    call judge_leap  ;负责判断是否闰年，并且修改月份表
    call conver      ;将月份和日期信息转换为天数存储在days
    call prin        ;打印出days的数值
    mov ah, 1
    int 21h
    
    mov ax, 4c00h ; exit to operating system.
    int 21h 
main endp

cin proc near
    push cx
    push ax 
    mov cx, 10;
    mov ax, 0;
    mov dx, 0 
cinloop1:              ;由于格式固定，我们可以根据输入的为第几个数字从而转换为不同的信息
    cmp cx,0
    jz cinexit
    mov ah,01
    int 21h 
    mov ah,0
    cmp cx,7
    jns cinyear        ;前四个信息为年份信息
    cmp cx,6
    jz cinconym        ;第五个为/略去
    cmp cx,4
    jns cinmonth       ;第六七位月份信息
    cmp cx,3
    jns cinconym       ;第八个位/略去
    cmp cx,1           ;第九十个位日期信息
    jns cinday
cinyear:               ;根据不同的位置乘以不同的权加入到year中
    sub  al,48
    imul k                               
    add year,ax
    jmp cincontinue
cinmonth:
    sub al,48
    imul k
    add month,ax
    jmp cincontinue  
cinday:
    sub al,48
    imul k
    add day,ax
    jmp cincontinue 
cinconym:               ;输入为\时，调整权的信息
    mov k,100 
    jmp cincontinue    
cincontinue:
    dec cx
    mov ax,k            ;每一轮循环结束，修改权
    idiv sign
    mov k,ax 
    mov dx,0            ;清除余数
    jmp cinloop1
    
cinexit:
    pop ax
    pop cx
    ret
cin endp

judge_leap proc endp
    push ax
    push dx
    
    mov ax, year       ;即一般判断闰年的方式
    mov dx,0
    mov k,4
    idiv k
    cmp dx,0
    jnz judgexit       ;不能被4整除的年份一定不是闰年
    mov ax,year
    mov k,100
    idiv k
    cmp dx,0
    jnz leap           ;能被4整除切且不能被100整除为闰年
    mov ax,year
    mov k,400           
    idiv k
    cmp dx,0
    jnz judgexit       ;能被100整除切且不能被400整除为平年
    jmp leap           ;能被400整除为闰年
leap:
    inc month_table[1]  ;
judgexit:
    pop dx
    pop ax
    ret
judge_leap endp
  
conver proc near
    push di
    push ax 
    push bx
    mov bx,0           ;初始化
    dec month
    mov di,0 
    mov ax,0           ;ax暂存
converloop:            ;通过遍访month_table来叠加日子
    cmp di,month       
    jz converexit 
    mov bl,month_table[di];叠加本月日子数 
    add ax,bx
    inc di
    jmp converloop
converexit:
    add ax,day 
    mov days,ax 
    pop bx
    pop ax
    pop dx
    ret 
conver endp 

prin proc near
    push ax
    push cx
    push dx
    push di 
    push dx
    
    mov dl,10
    mov ah,02
    int 21h
    mov dl,13
    mov ah,02
    int 21h
    mov k,10 
    mov ax,days
    mov cx,0        ;cx 用来记录转换后的十进制的位数，辅助输出
    lea di,mess1 
    mov dx, 0
;转换十六进制为十进制，并以字符形式存储在mess1中    
prinloop1:
    cmp ax,0
    jz prinprin         ;脱离循环的为一条件
    div k           ;
    add dl,48       ;dl中存储的是余数，将数字形式转换为字符形式存储到首地址为mees1的字符串中
    mov es:[di], dl
    mov dl,0
    inc di          ;字符串指针移向下一个空位
    inc cx          ;代表十进制位数多1
    jmp prinloop1
;倒序逐个输出mess1中的内容   
prinprin:
   dec di           ;到达prin模块，此时di实际指向在有意义的字符的下一位，所以要减一
prinloop2:
   cmp cx,0         ;cx是之前创建的计数器
   jz prinexit
   mov dl, es:[di]  ;逐位输出
   mov ah,02h
   int 21h
   dec cx           ;
   dec di
   jmp prinloop2 
   
;ax=0 的情况单独处理   
prinexit:
    pop dx
    pop di
    pop dx
    pop cx
    pop ax
    ret 
prin endp
ends

end start ; set entry point and stop the assembler.
