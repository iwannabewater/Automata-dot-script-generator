#include "fa.h"

//从文件中读取一个自动机
FA fa_fget(FILE *file)
{
    FA out = {0};
    char str[300] = {0};
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
    //读取开始状态
    my_fscanf(file, "%s", str);
    char st;
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

    //读取状态转移，先读取到开始符号为止
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
        sscanf(str, "%c,%c,%c", &out.move[out.numOfMoves][0], &out.move[out.numOfMoves][1], &out.move[out.numOfMoves][2]);
        out.numOfMoves++;
    } while (1);
    return out;
}

//把自动机以dot脚本的形式写入文件
void fa_fputDot(FILE *file, FA fa)
{
    fprintf(file, "digraph FiniteAutomaton{\n");
    fprintf(file, "rankdir=LR\n");
    //随着节点数量调整画布大小
    int size = fa.numOfConditions * 2;
    fprintf(file, "size=%d", size);
    //打印节点
    for (int i = 0; i < fa.numOfConditions; i++)
    {
        //如果是普通状态
        if (fa.conditionKinds[i] == 0)
        {
            fprintf(file, "\n%c[shape=circle]", fa.conditions[i]);
        }
        //如果是终止状态或者接受状态
        else if (fa.conditionKinds[i] == 2)
        {
            fprintf(file, "\n%c[shape=doublecircle]", fa.conditions[i]);
        }
        //如果是开始状态
        else if (fa.conditionKinds[i] == 1)
        {
            fprintf(file, "\n%c[shape=circle]", fa.conditions[i]);
            fprintf(file, "\nstart[label=\"\",shape=none]");
            fprintf(file, "\nstart->%c[label=\"start\"]", fa.conditions[i]);
        }
    }
    //打印线
    for (int i = 0; i < fa.numOfMoves; i++)
    {
        fprintf(file, "\n%c->%c[label=%c]", fa.move[i][0], fa.move[i][2], fa.move[i][1]);
    }
    fprintf(file, "\n}");
}

//读取s指定路径的文件，并输出相应的自动机图表
void fa_getPng(char *s)
{
    printf("%s\'s png generated!\n", s);
    char termName[500];
    char base[100] = {0};
    strncpy(base, s, strlen(s) - 4);
    FILE *file = fopen(s, "r");
    if (file == NULL) 
    {
        perror("fa_getPng");
        exit(-1);
    }
    int i = 0;
    while (!feof(file))
    {
        FA fa = fa_fget(file);
        //生成对应dot文档
        //先获取dot文档文件名
        sprintf(termName, "%s_%d.dot", base, i);
        FILE *fp = fopen(termName, "w");
        if (fp == NULL)
        {
            perror("fa_getPng");
            exit(-1);
        }
        fa_fputDot(fp, fa);
        fclose(fp);
        //生成控制生成图片的脚本编译命令
        char cmd[300] = {0};
        sprintf(cmd, "dot -Tpng %s_%d.dot -o %s_%d.png", base, i, base, i);
        system(cmd);
        i++;
    }
    fclose(file);
}

//在控制台上打印自动机的信息以检测正误
void fa_show(FA fa)
{
    printf("\nconditions:");
    for (int i = 0; i < fa.numOfConditions - 1; i++)
    {
        printf("%c,", fa.conditions[i]);
    }
    if (fa.numOfConditions >= 1)
        printf("%c", fa.conditions[fa.numOfConditions - 1]);
    printf("\nletters:");
    for (int i = 0; i < fa.numOfLetters - 1; i++)
    {
        printf("%c,", fa.letters[i]);
    }
    if (fa.numOfLetters >= 1)
        printf("%c", fa.letters[fa.numOfLetters - 1]);
    printf("\nstart condition:");
    for (int i = 0; i < fa.numOfConditions; i++)
    {
        if (fa.conditionKinds[i] == 1)
        {
            printf("%c", fa.conditions[i]);
            break;
        }
    }
    printf("\nfinal conditions:");
    int first = 0;
    for (int i = 0; i < fa.numOfConditions; i++)
    {
        if (fa.conditionKinds[i] == 2)
        {
            if (first == 0)
            {
                first = 1;
                printf("%c", fa.conditions[i]);
            }
            else
                printf(",%c", fa.conditions[i]);
        }
    }
    printf("\nmove vectors:"); //状态转移向量
    for (int i = 0; i < fa.numOfMoves; i++)
    {
        printf("\n%c,%c,%c", fa.move[i][0], fa.move[i][1], fa.move[i][2]);
    }
}