data    segment
count   dw      0
tel_tab db      50 dup (20 dup (?) , 8 dup (?))
msg1    db      'Input name:' , '$'
msg2    db      'Input a telephone number:' , '$'
msg3    db      'Do you want a telephone number:(Y/N)' , '$'
msg4    db      'name?' , '$'
inname  label   byte
inmax   db      21
inact   db      ?
inn     db      21 dup (?)
ipphone label   byte
ipmax   db      9
ipact   db      ?
ipp     db      9 dup (?)
cbuf    db      28 dup (?)
data    ends
code    segment
        assume  cs:code, ds:data , es:data
main    proc    far
        push    ds
        sub     ax , ax
        push    ax
        mov     ax , data
        mov     ds , ax
        mov     es , ax
        lea     bx , tel_tab
        mov     di , bx
new:    lea     dx , msg1
        mov     ah , 09h
        int     21h
        call    input_name
        call    crlf
        mov     al , inact
        cmp     al , 0
        jz      sort
        call    stor_name
        inc     count
        lea     dx , msg2
        mov     ah , 09h
        int     21h
        call    inphone
        call    crlf
        cmp     count , 50
        jl     new
sort:
        call    name_sort
ask_again:
        lea     dx , msg3
        mov     ah , 09h
        int     21h
        mov     ah , 01h
        int     21h
        cmp     al , 4eH
        je      return
        call    crlf
        cmp     al , 59H
        jne     ask_again
        call    crlf
        lea     dx , msg4
        mov     ah , 09h
        int     21h
        call    input_name
        call    name_search
        call    printline
        jmp     ask_again
return:
        ret
main    endp
input_name      proc
        push    di
        lea     dx , inname
        mov     ah , 0ah
        int     21h
        lea     di , inn
        mov     al , inact
        cbw
        add     di , ax
        mov     cx , 21
        sub     cx , ax
        mov     al , 20h
        cld
        rep     stosb
        pop     di
        ret
input_name      endp
stor_name       proc
        lea     si , inn
        mov     cx , 20
        cld
        rep     movsb
        ret
stor_name       endp
crlf    proc
        mov     dl , 0dh
        mov     ah , 02h
        int     21h
        mov     dl , 0ah
        mov     ah , 02h
        int     21h
        ret
crlf    endp
inphone proc
        push    di
        lea     dx , ipphone
        mov     ah , 0ah
        int     21h
        lea     di , ipp
        mov     al , ipact
        cbw
        add     di , ax
        mov     cx , 9
        sub     cx , ax
        mov     al , 20h
        cld
        rep     stosb
        pop     di

        lea     si , ipp
        mov     cx , 8
        cld
        rep     movsb
        ret
inphone endp
name_sort       proc
        mov     bx , count
        dec     bx
s3:
        lea     si , tel_tab
        mov     di , si
        add     di , 28
        mov     cx , bx
s2:
        push    si
        push    di
        push    cx
        mov     cx , 28
        cld
        repe    cmpsb
        pop     cx
        pop     di
        pop     si
        jl      s1
        call    exchang
s1:
        add     si , 28
        add     di , 28
        loop    s2
        dec     bx
        jnz     s3
        ret
name_sort       endp
name_search     proc
        ret
name_search     endp
printline       proc
        ret
printline       endp
exchang proc
        push    cx
        push    si
        push    di
        push    ax
        push    bx
        mov     ax , si
        mov     bx , di
        lea     di , cbuf
        mov     cx , 28
        cld
        rep     movsb
        mov     di , ax
        mov     si , bx
        mov     cx , 28
        cld
        rep     movsb
        lea     si , cbuf
        mov     bx , di
        mov     cx , 28
        cld
        rep     movsb
        pop     bx
        pop     ax
        pop     di
        pop     si
        pop     cx
        ret
exchang endp
code    ends
        end     main
