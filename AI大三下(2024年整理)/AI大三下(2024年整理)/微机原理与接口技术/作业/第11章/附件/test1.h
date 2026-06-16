BEGIN:	MOV AL,0        ;下限值
        MOV DX,220H     ;地址
UP:	    OUT DX,AL
        CALL DELAY
        INC AL
        CMP AL,00H      ;0FFH+1
        JNZ UP
        DEC AL
DOWN:	OUT DX,AL
        CALL DELAY
        DEC AL
        CMP AL,00H
        JNZ DOWN
        JMP BEGIN

