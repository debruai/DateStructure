#include <stdio.h>
#include <stdlib.h>

typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define INFEASIBLE -1

// 双向链表学习
typedef struct DuLNode
{
    int data;                // 数据域
    struct DuLNode* prev;    // 向前指针
    struct DuLNode* next;    // 向后指针
} DuLNode, * DuLinkList;     // 结点类型，双向链表指针类型

// 初始化循环双向链表
DuLinkList InitDuLinkList()
{
    DuLinkList L = (DuLinkList)malloc(sizeof(DuLNode));
    if (!L) return NULL;

    L->prev = L;  // 前驱指向自己
    L->next = L;  // 后继指向自己
    L->data = 0;  // 可以存储长度或其他信息

    return L;
}

// 查找第i个元素（i从1开始，返回对应结点指针）
DuLinkList GetElemPos(DuLinkList L, int i)
{
    if (!L || i < 1) return NULL;  // i从1开始

    DuLinkList p = L->next;  // 从第一个实际结点开始
    int count = 1;

    while (p != L && count < i)  // 循环到找到第i个结点或回到头结点
    {
        p = p->next;
        count++;
    }

    if (p == L || count < i) return NULL;  // 第i个结点不存在
    return p;
}

// 在第i个位置之前插入元素e
Status ListInsert(DuLinkList L, int i, int e)
{
    if (!L || i < 1) return ERROR;

    // 找到第i-1个结点
    DuLinkList p = L;  // 从头结点开始
    int j = 0;

    // 定位到第i-1个位置
    while (p->next != L && j < i - 1)
    {
        p = p->next;
        j++;
    }

    if (p->next == L && j < i - 1) return ERROR;  // i超出范围

    // 创建新结点
    DuLinkList s = (DuLinkList)malloc(sizeof(DuLNode));
    if (!s) return ERROR;

    s->data = e;

    // 插入新结点
    s->prev = p;
    s->next = p->next;
    p->next->prev = s;
    p->next = s;

    return OK;
}

// 删除第i个元素，返回其值
int DulLinkListDelete(DuLinkList L, int i)
{
    if (!L || i < 1) return ERROR;

    DuLinkList p = L->next;  // 从头结点的下一个开始
    int j = 1;

    // 定位到第i个结点
    while (p != L && j < i)
    {
        p = p->next;
        j++;
    }

    if (p == L || j < i) return ERROR;  // 第i个结点不存在

    int e = p->data;

    // 调整指针
    p->prev->next = p->next;
    p->next->prev = p->prev;

    free(p);
    return e;
}

// 打印链表
void PrintDuLinkList(DuLinkList L)
{
    if (!L)
    {
        printf("链表不存在\n");
        return;
    }

    if (L->next == L)
    {
        printf("空链表\n");
        return;
    }

    DuLinkList p = L->next;
    printf("双向链表元素: ");
    while (p != L)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

// 反向打印链表
void PrintDuLinkListReverse(DuLinkList L)
{
    if (!L || L->prev == L)
    {
        printf("空链表\n");
        return;
    }

    DuLinkList p = L->prev;  // 从最后一个结点开始
    printf("反向打印: ");
    while (p != L)
    {
        printf("%d ", p->data);
        p = p->prev;
    }
    printf("\n");
}

// 获取链表长度
int GetLength(DuLinkList L)
{
    if (!L) return 0;

    int length = 0;
    DuLinkList p = L->next;

    while (p != L)
    {
        length++;
        p = p->next;
    }

    return length;
}

// 释放链表
Status FreeDuLinkList(DuLinkList L)
{
    if (!L) return ERROR;

    DuLinkList p = L->next;
    DuLinkList q;

    // 释放所有结点
    while (p != L)
    {
        q = p;
        p = p->next;
        free(q);
    }

    // 释放头结点
    free(L);

    return OK;
}

int main()
{
    printf("======= 循环双向链表测试 =======\n");

    // 1. 初始化链表
    DuLinkList L = InitDuLinkList();
    if (!L)
    {
        printf("初始化失败\n");
        return 1;
    }
    printf("1. 链表初始化成功\n");

    // 2. 插入元素
    printf("\n2. 插入元素测试:\n");
    for (int i = 1; i <= 5; i++)
    {
        if (ListInsert(L, i, i * 10))  
        {
            printf("   在第%d个位置插入元素: %d\n", i, i * 10);
        }
        else
        {
            printf("   在第%d个位置插入失败\n", i);
        }
    }
    printf("   当前链表长度: %d\n", GetLength(L));
    PrintDuLinkList(L); 
    PrintDuLinkListReverse(L); 

    // 3. 查找元素
    printf("\n3. 查找元素测试:\n");
    for (int i = 1; i <= 6; i++)
    {
        DuLinkList p = GetElemPos(L, i); 
        if (p)
        {
            printf("   第%d个元素是: %d\n", i, p->data);
        }
        else
        {
            printf("   第%d个元素不存在\n", i);
        }
    }

    // 4. 删除元素
    printf("\n4. 删除元素测试:\n");
    int deleted = DulLinkListDelete(L, 3);  
    if (deleted != ERROR) 
    {
        printf("   删除第3个元素: %d\n", deleted);
    }
    else
    {
        printf("   删除第3个元素失败\n");
    }
    printf("   删除后链表: ");
    PrintDuLinkList(L);  

    // 5. 在中间插入元素
    printf("\n5. 在中间插入元素:\n");
    if (ListInsert(L, 3, 99)) 
    {
        printf("   在第3个位置插入元素: 99\n");
    }
    printf("   插入后链表: ");
    PrintDuLinkList(L); 

    // 6. 删除头尾元素
    printf("\n6. 删除头尾元素:\n");
    int first = DulLinkListDelete(L, 1);
    if (first != ERROR)  
    {
        printf("   删除第1个元素: %d\n", first);
    }

    int last = DulLinkListDelete(L, GetLength(L)); 
    if (last != ERROR) 
    {
        printf("   删除最后1个元素: %d\n", last);
    }
    printf("   删除后链表: ");
    PrintDuLinkList(L);  

    // 7. 释放链表
    printf("\n7. 释放链表:\n");
    if (FreeDuLinkList(L) == OK)  
    {
        printf("   链表释放成功\n");
    }
    L = NULL;  // 修正：NULL 而不是 null

    printf("\n======= 测试完成 =======\n");

    return 0;
}