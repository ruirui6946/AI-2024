BEGIN:	MOV AL,3DH      ;下限值
        MOV DX,220H     ;地址
UP:	    OUT DX,AL
        CALL DELAY
        INC AL
        CMP AL,0CDH     ;
        JNZ UP
        OUT DX,AL
        CALL DELAY_20MS ;上限值，持续20ms
        DEC AL
DOWN:	OUT DX,AL
        CALL DELAY
        DEC AL
        CMP AL,3DH
        JNZ DOWN
        OUT DX,AL
        CALL DELAY_20MS ;下限值，持续20ms
        INC AL
        JMP UP
