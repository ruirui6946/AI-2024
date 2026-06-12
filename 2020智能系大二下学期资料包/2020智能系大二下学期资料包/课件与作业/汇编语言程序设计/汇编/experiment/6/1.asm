; multi-segment executable file template.

data segment
    ; add your data here!
     data1 db 10 dup(6,4,8,2,1,3,5,7,9,10)
     mess1 db 'the ordered line is:',13,10,'$'

ends

 

code segment
start:
; set segment registers:
    mov ax, data
    mov ds, ax
    mov es, ax

    mov cx,10
    mov di,cx    ;使用di来临时存储cx的下一轮初始值，同时控制外循环是否进行
    
    
loop1:
    mov cx,di
    dec cx
    cmp cx,0
    jz exit      ;倘若此时cx的值为零，也就是外循环次数di的值为零，则外循环已经结束了9次循环
    mov di,cx    
    mov bx,0     ;bx存储的是相对于首地址的位移量
loop3:
    cmp cx,0     
    jz next      ;倘若cx为零表示此次内循环结束
    mov al,data1[bx];比较相邻的两个数，保证顺序排列 
    cmp al,data1[bx+1]
    js continue
    xchg al,data1[bx+1]
    mov data1[bx],al
    
continue:
    inc bx
    dec cx
    jmp loop3     ;继续内循环 

    
next:
    jmp loop1     ;继续外循环

exit:
    lea dx,mess1  ;输出排好序的数组
    mov ah,09;
    int 21h
    mov cx,10
    mov bx,0
loop2:
    mov dl,data1[bx]
    add dl,48
    mov ah,02h 
    int 21h
    inc bx 
    loop loop2
    
       
ends

end start ; set entry point and stop the assembler.
