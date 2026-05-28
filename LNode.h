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

// 遍历函数Visit：输出元素
Status Visit(int e);

// 单链表结构
typedef struct LNode
{
    int data;               // 数据域
    struct LNode* next;     // 指针域，一个指向下一结点结构体的指针
} LNode, * LinkList;

// 创建一个新链表
LinkList InitList();

// 获取第i个元素的值
int LNodeGetElem(LinkList L, int i);

// 在带头结点的单链表L中第i个元素之前插入元素e
Status LNodeListInsert(LinkList L, int i, int e);

// 在带头结点的单链表L中，删除第i个元素，并返回其值
int LNodeListDelete(LinkList L, int i);

// 由s的前n个元素构建并且返回带头结点的单链表
LinkList LNodeCreateList(int n, int s[]);

// 已知单链表La和Lb元素非递减排列,归并他们并返回结果链表Lc，也非递减排列
LinkList LNodeMergeList(LinkList La, LinkList Lb);

// 遍历带头结点的单链表L的每个元素
Status LNodeListTraverse(LinkList L, Status(*Visit)(int));

// 获取链表长度
int LNodeListLength(LinkList L);

// 清空链表
Status LNodeClearList(LinkList L);

// 销毁链表
Status LNodeDestroyList(LinkList L);

// 判断链表是否为空
Status LNodeListEmpty(LinkList L);

// 获取指定元素的前驱
Status LNodePriorElem(LinkList L, int cur_e, int* pre_e);

// 获取指定元素的后继
Status LNodeNextElem(LinkList L, int cur_e, int* next_e);

// 查找元素位置
int LNodeLocateElem(LinkList L, int e);

// 反转链表
Status LNodeReverseList(LinkList L);

// 打印链表
void LNodePrintList(LinkList L);
