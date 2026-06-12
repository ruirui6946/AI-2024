;********************************************************************
datasg segment
    mess1   db  'Enter Keyword:','$'
    mess2   db  'Enter Sentence:','$'
    mess3   db  'Match at location:','$'
    mess4   db  ' H of the Sentence',13,10,'$'
    mess5   db  'No match.',0dh,0ah,'$'
    crlf    db     0dh,0ah,'$'

    stokin1  label  byte
    max1      db     10
    act1      db     ?
    stokn1    db     10 dup(?)

    stokin2  label  byte
    max2      db     50
    act2      db     ?
    stokn2    db     50 dup(?)

datasg ends
;********************************************************************
codesg segment
    assume cs:codesg,ds:datasg,es:datasg
main proc far
start:
    push    ds
    sub     ax,ax
    push    ax
    mov     ax,datasg
    mov     ds,ax
    mov     es,ax
;MAIN PART OF PRIOGRAM
    lea dx,mess1        ;显示信息，请输入关键字keyword
    mov ah,09
    int 21h
    call input_keyword
    call stor_keyword

    ;lea dx,crlf        ;换行符
    ;mov ah,09
    ;int 21h

    lea dx,mess2        ;显示信息，请输入字符串sentence
    mov ah,09
    int 21h
    call input_sentence
    call stor_sentence


;ask_again:
    call keyword_search
;    lea dx,mess1        ;显示信息，请输入关键字keyword
;    mov ah,09
;    int 21h
;    call input_keyword
;    call stor_keyword
;    jmp askagain

exit:
    ret

main endp
;--------------------------------------------------------------------
;--------------------------------------------------------------------
;--------------------------------------------------------------------
input_keyword  proc  near
     mov ah,0ah
     lea dx,stokin1
     int 21h
     mov ah,09
     lea dx,crlf
     int 21h
     sub bh,bh
     mov bl,act1
     mov cx,11
     sub cx,bx            ;dst<-dst-src,cx为计数功能寄存器
b10:
     mov stokn1[bx],' '   ;补全空格
     inc bx
     loop b10

    ret
input_keyword endp
;--------------------------------------------------------------------
stor_keyword     proc   near
      lea  si,stokn1
      mov  cx,10
      rep  movsb
      ret
stor_keyword  endp
;--------------------------------------------------------------------
;--------------------------------------------------------------------
input_sentence  proc  near
     mov ah,0ah
     lea dx,stokin2
     int 21h
     mov ah,09
     lea dx,crlf
     int 21h
     sub bh,bh
     mov bl,act2
     mov cx,51
     sub cx,bx            ;dst<-dst-src,cx为计数功能寄存器
c10:
     mov stokn2[bx],' '   ;补全空格
     inc bx
     loop c10

    ret
input_sentence endp
;--------------------------------------------------------------------
stor_sentence     proc   near
      lea  si,stokn2
      mov  cx,50
      rep  movsb
      ret
stor_sentence  endp
;--------------------------------------------------------------------
keyword_search proc near
      mov  dx,0
      mov  al,act2
      sub  al,act1
      inc  al
looptimes:
      lea  bx,stokn2
      mov  di,bx
      add  di,dx
      lea  si,stokn1

      mov  cl,act1
      mov  ch,0
      repe  cmpsb
      je    matched
      jne   nomatched
    matched:
       lea dx,mess3
       mov ah,09
       int 21h
       mov bx,dx
       call binihex
       lea dx,mess4
       mov ah,09
       int 21h
       jmp exit_match
    nomatched:
        inc dx
        dec al
        jnz looptimes
        jz  printnoline
printnoline:
        lea dx,mess5
        mov ah,09
        int 21h
        jmp exit_match
exit_match:
        ret

keyword_search endp
;--------------------------------------------------------------------
binihex proc near
        mov ch,4
    rotate:
        mov cl,4
        rol bx,cl
        mov al,bl
        and al,0fh
        add al,30h
        cmp al,3ah
        jl printit
        add al,7h
    printit:
        mov dl,al
        mov ah,2
        int 21h
        dec ch
        jnz rotate
        ret
binihex endp
;--------------------------------------------------------------------
;--------------------------------------------------------------------
codesg ends
;--------------------------------------------------------------------
    end start
