#include<stdio.h>
#include<stdlib.h>
typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define INFEASIBLE -1
//双向链表学习
typedef struct DuLNode
{
	int data;//数据域
	struct DuLNode* prev;//向前指针
	struct DuLNode* next;//向后指针
}DuLNode, * DuLinkList;//结点类型，双向链表指针类型


//--------------------------------算法2.14---查找i位序的结点,返回位序为i的结点指针，不合法则返回NULL
DuLinkList GetElemPos(DuLinkList L, int i)
{
	if (!L || i < 0) return NULL;
	if (i == 0) return L;
	DuLinkList p = L->next; int count = 1;
	while (p != L && count < i)
	{
		p = p->next;
		count++;
	}
	if (p == L) return NULL;//第i个结点元素不存在，就返回一个空指针
	return p;
}

//----------------------------------算法2.15---在第i个结点之前插入元素e
Status ListInsert(DuLinkList L, int i, int e)
{
	if (!L || i < 1) return ERROR;
	DuLNode* p, * s;
	if (!GetElemPos(L, i - 1)) return ERROR;//p指向第i-1结点，若P空则报错
	if (!(s = (DuLNode*)malloc(sizeof(DuLNode)))) return ERROR;
	s->data = e;
	s->next = p->next;
	p->next->prev = s;
	s->prev = p;
	p->next = s;
	return OK;
}

//----------------------------------算法2.16---删除带头结点的循环链表的第i个元素，返回其值
int DulLinkListDelete(DuLinkList L, int i)
{
	if (!L || i < 1) return ERROR;
	DuLNode* p;
	if (!(p=GetElemPos(L, i))) return ERROR;//双向链表查找第i个结点
	int e = p->data;//取出p结点的指针
	p->prev->next = p->next;//改变指针，把p前后的结点逻辑相连
	p->next->prev = p->prev;
	free(p);
	return e;
}
