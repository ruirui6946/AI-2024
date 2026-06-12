;************************************************************************
datarea         segment                                 ;define data segment
        string1 db      'Move the cursor backward.'
        string2 db      'Move the cursor bbckward.'
;
        mess1   db      'Match.' , 13 , 10 , '$'
        mess2   db      'No match!' , 13 , 10 , '$'
datarea         ends        
;*************************************************************************
prognam         segment                                 ;define code segment
;-------------------------------------------------------------------------
main    proc    far
        assume  cs:prognam , ds:datarea , es:datarea
start:                                                  ;starting execution address
;set up stack for return
        push    ds                                      ;save old data segment
        sub     ax , ax                                 ;put zero in AX
        push    ax                                      ;save it on stack
;set DS register to current data segment
        mov     ax , datarea                            ;datarea segment addr
        mov     ds , ax                                 ;       into DS register
        mov     es , ax                                 ;       into ES regiser
;MAIN PART OF PROGRAM GOES HERE
        lea     si , string1
        lea     di , string2
        cld
        mov     cx , 25
        repz    cmpsb
        jz      match
        lea     dx , mess2
        jmp     short disp
match:
        lea     dx , mess1
disp:
        mov     ah , 09
        int     21h
        ret                                             ;return to DOS
main    endp                                            ;end of main part of prognam
;-------------------------------------------------------------------------
prognam ends                                            ;end of code segment                                                                                                                                    
;*************************************************************************
        end     start                                   ;end of assembly
