#ifndef __FA_H__
#define __FA_H__

#include "generic.h"

typedef struct FiniteAutomaton
{
    char conditions[MAX_SIZE];    //状态符号集合
    int conditionKinds[MAX_SIZE]; //状态种类标记,0为普通状态，1位开始状态，2为终止状态或者接受状态
    int numOfConditions;          //状态的数量
    char letters[MAX_SIZE];       //字母表
    int numOfLetters;             //字母表中字母数量
    char move[MAX_SIZE * 2][3];   //接受三种状态转移
    int numOfMoves;               //状态转移的数量
} FA, *FAP;

//从文件中读取一个自动机,如果遇到空行，自动跳过
FA fa_fget(FILE *file);

//把自动机以dot脚本原因的形式写入文件
void fa_fputDot(FILE *file, FA fa);

//读取s指定路径的文件，并输出相应的自动机图表
void fa_getPng(char *s);

//在控制台上打印自动机的信息以检测正误
void fa_show(FA fa);
//定义自动机结构体

#endif