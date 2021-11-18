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
    int tmp1 = POP_STACK;
    int tmp2 = POP_STACK;
    int num = tmp1 + tmp2;
    PUSH_STACK;
    invoker->ip += 4;
})

DEF_CMD(SUB, 4,
{
    int tmp1 = POP_STACK;
    int tmp2 = POP_STACK;
    int num = tmp1 - tmp2;
    PUSH_STACK;
    invoker->ip += 4;
})

DEF_CMD(MUL, 5, 
{
    int tmp1 = POP_STACK;
    int tmp2 = POP_STACK;
    int num = tmp1 * tmp2;
    PUSH_STACK;
    invoker->ip += 4;
})

DEF_CMD(DIV, 6,
{
    int tmp1 = POP_STACK;
    int tmp2 = POP_STACK;
    int num = tmp1 / tmp2;
    PUSH_STACK;
    invoker->ip += 4;
})

DEF_CMD(JMP, 7, 
{
    JMP_;
})

DEF_CMD(JGE, 8, 
{
    if (POP_STACK >= POP_STACK)
        JMP_;
    else
        invoker->ip += 5;
})

DEF_CMD(JLE, 9,
{
    if (POP_STACK >= POP_STACK)
        JMP_;
    else
        invoker->ip += 5;
})
DEF_CMD(CALL, 10, 
{
    CALL_;
})

DEF_CMD(RET, 11, 
{
    RET_;
})

DEF_CMD(IN, 12, 
{
    IN_;
})

DEF_CMD(OUT, 13, 
{
    OUT_;
})