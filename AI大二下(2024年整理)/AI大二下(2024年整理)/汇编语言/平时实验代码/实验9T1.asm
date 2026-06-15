data segment 
    input db 50,?,50 dup(?)
    output db 50,?,50 dup(0)
    crlf db 0ah,0dh,'$'
data ends
code segment
assume ds:data,cs:code,es:data
start:
    mov ax,data
    mov ds,ax
    mov es,ax
    mov dx,offset input
    mov ah,0ah
    int 21h
    lea dx,crlf 
    mov ah,9h
    int 21h

    mov cl,[input+1]
    mov ch,0
    mov ax,cx
    mov [output+1],cl
    add ax,2
    mov si,ax
    mov [output+si],'$'
    lea si,[input+si-1]
    lea di,[output+2]
    
    std
loop_start:
    lodsb
    mov [di],al
    inc di
    loop loop_start

    lea dx,output+2
    mov ah,9h
    int 21h
    mov ah,4ch 
    int 21h
code ends
end start