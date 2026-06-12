; multi-segment executable file template.

data segment
     num label byte
         max db 81
         act db 0
         stokn db 81 dup(0) 
     letter db 0
     digit  db 0
     other  db 0
ends

 

code segment
start:
; set segment registers:
    mov ax, data
    mov ds, ax
    mov es, ax
    
    lea dx,num
    mov ah,0ah
    int 21h
    
    mov cl,act
    lea si,stokn       ;预处理

loop1:    
    cmp cx,0           ;cx = 0意味着处理完了所有的输入 
    jz exit
    cmp [si],48        ;根据ascii值来进行分流
    js other1
    cmp [si],58
    js digit1
    cmp [si],65
    js other1
    cmp [si],91
    js letter1
    cmp [si],97
    js other1
    cmp [si],123
    js letter1
    jmp other1
;检测到是数字             
digit1:
    inc digit 
    jmp later
;检测到是字母
letter1:
    inc letter
    jmp later
;检测到是其他
other1:
    inc other
    jmp later
later:    
    inc si
    dec cx
    jmp loop1

exit:          
ends

end start ; set entry point and stop the assembler.
