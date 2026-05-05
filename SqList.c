#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
//数据结构第二章
//线性表的顺序表示与实现
//代表性的就是数组，数据在内存中物理上也是连续的，表示逻辑连续，只要确定了存储空间的起始位置就可以实现随存随取

typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
//线性表的动态分配顺序存储结构，此时ElemType为int型
typedef struct
{
	int* elem;//存储空间基地址
	int len;//当前长度
	int size;//当前分配的存储容量，以sizeof（ElemType）为单位
	int inc;//每次存储空间扩容的增量
}*Sqlist, * List;//顺序表类型，定义为指针类型方便实现动态分配与回收



//------------------------------算法2.3.1---实现顺序表的初始化操作

Sqlist InitList(int size, int inc)//返回类型：顺序表类型的指针
{
	Sqlist L;//声明顺序表
	if (!(L = (Sqlist)malloc(sizeof(*L)))) exit(0);//首先确认内存是否分配出来了,如果没有就返回错误0
	L->elem = (int*)malloc(size * sizeof(int));//malloc分配size个存储空间，每个的大小看int型，返回void指针强制转int指针
	if (!L->elem)exit(0);//分配元素空间失败就要报错
	L->len = 0;//只是分配了空间，还没有存元素所以len为0
	L->inc = inc;//顺序表每次扩容的增量在初始化时就指定好
	L->size = size;
	return L;//返回指向顺序表的指针
}

//-----------------------------算法2.3.2---实现顺序表的回收操作

Sqlist FreeList(Sqlist L)
{
	//传入指向顺序表的指针L，实现顺序表的完全释放
	if (L)//如果指针不是空指针NULL
	{
		free(L->elem);//先把存储元素的地址都释放了
		free(L);//再把存储结构体本身的地址释放
		return NULL;//释放完自然是返回空指针
	}
}

//------------------------------算法2.3.3---实现顺序表两个元素的比较
Status equal_int(int a, int b)
{
	return a == b;
}

//------------------------------算法2.4---在顺序表L的第i个位置插入新的元素e
Status ListInsert(Sqlist L, int i, int e)
{
	//在顺序表L中第i个位置插入新的元素e,1<=i<=L->len+1
	if (!L || i<1 || i>L->len + 1) return ERROR;//如果参数值不合法或者顺序表L错误就直接返回false
	if (L->len >= L->size)//如果顺序表长度已满，需要继续分配
	{
		int* newbase = (int*)realloc(L->elem, (L->size + L->inc) * sizeof(int));
		if (!newbase) exit(0);
		L->elem = newbase;
		L->size += L->inc;
	}
	memmove(L->elem + i, L->elem + i - 1, (L->len - i + 1) * sizeof(int));//（目的地，来源，字节数）
	L->len++;
	*(L->elem+i-1) = e;//腾出来的位置，赋值为需要插入的元素e
	return OK;
}

//-------------------------------算法2.5---在顺序表L中删除第i个元素并且返回其值，否则返回一个错误代码
int ListDelete(Sqlist L, int i)
{
	//依旧先检测是否合法值
	if (!L || i<1 || i>L->len) return 114514;
	int result = *(L->elem + i - 1);
	memmove(L->elem + i - 1, L->elem + i, (L->len - i) * sizeof(int));//被删除的元素后的所有元素都往前移动1位
	--L->len;//顺序表的长度-1
	return result;
}


//------------------------------算法2.6---在顺序表L中查找并返回第1个与e值满足compare()的元素的位序，找不到则返回0
int LocateElem_Sq(Sqlist L , int e, Status (*equal_int)(int ,int))
{
	if (!L) exit(0);
	int i = 1;//记录位序
	int* p = L->elem;
	while (i <= L->len && !equal_int(*p++,e))//当还没遍历完也没有出现相等的时候就继续
	{
		++i;
		//逐个比较，相等则i就是元素的位序，需要返回
		//*p++的执行：先用*p作为euqal_int的参数，然后指向下一个元素
	}
	if (i <= L->len) return i;
	else return 0;
}


//-------------------------------算法2.7归并两个已经有序的顺序表，全部按非递减排序
Sqlist MergeList_Sq(Sqlist La, Sqlist Lb)
{
	Sqlist Lc;//先声明一个新的顺序表Lc用于存储
	int* pa, * pb, * pc, * pa_end, * pb_end;
	if (!La || !Lb) return NULL;//两个传入的顺序表如果有空的，直接返回空的顺序表指针NULL
	pa = La->elem; pb = Lb->elem;
	Lc = InitList(La->len + Lb->len, La->inc);//初始化Lc顺序表
	pc = Lc->elem;
	Lc->len = Lc->size;
	pa_end = La->elem + La->len - 1;
	pb_end = Lb->elem + Lb->len - 1;
	while (pa <= pa_end && pb <= pb_end)//如果两个顺序表都还没遍历完就继续循环
	{
		if (*pa >=*pb)//顺序表a的元素更大，需要放在后面，所以先把顺序表b中的此元素加入顺序表C中
		{
			*pc++ = *pb++;
		}
		else
		{
			*pc++ = *pa++;
		}
	}
	//循环结束后，有一个数组还没用完可能，需要把后面的所有元素都加入Lc中
	if (pa <= pa_end)  memcpy(pc, pa, (pa_end - pa+1)*sizeof(int));
	if (pb <= pb_end) memcpy(pc, pb, (pb_end - pb+1)*sizeof(int));
	return Lc;
}



// 打印顺序表（辅助测试）
void PrintList(Sqlist L)
{
	if (!L || L->len == 0)
	{
		printf("空表\n");
		return;
	}
	for (int i = 0; i < L->len; i++)
	{
		printf("%d ", L->elem[i]);
	}
	printf("\n");
}



//------------------------------- 主函数测试所有算法
int main()
{
	// 1. 创建顺序表 La，初始容量5，扩容增量5
	Sqlist La = InitList(5, 5);
	printf("===== 初始化 La 成功 =====\n");

	// 2. 插入元素
	ListInsert(La, 1, 10);
	ListInsert(La, 2, 20);
	ListInsert(La, 3, 30);
	ListInsert(La, 4, 40);
	ListInsert(La, 5, 50);
	printf("插入后 La：");
	PrintList(La);

	// 3. 查找元素 30
	int pos = LocateElem_Sq(La, 30, equal_int);
	printf("元素 30 的位置：%d\n", pos);

	// 4. 删除第 2 个元素
	int del = ListDelete(La, 2);
	printf("删除元素：%d，删除后 La：", del);
	PrintList(La);

	// 5. 创建另一个有序表 Lb
	Sqlist Lb = InitList(5, 5);
	ListInsert(Lb, 1, 15);
	ListInsert(Lb, 2, 25);
	ListInsert(Lb, 3, 35);
	printf("Lb：");
	PrintList(Lb);

	// 6. 归并 La 和 Lb → Lc
	Sqlist Lc = MergeList_Sq(La, Lb);
	printf("归并后 Lc：");
	PrintList(Lc);

	// 7. 释放所有内存
	La = FreeList(La);
	Lb = FreeList(Lb);
	Lc = FreeList(Lc);

	printf("\n===== 所有算法测试完成 =====\n");
	return 0;
}