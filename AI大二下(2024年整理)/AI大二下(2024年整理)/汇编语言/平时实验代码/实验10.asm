public msg
data segment
    is_prime db 'Is a prime$'  
    no_prime db 'Not a prime$'  
    msg db 'please put in your plus integer',0ah,0dh,'$'
    number dw 10,?,50 dup(?)
data ends  

code segment  
    assume cs:code,ds:data  
start:  
    mov ax,data  
    mov ds,ax  

    call input;输入存储在bx中
    call crlf
    call judge;判断
    call crlf  
    mov ah,4ch  
    int 21h  

input proc near
    lea dx,msg
    mov ah,9
    int 21h 

    lea dx,number
    mov ah,0ah
    int 21h
    mov cx,[number+1];字符个数
    mov ch,0
    mov di,2;当前位位置
    mov bx,0;储存结果
loop_start:
    mov si,[number+di]
    and si,0fh;得到数字
    mov ax,bx
    mov dx,10;权重
    mul dx
    mov bx,ax;放回bx
    add bx,si
    add di,1
    loop loop_start
    ret
input endp

judge proc near  
    mov cx,bx
    sub cx,2
    cmp bx,1
    je print2;1肯定不是啊
    cmp bx,2
    je print1;2肯定是啊
    mov dl,1  
judge_loop:  
    mov ax,bx  
    inc dl;从2到n-1，看看是不是这个数的因数,一共进行n-2次
    div dl
    cmp ah,0;看看余数
    jz print2
    loop judge_loop 
    jmp print1  
print1:  
    mov ah,09h  
    lea dx,is_prime
    int 21h  
    ret  
print2:  
    mov ah,09h  
    lea dx,no_prime
    int 21h       
ret  
judge endp  

crlf proc near  
    mov dl,0dh  
    mov ah,02h  
    int 21h  
    mov dl,0ah  
    mov ah,02h  
    int 21h    
ret  
crlf endp  

code ends  
    end start  