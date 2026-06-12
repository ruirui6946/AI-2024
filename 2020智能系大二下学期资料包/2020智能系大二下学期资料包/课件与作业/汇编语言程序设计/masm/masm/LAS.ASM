datarea segment
        grade   dw      88,75,95,63,98,78,87,73,90,60
	rank	dw	10 dup(?)
        result  dw      10 dup(?)
datarea ends

prognam segment
main	proc	far
	assume cs:prognam,ds:datarea
start:
	push	ds
	sub	ax,ax
	push	ax

	mov	ax,datarea
	mov	ds,ax
	
	mov	di,10
	mov	bx,0
	
loop:
	mov	ax,grade[bx]
	mov	dx,0
	mov	cx,10
	lea	si,grade

next:
	cmp	ax,[si]
	jg	no_count
	inc	dx

no_count:
	add	si,2
	loop	next
	mov	rank[bx],dx
	add	bx,2
	dec	di
        jne     loop
memset:
        mov     cx,10
        mov     ax,0
        lea     si,rank
        mov     di,ax
        mov     bx,ax
        mov     dx,ax
ans:
        mov     bx,[si]
        add     si,2
        mov     dx,grade[di]
        add     di,2
        add     bx,bx
        mov     result[bx],dx
        loop    ans
        mov     cx,11
        mov     ax,0
        mov     bx,ax
        mov     si,ax
        mov     di,ax
print:
        mov     ax,result[bx]
        mov     dl,0ah
        div     dl
        mov     dh,ah
phigh:
        add     al,30h
        mov     ah,02h
	mov     dl,al
        int     21h
plow:
        add     dh,30h
        mov     ah,02h
	mov     dl,dh
        int     21h
	mov	dl,32
	mov     ah,02h
	int 	21h
        add     bx,2
        loop    print
        ret
main	endp

prognam ends
	end	start
