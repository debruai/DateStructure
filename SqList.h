#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// 数据结构第二章
// 线性表的顺序表示与实现
// 代表性的就是数组，数据在内存中物理上也是连续的，表示逻辑连续，只要确定了存储空间的起始位置就可以实现随存随取

typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2

// 线性表的动态分配顺序存储结构，此时ElemType为int型
typedef struct
{
    int* elem;  // 存储空间基地址
    int len;    // 当前长度
    int size;   // 当前分配的存储容量，以sizeof(int)为单位
    int inc;    // 每次存储空间扩容的增量
} SqList, * List;  // 顺序表类型，定义为指针类型方便实现动态分配与回收

// 打印顺序表
void SqPrintList(SqList* L);

//------------------------------算法2.3.1---实现顺序表的初始化操作
SqList* SqInitList(int size, int inc); // 返回类型：顺序表类型的指针

//-----------------------------算法2.3.2---实现顺序表的回收操作
void SqFreeList(SqList* L);
//------------------------------算法2.3.3---实现顺序表两个元素的比较
Status SqEqualInt(int a, int b);

//------------------------------算法2.4---在顺序表L的第i个位置插入新的元素e
Status SqListInsert(SqList* L, int i, int e);

//-------------------------------算法2.5---在顺序表L中删除第i个元素并且返回其值，否则返回一个错误代码
Status SqListDelete(SqList* L, int i, int* e);

//------------------------------算法2.6---在顺序表L中查找并返回第1个与e值满足compare()的元素的位序，找不到则返回0
int SqLocateElem(SqList* L, int e, Status(*compare)(int, int));
//-------------------------------算法2.7归并两个已经有序的顺序表，全部按非递减排序
SqList* SqMergeList(SqList* La, SqList* Lb);
// 获取顺序表长度
int SqListLength(SqList* L);

// 判断顺序表是否为空
Status SqListEmpty(SqList* L);

// 获取指定位置的元素
Status SqGetElem(SqList* L, int i, int* e);

// 修改指定位置的元素
Status SqSetElem(SqList* L, int i, int e);

// 清空顺序表
Status SqClearList(SqList* L);
// 查找元素的前驱
Status SqPriorElem(SqList* L, int cur_e, int* pre_e);
// 查找元素的后继
Status SqNextElem(SqList* L, int cur_e, int* next_e);
