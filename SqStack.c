#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define INFEASIBLE -1
//第三章栈与队列------------栈的数据结构与算法实现
typedef int ElemType;

typedef struct
{
	ElemType* base;//基地址，也叫栈底指针，初始化时指向分配的元素存储空间
	ElemType* top;//栈顶指针
	int size;//元素存储容量
} *SqStack;

SqStack InitStack(int size)
{
	//构造并返回一个空的顺序栈
	SqStack S;
	if (!(S = (SqStack)malloc(sizeof(*S)))) exit(OVERFLOW);
	if (!(S->base = (ElemType*)malloc( size * sizeof(ElemType)))) exit  (OVERFLOW);

	S->top = S->base;//栈里还没有元素，所有栈顶和栈底是暂时重合的
	return S;
}

ElemType GetTop(SqStack S)
{
	//若栈不为空，则返回栈顶元素，否则返回错误码
	if (!S || S->top == S->base) return -10086;
	return *(S->top - 1);
}

Status DestroyStack(SqStack S)
{
	//销毁栈的函数
	if (!S) return -10086;

	if (S->base)
	{
		free(S->base);
		S->base = NULL;
	}

	free(S);

	return OK;
}

Status Push(SqStack S, ElemType e)
{
	//入栈，插入元素e，作为新的栈顶元素
	if (!S) { return ERROR; }
	if (S->top - S->base >= S->size)//如果需要扩容的话
	{
		S->base = (ElemType*)realloc(S->base, S->size * 2 * sizeof(ElemType));
		if (!S->base) return ERROR;
		S->top = S->base + S->size;
	}
	*S->top++ = e;
	return OK;
}

ElemType Pop(SqStack S)
{
	if (!S || S->top == S->base) return ERROR;
	return * --S->top;
}

bool StackEmpty(SqStack S)
{
	if (S->top == S->base) return true;
	return false;
}

//数值转换，利用栈来实现
void Conversion(int N, int x)
{
	//将非负十进制数N转为x进制,并且打印输出
	SqStack S = InitStack(10);
	while (N)
	{
		Push(S, N % x);
		N = N / x;
	}
	while (!StackEmpty(S))
	{
		printf("%d", Pop(S));
	}
	printf("\n");
	DestroyStack(S);//用完要销毁栈
}