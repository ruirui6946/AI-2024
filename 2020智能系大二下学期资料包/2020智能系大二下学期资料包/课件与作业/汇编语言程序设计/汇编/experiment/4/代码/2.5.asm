; multi-segment executable file template.

data segment
    ; add your data here!
    a db 4
    c db -5
    b db 2
    k db 4
    tag db 0
    temp dw 0 
    sign db -1
ends

stack segment
    dw   128  dup(0)
ends

code segment
    start:

    mov ax, data
    mov ds, ax
    mov es, ax 
    
    cmp a,0     ;讨论a的取值 ,倘若a是0，讨论b是否为0，若b也为0,则方程无解，若a非零，则讨论a,c 是否异号
    js am       ; a为负数
    cmp a,1 
    jns ap      ;a为正数
    cmp b,0     
    jz exit     ;a,b都是零
    jmp great
am:
    cmp c,0     ; 讨论a,c是否异号
    jns great   ;异号一定有解，同号则去计算delta
    mov al,a    ;倘若a,c都是负数，则置为正数，方便之后计算，避免溢出
    imul sign
    mov a,al 
    mov al,c
    imul sign
    mov c,al
    jmp caldelta
    
ap:
    cmp c,1      ; 讨论a,c是否异号
    js great 
    jmp caldelta
     
     
caldelta:
    mov al,b
    imul b
    mov temp,ax  ; temp中存储b^2
    mov al,a
    mul c
    mul k
    cmp temp,ax
    jns great    ;b^2大于等于4ac则有根
    jmp exit 
    
great:           ;意味有根
   mov tag,1 
   jmp exit
   
exit:
   mov dl,tag   ;输出结果，1，代表有根；0，代表无根
   add dl,48
   mov ah,02h
   int 21h 
    
ends

end start ; set entry point and stop the assembler.
