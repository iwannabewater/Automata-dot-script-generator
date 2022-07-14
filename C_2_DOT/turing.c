#include "turing.h"

//读取s指定路径的文件，并输出相应的自动机图表
void tm_getPng(char *s)
{
    printf("%s\'s png generated!\n", s);
    char termName[500];
    char base[100] = {0};
    strncpy(base, s, strlen(s) - 4);
    FILE *file = fopen(s, "r");
    if (file == NULL)
    {
        perror("tm_getPng");
        exit(-1);
    }
    int i = 0;
    while (!feof(file))
    {
        TM tm = tm_fget(file);
        //生成对应dot文档
        //先获取dot文档文件名
        sprintf(termName, "%s_%d.dot", base, i);
        FILE *fp = fopen(termName, "w");
        if (fp == NULL)
        {
            perror("tm_getPng");
            exit(-1);
        }
        tm_fputDot(fp, tm);
        fclose(fp);
        //生成控制生成图片的脚本编译命令
        char cmd[300] = {0};
        sprintf(cmd, "dot -Tpng %s_%d.dot -o %s_%d.png", base, i, base, i);
        system(cmd);
        i++;
    }
    fclose(file);
}

//初始化转移条件表
void tm_initMoveSE(TMP tm)
{
    for (int i = 0; i < MAX_SIZE; i++)
    {
        for (int j = 0; j < MAX_SIZE; j++)
        {
            tm->moveSE[i][j] = -1;
        }
    }
}

//从文件中读取一个自动机
TM tm_fget(FILE *file)
{
    TM out = {0};
    char str[300] = {0};
    // TODO 初始化moveCases
    tm_initMoveSE(&out);

    //读取状态集合
    my_fscanf(file, "%s", str);
    int len = strlen(str);
    out.numOfConditions = len / 2 + 1;
    char term[300];
    for (int i = 0; i < out.numOfConditions; i++)
    {
        sscanf(str, "%c,%s", &out.conditions[i], term);
        strcpy(str, term);
    }
    //读取字母表
    my_fscanf(file, "%s", str);
    len = strlen(str);
    out.numOfLetters = len / 2 + 1;
    for (int i = 0; i < out.numOfLetters; i++)
    {
        sscanf(str, "%c,%s", &out.letters[i], term);
        strcpy(str, term); //把term的字符串复制给st
    }

    //读取带符号集合
    my_fscanf(file, "%s", str);
    len = strlen(str);
    out.numOfTapeLetters = len / 2 + 1;
    for (int i = 0; i < out.numOfTapeLetters; i++)
    {
        sscanf(str, "%c,%s", &out.tapeLetters[i], term);
        strcpy(str, term); //把term的字符串复制给st
    }

    //读取空白带符号定义
    my_fscanf(file, "%s", str);
    sscanf(str, "%c", &out.emptyTapeLetter);

    //读取开始状态
    char st;
    my_fscanf(file, "%s", str);
    sscanf(str, "%c", &st);

    //读取终止状态或者接受状态
    char finals[300];
    my_fscanf(file, "%s", str);
    len = strlen(str);
    int lenOfFinals = len / 2 + 1;
    for (int i = 0; i < lenOfFinals; i++)
    {
        sscanf(str, "%c,%s", &finals[i], term);
        strcpy(str, term); //把term的字符串复制给st
    }
    //根据读到的开始状态或者接受状态对状态种类进行标记
    for (int i = 0; i < out.numOfConditions; i++)
    {
        if (out.conditions[i] == st)
        {
            out.conditionKinds[i] = 1;
            continue;
        }
        else
        {
            for (int j = 0; j < lenOfFinals; j++)
            {
                if (out.conditions[i] == finals[j])
                {
                    out.conditionKinds[i] = 2;
                    break;
                }
            }
        }
    }

    // 读取状态转移，先读取到开始符号为止
    do
    {
        my_fscanf(file, "%s", str);
        if (strcmp(str, "[") == 0)
            break;
    } while (1);
    //读取状态转移向量
    out.numOfMoves = 0;
    do
    {
        my_fscanf(file, "%s", str);
        if (strcmp(str, "]") == 0)
            break;
        char curMoveCase[100]; //跳转条件
        char moveS;            //跳转开始状态
        char moveE;            //跳转结束状态
        sscanf(str, "%c>%c:%s", &moveS, &moveE, curMoveCase);
        //先判断这个方向和首位的跳转是否已经存在
        //如果该线条不存在,添加该线条
        if (out.moveSE[(int)moveS][(int)moveE] == -1)
        {
            out.move[out.numOfMoves][0] = moveS;
            out.move[out.numOfMoves][1] = moveE;
            out.moveSE[(int)moveS][(int)moveE] = out.numOfMoves;
            //当前线条的下标
            int indexOfCurMove = out.numOfMoves;
            //当前线条的条件数量(新增条件的下标)
            int indexOfCurCon = out.numOfMoveCases[indexOfCurMove];
            //传递读取到的条件到该位置
            strcpy(out.moveCases[indexOfCurMove][indexOfCurCon], curMoveCase);
            //更新条件下标和线条下标
            out.numOfMoveCases[out.numOfMoves]++;
            out.numOfMoves++;
        }
        //如果该线条存在，往对应条件中添加
        else
        {
            //获取当前线条的下标
            int indexOfCurMove = out.moveSE[(int)moveS][(int)moveE];
            //当前线条的条件数量(新增条件的下标)
            int indexOfCurCon = out.numOfMoveCases[indexOfCurMove];
            //传递读取到的条件到该位置
            strcpy(out.moveCases[indexOfCurMove][indexOfCurCon], curMoveCase);
            //更新条件下标
            out.numOfMoveCases[indexOfCurMove]++;
        }
    } while (1);
    return out;
}

//把自动机以dot脚本形式的形式写入文件
void tm_fputDot(FILE *file, TM tm)
{
    fprintf(file, "digraph TuringMachine{\n");
    //设置节点排布方式为从左到右
    fprintf(file, "rankdir=LR\n");
    //根据节点数量调节文件大小
    int size = tm.numOfConditions * 2;
    fprintf(file, "size=%d\n", size);

    //打印节点
    for (int i = 0; i < tm.numOfConditions; i++)
    {
        //如果是普通状态
        if (tm.conditionKinds[i] == 0)
        {
            fprintf(file, "\n%c[shape=circle]", tm.conditions[i]);
        }
        //如果是终止状态或者接受状态
        else if (tm.conditionKinds[i] == 2)
        {
            fprintf(file, "\n%c[shape=doublecircle]", tm.conditions[i]);
        }
        //如果是开始状态
        else if (tm.conditionKinds[i] == 1)
        {
            fprintf(file, "\n%c[shape=circle]", tm.conditions[i]);
            fprintf(file, "\nstart[label=\"\",shape=none]");
            fprintf(file, "\nstart->%c[label=\"start\"]", tm.conditions[i]);
        }
    }
    //打印跳转
    for (int i = 0; i < tm.numOfMoves; i++)
    {
        char moveS = tm.move[i][0];
        char moveE = tm.move[i][1];
        int numOfCurCases = tm.numOfMoveCases[i];

        //加上字符串的拼接
        char curMoveCase[500] = {0};
        tm_casescat(curMoveCase, tm.moveCases[i], numOfCurCases);
        //以拼接好的条件写入字符串
        fprintf(file, "\n%c->%c[label=\"%s\"]", moveS, moveE, curMoveCase);
    }
    fprintf(file, "\n}");
}

//按照一定的格式拼接条件字符串
void tm_casescat(char *dir, char sarr[MAX_CON_SIZE][MAX_STRLEN], int size)
{
    if (size < 1)
    {
        printf("wrong input in tm_casescat");
        return; //保证程序稳健性的意外处理
    }
    int len = 0;
    for (int i = 0; i < size - 1; i++)
    {
        int j = 0;
        while (sarr[i][j] != '\0')
        {
            dir[len++] = sarr[i][j];
            j++;
        }
        dir[len++] = '\\';
        dir[len++] = 'n';
    }
    int j = 0;
    while (sarr[size - 1][j] != '\0')
    {
        dir[len++] = sarr[size - 1][j];
        j++;
    }
    dir[len] = '\0';
}