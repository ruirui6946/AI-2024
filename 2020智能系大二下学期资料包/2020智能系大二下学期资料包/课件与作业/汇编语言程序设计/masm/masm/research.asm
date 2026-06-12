datasg segment
	mess1 db 'Enter keyword:','$'
	mess2 db 'Enter Sentence:','$'
	mess3 db 'Match at location:','$'
	mess4 db ' H of the sentence.','$'
	mess5 db 'No match!','$'

stoknin1 label byte
	max1 db 10
	act1 db ?
	stokn1 db 10 dup(?)
stoknin2 label byte
	max2 db 50
	act2 db ?
	stokn2 db 50 dup(?)

datasg ends

codesg segment
	assume cs:codesg,ds:datasg,es:datasg

main proc far

start:
	push ds
	sub ax,ax
	push ax
	mov ax,datasg
	mov ds,ax
	mov es,ax

	lea dx,mess1
	mov ah,09
	int 21h
	lea dx,stoknin1
	mov ah,0ah
	int 21h
	cmp act1,0
	je exit
sentence:
	call crlf
	lea dx,mess2
	mov ah,09
	int 21h
	lea dx,stoknin2
	mov ah,0ah
	int 21h
	cmp act2,0
	je nomatch
	mov al,act1
	cbw
	mov cx,ax
	push cx
	mov al,act2
	sub al,act1
	js nomatch
	mov di,0
	mov si,0
	lea bx,stokn2
a20:
	mov ah,[bx+di]
	cmp ah,stokn1[si]
	jne a30
	inc si
	inc di
	dec cx
	cmp cx,0
	je match
	jmp a20
a30:
	inc bx
	dec al
	cmp al,0
	je nomatch
	mov si,0
	mov di,0
	pop cx
	push cx
	jmp a20
exit:
	ret
nomatch:
	call crlf
	lea dx,mess4
	mov ah,09
	int 21h
	jmp sentence
match:
	call crlf
	lea dx,mess3
	mov ah,09	
	int 21h
	sub bx,offset stokn2
	inc bx
	call trans
	lea dx,mess4
	mov ah,09
	int 21h
	jmp sentence

crlf proc near 			 ;回车，换行
	mov dl,0dh
	mov ah,2
	int 21h
	mov dl,0ah
	mov ah,2
	int 21h
	ret
crlf   endp

trans proc near			 ;转换为16进制，参考书上例6.3
	mov ch,4 		 ;number of digits
rotate: mov cl,4  	         ;set count to 4bits
	rol bx,cl		 ;left digit to right
	mov al,bl		 ;mov to al
	and al,0fh		 ;mask off left digit
	add al,30h		 ;convert hex to ASCII
	cmp al,3ah		 ;is it>9?
	jl printit		 ;jump if digit=0 to 9
	add al,7h		 ;digit is A to F
printit:
	mov dl,al 		 ;put ASCII char in DL
	mov ah,2		 ;Display Output funct
	int 21h			 ;call DOS
	dec ch			 ;done 4 digits?
	jnz rotate		 ;not yet
	ret			 ;return from trans
trans endp			

main endp
codesg ends

	end start