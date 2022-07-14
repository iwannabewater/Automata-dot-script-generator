#include "generic.h"

//获取文件的种类
Kind get_fileKind(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        perror("get_fileKind");
        exit(-1);
    }
    char term[MAX_STRLEN];
    int i = 0;
    do
    {
        my_fscanf(file, "%s", term);
        if (strcmp(term, "[") == 0)
            break;
        i++;
    } while (1);
    //经过循环后i为读到[(转移向量开始输入标记)之前读取的行数

    //如果读取到的是fa,状态转移函数位于5-1=4行
    if (i == 4)
    {
        fclose(file);
        return FA_FILE;
    }
    //否则读取到的是pda或者turing machine
    //读取一个状态转移向量来判断是图灵机还是下推自动机
    my_fscanf(file, "%s", term);
    int len = strlen(term);
    //如果读取到的是图灵机
    if ((term[len - 1] == 'R' || term[len - 1] == 'L') && term[len - 2] == ',')
    {
        fclose(file);
        return TM_FILE;
    }
    //如果不是图灵机又不是有穷自动机则是下推自动机
    fclose(file);
    return PDA_FILE;
}

//如果该字符串s以//开头，则说明它和它后面同行字符都是注释，则返回1，否则返回0
int is_comment(char *s)
{
    if (strlen(s) >= 2 && s[0] == '/' && s[1] == '/')
    {
        return 1;
    }
    return 0;
}

//从文件输入流读取一行字符串（当输入参数为std时为从控制台读取一行字符串，可包含空格)
void str_readline(FILE *path)
{
    while (!feof(path))
    {
        char add = fgetc(path);
        if (add == EOF)
            break;
        if (add == '\n')
            break;
        if (add == '\r')
            break;
    }
}

//带忽略注释地读取一个字符串
void my_fscanf(FILE *file, char *mode, char *target)
{
    while (1)
    {
        fscanf(file, mode, target);
        if (is_comment(target))
            str_readline(file);
        else
            break;
    }
}

// help_info
void help_info()
{
    printf("*****************************\n");
    printf("1. finite automaton\n");
    printf("2. pushdown automaton\n");
    printf("3. turing machine\n");
    printf("*****************************\n");

    //选择展示自动机的格式
    int choice = 0;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("******************************FA Tutorail****************************\n");
            printf("A,B,C,D    //第一行是状态集合\n");
            printf("1,2,3      //第二行是字母表，用,隔开\n");
            printf("A          //第三行是开始状态，只能是一个字符\n");
            printf("C,D        //第四行是终结状态集合，能够输入多个，一般来说不能和开始状态重复\n");
            printf("[ A,1,B A,3,D B,2,C B,3,D ]\n");
            printf("           //第五行开始是状态转移输入，用[开始，空一个再开始输入状态转移\n");
            printf("           //每个状态转移方程的输入格式如右，开始状态，遇到字符，跳转状态\n");
            printf("           //每个输入的状态之间空一格\n");
            printf("           //完成输入状态之后，需要空一格，输入]标记结束输入状态\n");
            printf("*********************************************************************\n");
            break;
        case 2:
            printf("******************************PDA Tutorail***************************\n");
            printf("A,B,C,D    //第一行是状态集合\n");
            printf("1,2,3      //第二行是字母表，用,隔开\n");
            printf("F,Z,M,N    //第三行是栈符合集\n");
            printf("A          //第四行是开始状态，只能是一个字符\n");
            printf("C,D        //第五行是终结状态集合\n");
            printf("Z          //第六行是初始栈底符号\n");
            printf("[ A>B:2,Z/1Z B>C:3,N/e A>D:2,1/M1 ]\n");
            printf("           //状态转移向量输入同样是以[开头，空一格开始\n");
            printf("           //以空一格加]结束\n");
            printf("           //每个状态转移向量以一个空格隔开\n");
            printf("*********************************************************************\n");
            break;
        case 3:
            printf("******************************TM Tutorail****************************\n");
            printf("A,B,C,D    //第一行是状态集合\n");
            printf("1,2,3      //第二行是字母表，用,隔开\n");
            printf("M,Q,Z      //第三行是带符号集合,它总是包含字母表\n");
            printf("e          //第四行是空白带符号定义\n");
            printf("A          //第五行是开始状态\n");
            printf("C,D        //第六行是终止带符号\n");
            printf("[ A>B:M/Q,L B>C:Q/Z,R ]\n");
            printf("           //第七行状态转移向量，中间用空格隔开\n");
            printf("*********************************************************************\n");
            break;
        default:
            printf("Sorry, your input is incorrect, the program exits\n");
            break;
    }
}