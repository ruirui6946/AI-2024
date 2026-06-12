;**********************************************
datarea         segment

      string1          db         "Enter keyword:$"
      string2          db         "Enter sentence:$"
      string3          db         "Match at location:$"
      string4          db         "No match!",13,10,"$"
      string5          db         "H of the sentence.$"
      keyword          db         50D,?,50D DUP(?)  
      sentence         db         50D,?,50D DUP(?)

datarea         ends
;**********************************************
prognam      segment

;----------------------------------------------
main   proc far
        assume         ds:datarea,cs:prognam,es:datarea
start:
        push        ds
        sub         ax,ax
        push        ax

        mov         ax,datarea
        mov         ds,ax
        mov         es,ax

        lea         dx,string1          ;display 'Enter keyword'     
        mov         ah,09h
        int         21h

        lea         dx,keyword          ;enter keyword
        mov         ah,0Ah
        int         21h

        mov         ah,02h                 
        mov         dl,0Ah
        int         21h

        lea         dx,string2             
        mov         ah,09h
        int         21h

        lea         dx,sentence          ;enter sentence
        mov         ah,0Ah
        int         21h

        mov         ah,02h              
        mov         dl,0Ah
        int         21h

        lea         si,keyword+2        ;start at the third byte
        lea         di,sentence+2

        mov         ax,0
        mov         al,[sentence+1]
        mov         ah,[keyword+1]
        cmp         al,ah               ;compare the length of key and sen
       
        jl          wrong

             
        sub        al,ah
        mov        ah,0
        add        al,1
        mov        cx,ax                ;cycle index         

compare:
        push      cx
        mov al,[keyword+1]
        cbw
        mov cx,ax
        cld
        repz cmpsb
        jz         match
        lea si,keyword+2        ;key back to the first
        sub ax,cx
        dec ax
        sub  di,ax
        pop        cx
        loop       compare

;----------------------------------------------------------------
wrong:
        lea        dx,string4
        mov        ah,09h
        int        21h
        jmp        exit

;----------------------------------------------------------------
match:
        pop         cx
        sub         di,[keyword+1]         
        mov         bx,di       ;DI last character of the string matching success

        lea         dx,string3
        mov         ah,09h
        int         21h

        sub         bx,offset sentence+2
        inc         bx           ;BX the first place of matching key

        call        trans               
        lea         dx,string5
        mov         ah,09h
        int         21h

;----------------------------------------------------------------------
exit:
        mov         ah,4ch
        int         21h
        ret

;****************************************************************************

main      endp

;____________________________
trans proc near
    mov ch,4
rotate:mov cl,4
       rol bx,cl
       mov al,bl
       and al,0fh
       add al,30h
       cmp al,3ah
       jl printit
       add al,7h
printit:mov dl,al
        mov ah,2
        int 21h
        dec ch
        jnz rotate
        ret
trans endp
;____________________________

prognam         ends

;****************************************************************************
                end       start
