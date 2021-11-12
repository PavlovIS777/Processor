DEF_CMD(HLT, 0,
{
    HLT_;
})

DEF_CMD(PUSH, 1,
{
    PUSH_;
})

DEF_CMD(POP, 2, 
{
    POP_;
})

DEF_CMD(ADD, 3,
{
    PUSH_;
})

DEF_CMD(SUB, 4,
{
    PUSH_;
})

DEF_CMD(MUL, 5, 
{
    PUSH_;
})

DEF_CMD(DIV, 6,
{
    int tmp1 = POP_;
    int tmp2 = POP_;
    tmp2 = tmp1 / tmp2;
    pushMyStack(&invoker->stack, (ptr_t)&tmp2);
})

DEF_CMD(JMP, 7, 
{
    JMP_;
})

DEF_CMD(JGE, 8, 
{
    if (POP_ >= POP_)
        JMP_;
})

DEF_CMD(JLE, 9,
{
    if (POP_ >= POP_)
        JMP_;
})
DEF_CMD(CALL, 10, 
{
    CALL_;
})

DEF_CMD(RET, 11, 
{
    RET_;
})