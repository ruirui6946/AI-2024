; multi-segment executable file template.

data segment
    ; add your data here!
    list db 160 dup(0)  ;负责存出人名，每个人名预留15字节，人名以回车（13）结束
    temp db 16 dup(0)   ;临时缓冲区，在人名排序时，作为一个中间量，帮助传递
    temp1 dw 0          ;临时存储区，用于存储地址，在调用子程序前，存入，子程序结束读出，不用堆栈是因为，子程序需要用到地址数据
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

main proc near  ;主程序
    call cin    ;负责从键盘读取人名
    call order  ;负责排序
    call prin   ;负责输出
    ; wait for any key....    
    mov ah, 1
    int 21h
    
    mov ax, 4c00h ; exit to operating system.
    int 21h   
main endp 
         
cin proc near   ;读取采用01号中断和循环叠加的情况（认为空格也作为人名一部分参与排序）
    mov cx,0
    mov di,0
loopcin1:
    add cx,16   ; cx中存储的是，下一次人名的起始地址
loopcin2:
    mov ah,01
    int 21h 
    mov list[di],al
    cmp al,13  ;每当输入了一个回车，就认为这个人名已经输完
    jz exitcin
    inc di
    jmp loopcin2
exitcin:      ; cx中存储的是，下一次
    cmp cx,160  ;此时意味已经读完了十个人名
    jz finsh
    mov di,cx   ;没有读完的话，di指针指向下一个人名开始的相对地址
    jmp loopcin1
finsh:
    ret    
cin endp

order proc near
    mov cx,10
    mov di,cx    ;使用di来临时存储cx的下一轮初始值+1，同时控制外循环是否进行      
orloop1:
    mov cx,di
    dec cx
    cmp cx,0
    jz orderexit ;倘若此时cx的值为零，也就是外循环次数di的值为1，则外循环已经结束了9次循环
    mov di,cx    ;使用di来临时存储cx的下一轮初始值+1，同时控制外循环是否进行 
    mov bx,0     ;bx存储的是相对于首地址的位移量，每一次外循环都是从数组头开始
orloop3:         ;内循环的作用是，将最大的数压到最底下，
    cmp cx,0     
    jz ornext      ;倘若cx为零表示此次内循环结束
    mov temp1,bx   ; 临时存储，避免因为子程序的改动，影响这里的循环
    call compare   ;调用子程序，负责比较这个时候，bx的指向和bx+16指向的人名的大小关系，并用ax返回比较结果
    mov bx,temp1
    cmp ax,1
    jz orcontinue  ;说明较小的数字在较前的位置，所以无须交换
    mov temp1,bx
    call exchange  ;说明较大的数字在较前的位置，前后交换，list[bx]和list[bx+16]交换位置
    mov bx,temp1
orcontinue:
    add bx,16 
    dec cx
    jmp orloop3     ;继续内循环
ornext:
    jmp orloop1     ;继续外循环
orderexit:
    ret
order endp
 
 
compare proc near 
    push cx
    mov ax,0  
    mov cx,16
comloop:            ;比较这两个人名 
       cmp cx,0     ;倘若cx为零，说明两个人名完全相同
       jz comno;
       mov al,list[bx]   ;逐个比较人名中的各个字符
       cmp al, list[bx+16]
       jz comcontin  ;如果相等，则继续比较
       js  comno     ;如果地址靠前的数字较小，则ax标记1
       jmp comyes    ;如果地址靠后的数字较小，则ax标记0
comcontin:
       inc bx
       dec cx
       jmp comloop
comno:
    mov ax, 1
    jmp comexit
comyes: 
    mov ax,0
    jmp comexit
comexit:
    pop cx
    ret
compare endp  

exchange proc near 
    push cx
    push si
    push dx 
    push ax
    
    mov cx,16
    mov ax,bx    ;ax负责临时存储bx的地址，总共有三次交换，而每次交换前，回复bx的初始值
    mov si,0;    ;si是temp的指针
exloop1:         ; list[bx]移入temp
    cmp cx,0
    jz excont1     
    mov dl, list[bx]
    mov temp[si],dl
    inc bx
    inc si
    dec cx
    jmp exloop1
excont1:
    mov cx,16
    mov bx,ax 
exloop2:
    cmp cx,0      ; list[bx+16]移入list[bx]
    jz excont2     
    mov dl, list[bx+16]
    mov list[bx],dl
    inc bx
    dec cx
    jmp exloop2
excont2: 
    mov cx,16
    mov bx,ax  
    mov si,0
exloop3:          ;temp移入list[bx+16]
    cmp cx,0
    jz exexit     
    mov dl, temp[si]
    mov list[bx+16],dl
    inc bx 
    inc si
    dec cx
    jmp exloop3
exexit:
    mov bx,ax 
    
    pop ax  
    pop si
    pop dx
    pop cx
    ret   
exchange endp
  
  
prin proc near      ;输出list中的内容，采用02号中断和循环配合的方法
     push cx
     push di
     push bx
     push dx 
     mov bx, 0       
     mov cx,10      ;总计输出10个人名
     mov di,0  
     add bx,16      ;bx中负责存储，下一个人名的相对地址
            
     mov dl,10      ;先回车换行一下，和输入区分开来
     mov ah,02
     int 21h
     mov dl,13
     mov ah,02
     int 21h 
prinloop:          ;
    cmp cx,0
    jz prinexit
    mov dl,list[di]  ;调用02号中断输出
    cmp dl,13
    jz prinnext     ; 每个人名都是用回车（13）作为结尾
    mov ah,02
    int 21h     
    inc di
    jmp prinloop
prinnext:          ;一个人名输出结束
    mov dl,10      ;回车换行一下
    mov ah,02
    int 21h
    mov dl,13
    mov ah,02
    int 21h 
    dec cx
    mov di,bx      ;di指针指向下一个人名
    add bx,16
    jmp prinloop
prinexit:
    pop si
    pop dx      
    pop bx
    pop dx
    pop cx
    ret
prin endp
       
ends

end start ; set entry point and stop the assembler.
