#ifndef __GENERIC_H__
#define __GENERIC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//定义最多接受的控制台参数的数量
#define MAX_INPUT_NUM 100
//最多跳转数量
#define MAX_SIZE 200
//最多跳转条件数量
#define MAX_CON_SIZE 20
//最大字符串长度
#define MAX_STRLEN 100

//自动机类型枚举
typedef enum FILEKIND
{
    FA_FILE,
    PDA_FILE,
    TM_FILE
} Kind;

//判断文件的种类
extern Kind get_fileKind(char *fileName);

//判断读取到的字符是否是注释
extern int is_comment(char *s);

//读取该行后面部分，并忽略
extern void str_readline(FILE *file);

//带忽略注释地读取一个字符串
extern void my_fscanf(FILE *file, char *mode, char *target);

//help info
extern void help_info();

#endif