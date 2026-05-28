#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define INFEASIBLE -1
typedef int Elemtype;

// 队列结点结构
typedef struct QNode
{
    Elemtype data;          // 数据域
    struct QNode* next;     // 指针域，这是一个指向下一个结点结构体的指针
} QNode, * QueuePtr;

// 队列结构
typedef struct
{
    QueuePtr front;  // 队头指针
    QueuePtr rear;   // 队尾指针
} LinkQueue;

// 初始化一个队列的函数，返回一个队列指针
LinkQueue* initQueue();

// 回收队列Q的函数
Status FreeQueueL(LinkQueue* Q);

// 清空队列的函数，将Q置为空队列，有头结点
Status ClearQueueL(LinkQueue* Q);

// 获取队列长度
int QueueLenL(LinkQueue* Q);

// 入队列的函数
Status EnQueue(LinkQueue* Q, Elemtype newdata);

// 出队列的函数
Status DeQueueL(LinkQueue* Q, Elemtype* e);

// 若队列不为空，返回队头元素，否则返回ERROR
Elemtype GetHeadL(LinkQueue* Q);

// 打印队列所有元素
Status PrintQueue(LinkQueue* Q);
