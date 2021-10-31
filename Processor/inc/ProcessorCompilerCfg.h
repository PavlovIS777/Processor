#ifndef PROCESSORCOMPILERCFG_H
#define PROCESSORCOMPILERCFG_H

enum cmdHash
{
    HLT  = 12702567513871516423, //0
    PUSH = 10765274589010315799, //1
    POP  = 5089677864028293143 , //2
    ADD  = 17827079209670796541, //3
    SUB  = 5144971033618411685 , //4
    MUL  = 743300950465355368  , //5
    JMP  = 9173871738894164855 , //6
    JGE  = 4000240509173947135 , //7
    JLE  = 11284796518070371048, //8
    CALL = 12933319170532227001, //9
    RET  = 888342110980918609    //10
    
};  

enum regHash
{
    ax = 12174441381696988942, //1
    bx = 10671071278501398061, //2
    cx = 11969467366185082959, //3
    dx = 12187072656125612012  //4
};

struct label
{
    size_t hash;
    int ip;
};

struct Marks
{
    int marksCount;
    int capacity;
    label* labels;
};

struct Labels
{
    int labelsCount;
    int capacity;
    label* labels;
};







#endif //!PROCESSORCOMPILERCFG_H