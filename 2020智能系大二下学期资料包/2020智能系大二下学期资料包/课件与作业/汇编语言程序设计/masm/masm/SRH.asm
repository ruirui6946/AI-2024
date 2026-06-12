datasg segment
mess1   db      'Enter keyword:','$'
mess2   db      'Enter Sentence:','$'
;
stokkey label   byte
    max1 db      30
    act1 db      ?
    key  db      30 dup(?)
;
stoksen label   byte
    max2 db      30
    act2 db      ?
    sen  db      30 dup(?)
;
mess3   db      'No match.',13,10,'$'
mess4   db      13,10,'$'
datasg ends
;
codesg segment
        assume cs:codesg,ds:datasg,es:datasg
;
main proc far
start:
        push    ds
        sub     ax,ax
        push    ax

        mov     ax,datasg
        mov     ds,ax
        mov     es,ax

        lea     dx,mess1
        mov     ah,09
        int     21h
        lea     dx,stokkey
        mov     ah,0ah
        int     21h
        lea     dx,mess4
        mov     ah,09
        int     21h

        lea     dx,mess2
        mov     ah,09
        int     21h
        lea     dx,stoksen
        mov     ah,0ah
        int     21h
        lea     dx,mess4
        mov     ah,09
        int     21h

 judge:
        mov     al,act1
        cmp     al,act2
        ja      Nmatch

initial:
        mov     al,act2
        sub     al,act1
        inc     al
        inc     al
        mov     ch,0
        mov     cl,act1
        lea     bx,sen
        lea     di,sen
        lea     si,key
        dec     di

 loop1:
        dec     al
        jz      Nmatch
        inc     di
        mov     bx,di
        mov     ah,[si]
        cmp     ah,[di]
        jnz     loop1

 loop2:
        repz    cmpsb
        cmp     cx,0
        jz      Match
        inc     bx
        lea     si,mess1
        mov     di,bx
        jmp     loop1

 Nmatch:
        lea     dx,mess3
        mov     ah,09
        int     21h
        jmp     start

  Match:
        mov     ah,act2
        sub     ah,act1
        inc     ah
        sub     ah,al
        mov     al,ah
        mov     ah,09
        int     21
        jmp     start

        main    endp
        codesg  ends
        end     start
