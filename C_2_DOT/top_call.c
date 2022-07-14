#include "fa.h"
#include "pda.h"
#include "turing.h"

/*
   顶层模块，用于调用三个子程序
   备注，当且仅当三个子程序存在，
   且三个子程序命名为turing,pda,fa的时候才能正常使用
*/

int main(int argc, char *argv[])
{
    //在使用前对每个文件进行检查，判断文件中的自动机类型

    //顶层模块不支持获取控制台输入
    if (argc < 2)
    {
        printf("You must input command line parameters as filename\n");
        return 1;
    }

    //使用手册
    if (strcmp(argv[1], "--help") == 0)
    {
        help_info();
        return 2;
    }

    //char s[200] = {0};
    //如果输入了正确的控制台参数作为要获取自动机的文件名
    for (int i = 1; i < argc; i++)
    {
        // 根据文件种类的不同选择不同的程序
        Kind fileKind = get_fileKind(argv[i]);
        switch (fileKind)
        {
            case FA_FILE:
                fa_getPng(argv[i]);
                break;
            case PDA_FILE:
                pda_getPng(argv[i]);
                break;
            default:
                tm_getPng(argv[i]);
                break;
        }
    }
    return 0;
}