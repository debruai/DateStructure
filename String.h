#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2

// 串的堆分配存储表示
typedef struct {
    char* ch;       // 若是非空串，则按串长分配存储区，否则ch为NULL
    int length;     // 串长度
} HString;

// 串的初始化
Status InitString(HString* S);

// 串的赋值操作
Status StrAssign(HString* S, const char* chars);

// 串的复制操作
Status StrCopy(HString* T, const HString* S);

// 判断串是否为空
Status StrEmpty(const HString* S);

// 比较两个串的大小
int StrCompare(const HString* S, const HString* T);

// 求串的长度
int StrLength(const HString* S);

// 清空串
Status ClearString(HString* S);

// 连接两个串
Status Concat(HString* T, const HString* S1, const HString* S2);

// 求子串
Status SubString(HString* Sub, const HString* S, int pos, int len);

// 朴素模式匹配算法（Brute-Force）
int Index_BF(const HString* S, const HString* T, int pos);

// KMP模式匹配算法（计算next数组）
void GetNext(const HString* T, int next[]);

// KMP模式匹配算法
int Index_KMP(const HString* S, const HString* T, int pos);

// 串的替换操作
Status Replace(HString* S, const HString* T, const HString* V);

// 串的插入操作
Status StrInsert(HString* S, int pos, const HString* T);

// 串的删除操作
Status StrDelete(HString* S, int pos, int len);

// 串的销毁操作
Status DestroyString(HString* S);

// 辅助函数：打印串
void PrintString(const HString* S);
