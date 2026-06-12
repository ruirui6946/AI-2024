; multi-segment executable file template.

data segment
    ; add your data here!
    mess1 db "wrong input",13,10,'$'
    num label byte
         max db 3
         act db 0
         stokn db 3 dup(0)
    stoktab db '1','JAN'             ;月份表
            db '2','FEB'
            db '3','MAR'
            db '4','APR'
            db '5','MAY'
            db '6','JUN'
            db '7','JUL'
            db '8','AGU'
            db '9','SEP'
            db '10','OCT'
            db '11','NOV'
            db '12','DEC' 
     mess2 db 3 dup(20h),13,10,'$'
ends
code segment
start:
; set segment registers:
    mov ax, data
    mov ds, ax
    mov es, ax

   lea dx, num     ;存储输入
   mov ah, 0ah
   int 21h    
   mov al,stokn    ;将输入结果放在ax中
   mov ah,(stokn+1)
    
   mov cx, 9 
   lea si,stoktab  ;因为有可能输入一位或者两位，我们选择分类讨论，先假定输入为1为进行预处理
      
   mov dl,act      ;验证输入为一位还是两位
   cmp dl,2
   
   js loop1        ;如果是一位的话，则进入1号循环比较
   mov cx, 3
   add si,36       ;如果是两位的话，需要的预处理
;loop2 和 later2都是输入为两位的处理
loop2:
    cmp cx,0
    jz wrong       ;进入wrong代表输入错误，例如1-12的其他东西
    cmp ax,word ptr [si]   
    jnz later2     ;并不相同，则需要进行下一次循环的准备
    add si,2       ;相同的话，将si指针指向字符串
    jmp prin

later2:
    dec cx
    add si,5
    jmp loop2  
;loop1 和later1 都是输入为一位的处理
loop1:
    cmp cx,0
    jz wrong
    cmp al, byte ptr [si]
    jnz later1
    add si,1
    jmp prin
later1:
    dec cx
    add si, 4
    jmp loop1
;找到了对应的数字，则输出相应的月份
prin:
   mov cx,3
   lea di,mess2
   rep movsb
   
   lea dx,mess2
   mov ah,09h
   int 21h
   
   jmp exit
;未找到对应的数字，则报错
wrong:
   lea dx,mess1
   mov ah,9h
   int 21h
   jmp exit
   
exit:
  
ends

end start ; set entry point and stop the assembler.
