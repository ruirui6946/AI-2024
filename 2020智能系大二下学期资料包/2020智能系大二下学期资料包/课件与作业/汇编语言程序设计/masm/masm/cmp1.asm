 datarea        segment
        t1      db      'Enter keyword:','$'
        ;t2      db      'Enter Sentence:','$'
        ;t3      db      'Match at location:','$'
        ;t4      db      'H of the sentence.',13,10,'$'
        ;t5      db      'No match.',13,10,'$'

        ;keyword label   byte
              ;  maxk    db      10
              ;  actk    db      ?
              ;  kword   db      10 dup(?)

       ; sent    label   byte
          ;      maxs    db      50
          ;      acts    db      ?
          ;      ence    db      50 dup(?)

 datarea        ends

 prognam        segment

 main   proc    far
        assume cs:prognam,ds:datarea,es:datarea

 start:
        push ds
        sub ax,ax
        push ax

        mov ax , datarea
        mov ds , ax
        mov es , ax
            IRP REG,<AX,BX,CX,DX>
    PUSH REG
    ENDM 
 mIN:
        lea dx,t1
        mov ah,09h
        int 21h
        ;lea dx,keyword
        ;mov ah,0ah
        ;int 21h
 ;mINS:
        ;lea dx,t2
       ; mov ah,09h
       ; int 21h
       ; mov ah,0ah
       ; lea dx,sent
        ;int 21h
        ;mov bx,0

;lea dx,kword
;;mov ah,09h
;int 21h
;lea dx,ence
;int 21h


exit:
        ret
main endp

prognam ends
        end start


