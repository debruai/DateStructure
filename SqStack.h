#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define INFEASIBLE -1

typedef int ElemType;

// 栈结构定义
typedef struct
{
    ElemType* base;  // 基地址，也叫栈底指针，初始化时指向分配的元素存储空间
    ElemType* top;   // 栈顶指针
    int size;        // 元素存储容量
} SqStack, * StackPtr;  // 这里使用SqStack作为结构体类型，StackPtr作为指针类型

// 初始化栈
StackPtr InitStack(int size);

// 获取栈顶元素
Status GetTop(StackPtr S, ElemType* e);

// 销毁栈
Status DestroyStack(StackPtr S);

// 入栈
Status Push(StackPtr S, ElemType e);

// 出栈
Status Pop(StackPtr S, ElemType* e);

// 判断栈是否为空
bool StackEmpty(StackPtr S);

// 获取栈的当前长度
int StackLength(StackPtr S);

// 清空栈
Status ClearStack(StackPtr S);

// 数值转换，利用栈来实现
void Conversion(int N, int x);

// 括号匹配检查
Status BracketMatch(const char* str);

// 打印栈
void PrintStack(StackPtr S);
