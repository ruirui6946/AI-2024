datarea segment
    grade   dw      88,75,95,63,98,78,87,73,90,60
	rank	dw	10 dup(?)
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

loop1:
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
        jne     loop1

     mov di,10
     mov bx,1
first:
     mov cx,10
     mov si , 0
next1:
     cmp bx,rank[si]
     jne last
     lea dx,grade[si]
     mov ah,09
     int 21h
last:
      add si,2
      loop next1
      add bx,1
      dec di
      jne first
	ret

main	endp

prognam ends
	end	start
