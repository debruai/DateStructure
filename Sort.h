#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define INFEASIBLE -1

// 所有排序算法操作同一个数组，定义最大长度
#define MAXSIZE 100

// 待排序的"顺序表"
typedef struct
{
    int key;             // 关键字
    // 可以扩展其他信息（比如原始下标用于验证稳定性）
    int originalIndex;
} RedType;

// 顺序表
typedef struct
{
    RedType r[MAXSIZE + 1];  // r[0] 闲置或用作哨兵
    int length;              // 当前长度
} SqList;

// 初始化顺序表（从数组）
void InitList(SqList* L, int keys[], int n);

// 复制顺序表（用于不同排序的独立测试）
void CopyList(SqList* dest, SqList* src);

// 打印顺序表
void PrintList(SqList* L, const char* msg);

// 判断是否已排序（从小到大）
int IsSorted(SqList* L);

//---------------------------------------------------------------
//  9 种排序算法（内部排序）
//---------------------------------------------------------------

// 1. 直接插入排序 — 稳定
void InsertSort(SqList* L);

// 2. 折半插入排序 — 稳定
void BInsertSort(SqList* L);

// 3. 希尔排序 — 不稳定
void ShellSort(SqList* L);

// 4. 冒泡排序 — 稳定
void BubbleSort(SqList* L);

// 5. 快速排序 — 不稳定（核心 Partition）
int Partition(SqList* L, int low, int high);
void QSort(SqList* L, int low, int high);
void QuickSort(SqList* L);

// 6. 简单选择排序 — 不稳定
void SelectSort(SqList* L);

// 7. 堆排序 — 不稳定
void HeapAdjust(SqList* L, int s, int m);
void HeapSort(SqList* L);

// 8. 归并排序 — 稳定
void Merge(SqList* L, SqList* T, int low, int mid, int high);
void MSort(SqList* L, SqList* T, int low, int high);
void MergeSort(SqList* L);

// 9. 基数排序 — 稳定（LSD，最低位优先）
int GetDigit(int key, int radix, int k);
void RadixSort(SqList* L, int radix, int maxDigits);
