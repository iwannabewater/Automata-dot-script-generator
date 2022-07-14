#ifndef __PDA_H__
#define __PDA_H__

#include "generic.h"

//定义下推自动机结构体
typedef struct PushDownAutomaton
{
    char conditions[MAX_SIZE];                          //状态符号集合
    int conditionKinds[MAX_SIZE];                       //状态种类标记,0为普通状态，1位开始状态，2为终止状态或者接受状态
    int numOfConditions;                                //状态的数量
    char stacks[MAX_SIZE];                              //栈符号集合
    int numOfStacks;                                    //栈符号数量
    char firstStack;                                    //初始栈底符号
    char letters[MAX_SIZE];                             //字母表
    int numOfLetters;                                   //字母表中字母数量
    char moveCases[MAX_SIZE][MAX_CON_SIZE][MAX_STRLEN]; //跳转条件,由move->moveSE->moveCases
    int numOfMoveCases[MAX_SIZE];                       //初始为0,每加入一个条件后加一
    int moveSE[MAX_SIZE][MAX_SIZE];                     //用做哈希的中转，查询对应的跳转条件的下标，以及查询对应跳转是否存在，不存在则初始值为-1
    char move[MAX_SIZE][2];                             // 0位存储跳转初态，1位存储跳转末态
    int numOfMoves;                                     //状态转移的数量
} PDA, *PDAP;

//按照一定的格式拼接条件字符串
void pda_casescat(char *dir, char sarr[MAX_CON_SIZE][MAX_STRLEN], int size);

//初始化转移条件表
void pda_initMoveSE(PDAP pda);

//从文件中读取一个自动机,如果遇到空行，自动跳过
PDA pda_fget(FILE *file);

//把自动机以dot脚本原因的形式写入文件
void pda_fputDot(FILE *file, PDA pda);

//读取s指定路径的文件，并输出相应的自动机图表
void pda_getPng(char *s);

#endif
