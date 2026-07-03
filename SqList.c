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
void PrintList(SqList* L)
{
    if (!L || L->len == 0)
    {
        printf("空表\n");
        return;
    }
    printf("顺序表元素: ");
    for (int i = 0; i < L->len; i++)
    {
        printf("%d ", L->elem[i]);
    }
    printf(" [长度: %d, 容量: %d]\n", L->len, L->size);
}

//------------------------------算法2.3.1---实现顺序表的初始化操作
SqList* InitList(int size, int inc)  // 返回类型：顺序表类型的指针
{
    SqList* L;  // 声明顺序表指针
    if (!(L = (SqList*)malloc(sizeof(SqList))))
    {
        exit(OVERFLOW);  // 首先确认内存是否分配出来了,如果没有就返回错误
    }

    L->elem = (int*)malloc(size * sizeof(int));  // malloc分配size个存储空间
    if (!L->elem)
    {
        free(L);
        exit(OVERFLOW);  // 分配元素空间失败就要报错
    }

    L->len = 0;     // 只是分配了空间，还没有存元素所以len为0
    L->inc = inc;   // 顺序表每次扩容的增量在初始化时就指定好
    L->size = size;

    return L;       // 返回指向顺序表的指针
}

//-----------------------------算法2.3.2---实现顺序表的回收操作
void FreeList(SqList* L)
{
    // 传入指向顺序表的指针L，实现顺序表的完全释放
    if (L)  // 如果指针不是空指针NULL
    {
        if (L->elem)
        {
            free(L->elem);  // 先把存储元素的地址都释放了
        }
        free(L);            // 再把存储结构体本身的地址释放
    }
}

//------------------------------算法2.3.3---实现顺序表两个元素的比较
Status equal_int(int a, int b)
{
    return a == b;
}

//------------------------------算法2.4---在顺序表L的第i个位置插入新的元素e
Status ListInsert(SqList* L, int i, int e)
{
    // 在顺序表L中第i个位置插入新的元素e, 1<=i<=L->len+1
    if (!L || i < 1 || i > L->len + 1)
        return ERROR;  // 如果参数值不合法或者顺序表L错误就直接返回false

    if (L->len >= L->size)  // 如果顺序表长度已满，需要继续分配
    {
        int* newbase = (int*)realloc(L->elem, (L->size + L->inc) * sizeof(int));
        if (!newbase)
            return ERROR;

        L->elem = newbase;
        L->size += L->inc;
    }
    
    // 从第i-1个位置开始，后面的元素全部后移一位
    for (int j = L->len; j >= i; j--)
    {
        L->elem[j] = L->elem[j - 1];
    }

    L->elem[i - 1] = e;  // 在第i个位置插入新元素
    L->len++;            // 长度加1

    return OK;
}

//-------------------------------算法2.5---在顺序表L中删除第i个元素并且返回其值，否则返回一个错误代码
Status ListDelete(SqList* L, int i, int* e)
{
    // 先检测是否合法值
    if (!L || i < 1 || i > L->len)
        return ERROR;

    *e = L->elem[i - 1];  // 保存要删除的元素值

    // 从第i个元素开始，后面的元素全部前移一位
    for (int j = i; j < L->len; j++)
    {
        L->elem[j - 1] = L->elem[j];
    }

    L->len--;  // 长度减1
    return OK;
}

//------------------------------算法2.6---在顺序表L中查找并返回第1个与e值满足compare()的元素的位序，找不到则返回0
int LocateElem_Sq(SqList* L, int e, Status(*compare)(int, int))
{
    if (!L)
        return 0;

    for (int i = 0; i < L->len; i++)
    {
        if (compare(L->elem[i], e))
        {
            return i + 1;  // 返回位序（从1开始）
        }
    }

    return 0;  // 没找到返回0
}

//-------------------------------算法2.7归并两个已经有序的顺序表，全部按非递减排序
SqList* MergeList_Sq(SqList* La, SqList* Lb)
{
    if (!La || !Lb)
        return NULL;  // 两个传入的顺序表如果有空的，直接返回空的顺序表指针NULL

    // 创建结果顺序表
    SqList* Lc = InitList(La->len + Lb->len, La->inc);
    if (!Lc)
        return NULL;

    int i = 0, j = 0, k = 0;

    // 归并两个有序顺序表
    while (i < La->len && j < Lb->len)
    {
        if (La->elem[i] <= Lb->elem[j])
        {
            Lc->elem[k++] = La->elem[i++];
        }
        else
        {
            Lc->elem[k++] = Lb->elem[j++];
        }
    }

    // 将剩余元素复制到Lc
    while (i < La->len)
    {
        Lc->elem[k++] = La->elem[i++];
    }

    while (j < Lb->len)
    {
        Lc->elem[k++] = Lb->elem[j++];
    }

    Lc->len = k;  // 设置Lc的长度

    return Lc;
}

// 获取顺序表长度
int ListLength(SqList* L)
{
    if (!L)
        return 0;
    return L->len;
}

// 判断顺序表是否为空
Status ListEmpty(SqList* L)
{
    if (!L)
        return TRUE;
    return (L->len == 0) ? TRUE : FALSE;
}

// 获取指定位置的元素
Status GetElem(SqList* L, int i, int* e)
{
    if (!L || i < 1 || i > L->len)
        return ERROR;

    *e = L->elem[i - 1];
    return OK;
}

// 修改指定位置的元素
Status SetElem(SqList* L, int i, int e)
{
    if (!L || i < 1 || i > L->len)
        return ERROR;

    L->elem[i - 1] = e;
    return OK;
}

// 清空顺序表
Status ClearList(SqList* L)
{
    if (!L)
        return ERROR;

    L->len = 0;
    return OK;
}

// 查找元素的前驱
Status PriorElem(SqList* L, int cur_e, int* pre_e)
{
    if (!L || L->len < 2)
        return ERROR;

    for (int i = 1; i < L->len; i++)
    {
        if (L->elem[i] == cur_e)
        {
            *pre_e = L->elem[i - 1];
            return OK;
        }
    }

    return ERROR;  // 没找到cur_e，或者cur_e是第一个元素
}

// 查找元素的后继
Status NextElem(SqList* L, int cur_e, int* next_e)
{
    if (!L || L->len < 2)
        return ERROR;

    for (int i = 0; i < L->len - 1; i++)
    {
        if (L->elem[i] == cur_e)
        {
            *next_e = L->elem[i + 1];
            return OK;
        }
    }

    return ERROR;  // 没找到cur_e，或者cur_e是最后一个元素
}

// 测试主函数
int main()
{
    printf("======= 顺序表测试 =======\n");

    // 1. 初始化顺序表
    SqList* La = InitList(5, 5);
    printf("1. 顺序表La初始化成功\n");
    printf("   初始状态: ");
    PrintList(La);

    // 2. 插入元素
    printf("\n2. 插入元素测试:\n");
    for (int i = 1; i <= 8; i++)
    {
        if (ListInsert(La, i, i * 10) == OK)
        {
            printf("   在第%d个位置插入元素: %d\n", i, i * 10);
        }
    }
    printf("   当前顺序表: ");
    PrintList(La);

    // 3. 获取元素
    printf("\n3. 获取元素测试:\n");
    int elem;
    for (int i = 1; i <= 3; i++)
    {
        if (GetElem(La, i, &elem) == OK)
        {
            printf("   第%d个元素: %d\n", i, elem);
        }
    }

    // 4. 查找元素
    printf("\n4. 查找元素测试:\n");
    int pos = LocateElem_Sq(La, 30, equal_int);
    if (pos > 0)
    {
        printf("   元素30在顺序表中的位置: %d\n", pos);
    }

    // 5. 前驱和后继
    printf("\n5. 前驱和后继测试:\n");
    int pre, next;
    if (PriorElem(La, 30, &pre) == OK)
    {
        printf("   元素30的前驱是: %d\n", pre);
    }
    if (NextElem(La, 30, &next) == OK)
    {
        printf("   元素30的后继是: %d\n", next);
    }

    // 6. 修改元素
    printf("\n6. 修改元素测试:\n");
    if (SetElem(La, 3, 99) == OK)
    {
        printf("   将第3个元素修改为99\n");
    }
    printf("   修改后顺序表: ");
    PrintList(La);

    // 7. 删除元素
    printf("\n7. 删除元素测试:\n");
    int deleted;
    if (ListDelete(La, 3, &deleted) == OK)
    {
        printf("   删除第3个元素: %d\n", deleted);
    }
    printf("   删除后顺序表: ");
    PrintList(La);

    // 8. 创建另一个有序顺序表
    printf("\n8. 创建有序顺序表Lb:\n");
    SqList* Lb = InitList(5, 5);
    int arr[] = { 15, 25, 35, 45 };
    for (int i = 0; i < 4; i++)
    {
        ListInsert(Lb, i + 1, arr[i]);
    }
    printf("   顺序表Lb: ");
    PrintList(Lb);

    // 9. 归并两个有序顺序表
    printf("\n9. 归并两个有序顺序表:\n");
    SqList* Lc = MergeList_Sq(La, Lb);
    if (Lc)
    {
        printf("   归并结果Lc: ");
        PrintList(Lc);
    }

    // 10. 清空顺序表
    printf("\n10. 清空顺序表测试:\n");
    if (ClearList(Lc) == OK)
    {
        printf("   顺序表Lc已清空\n");
        printf("   清空后Lc: ");
        PrintList(Lc);
    }

    // 11. 释放所有顺序表
    printf("\n11. 释放所有顺序表:\n");
    FreeList(La);
    FreeList(Lb);
    FreeList(Lc);
    printf("   所有顺序表已释放\n");

    printf("\n======= 测试完成 =======\n");

    return 0;
}