IN
POP ax
PUSH 1
POP bx

CALL Factorio

skipToRet:
    PUSH bx
    OUT
    HLT

Factorio:
    PUSH ax   
    PUSH 0    

JGE skipToRet

    PUSH ax
    PUSH bx
    MUL
    POP bx
    PUSH 1
    PUSH ax
    SUB
    POP ax

    CALL Factorio