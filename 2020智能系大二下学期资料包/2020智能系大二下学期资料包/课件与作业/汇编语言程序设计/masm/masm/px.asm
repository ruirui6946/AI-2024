datarea segment
  grade  dw  88,75,95,63,98,78,87,73,90,60
  rank   dw  10 dup(?)
  mess3  db  'Rank: $'
  count  dw  ?
datarea ends
prognam  segment
;*********************************************
main    proc  far
        assume cs:prognam,ds:datarea
start:

        push  ds
        sub   ax,ax
        push  ax

        mov   ax,datarea
        mov   ds,ax

        mov   di,10
        mov   bx,0
loop1:
        mov   ax,grade[bx]
        mov   dx,0
        mov   cx,10
        lea   si,grade
next1:
        cmp   ax,[si]
        jg    no_count
        inc   dx
no_count:
        add   si,2
        loop  next1
        sub   bp,bp
        mov   bp,dx
        mov   rank[bp],ax
        add   bx,2
        dec   di
        jne   loop1
        mov   count,10
        call  output
        ret
        

main    endp
;-------------------------------------------------
output  proc  near
        lea   dx,mess3
        mov   ah,0-9
        int   21h
;
        mov   si,0
        mov   di,count
       
next2:
        mov   bx,rank[si]
        call  binidec
        mov   dl,','
        mov   ah,02
        int   21h
        add   si,2
        dec   di
        jnz   next2
        call  crlf
        ret

output  endp
;
binidec proc  near

        push  bx
        push  cx
        push  si
        push  di
        mov   cx,10d
        call  dec_div
        mov   cx,1d
        call  dec_div
        pop   di
        pop   si
        pop   cx
        pop   bx
        ret

binidec   endp
;
dec_div proc  near
        mov  ax,bx
        mov  dx,0
        div  cx
        mov  bx,dx
        mov  dl,al

        add  dl,03h
        mov  ah,20h
        int  21h
        ret
dec_div endp

crlf    proc  near
        mov   al,0ah
        mov   ah,02h
        int   21h

        mov   dl,0dh
        mov   ah,02h
        int   21h
        ret
crlf    endp

prognam ends
;*************************************************
        end   start

