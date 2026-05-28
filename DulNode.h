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

// 双向链表结构
typedef struct DuLNode
{
    int data;                // 数据域
    struct DuLNode* prev;    // 向前指针
    struct DuLNode* next;    // 向后指针
} DuLNode, * DuLinkList;     // 结点类型，双向链表指针类型

// 初始化循环双向链表
DuLinkList InitDuLinkList();

// 查找第i个元素（i从1开始，返回对应结点指针）
DuLinkList GetElemPos(DuLinkList L, int i);

// 在第i个位置之前插入元素e
Status DuLListInsert(DuLinkList L, int i, int e);

// 删除第i个元素，返回其值
int DuLListDelete(DuLinkList L, int i);

// 打印链表
void PrintDuLinkList(DuLinkList L);

// 反向打印链表
void PrintDuLinkListReverse(DuLinkList L);

// 获取链表长度
int DuLGetLength(DuLinkList L);

// 释放链表
Status FreeDuLinkList(DuLinkList L);
