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
Status Visit(int e)
{
    printf("%d ", e);
    return TRUE;  // 返回TRUE表示继续遍历
}

typedef struct LNode
{
    // 定义链表结点的结构体
    int data;               // 数据域
    struct LNode* next;     // 指针域，一个指向下一结点结构体的指针
} LNode, * LinkList;

LinkList InitList()
{   // 创建一个新链表的函数
    LinkList L;
    if (!(L = (LNode*)calloc(1, sizeof(LNode))))
    {
        exit(OVERFLOW);
    }
    L->next = NULL;  // 头结点的next初始化为NULL
    return L;        // 返回生成的头结点指针
}

//----------------------------------算法2.8---L为头结点指针的单链表，若第i个元素存在则返回其值，否则返回ERROR
int GetElem(LinkList L, int i)
{
    int count = 0;
    if (!L || i < 1) return ERROR;  // 如果L为空指针或者要找的元素序号小于一就有问题，不找了
    LNode* p = L->next;
    count = 1;                      // count代表现在所处的元素序号
    while (p && count < i)         // 当当前结点存在并且序号还小于i时循环继续
    {
        p = p->next;
        count++;
    }
    if (!p) return ERROR;
    return p->data;
}

//----------------------------------算法2.9---在带头结点的单链表L中第i个元素之前插入元素e
Status ListInsert(LinkList L, int i, int e)
{
    if (!L || i < 1) return ERROR;  // L不存在或者i不合法就报错
    LNode* s, * p = L;
    int count = 0;                  // 指向结点结构体的指针s和p暂时都是头结点指针，再生成计数器count

    while (p && count < i - 1)      // 注意是插在i号之前，所以循环结束的时候p应该指向i-1号结点,同时每次循环要保证p指针不为空，结点存在
    {
        p = p->next;
        ++count;
    }
    // 循环结束
    if (!p) return ERROR;

    // 结点指针S用于生成新的结点
    if (!(s = (LNode*)malloc(sizeof(LNode)))) return OVERFLOW;
    s->data = e;
    s->next = p->next;
    p->next = s;
    return OK;
}

//-----------------------------------算法2.10---在带头结点的单链表L中，删除第i个元素，并返回其值
int ListDelete(LinkList L, int i)
{
    int count = 0;
    if (!L || i < 1) return ERROR;
    LNode* q, * p = L;

    while (p->next && count < i - 1)
    {
        p = p->next;
        count++;
    }  // 循环结束时让p指向i-1结点

    if (!p->next) return ERROR;  // p后面没节点了就错误

    q = p->next;
    p->next = q->next;
    int e = q->data;
    free(q);                     // 释放结点
    return e;
}

//-----------------------------------算法2.11---由s的前n个元素构建并且返回带头结点的单链表
LinkList CreateList(int n, int s[])
{
    if (!s || n <= 0) return NULL;  // 参数不合法

    LinkList L, p;
    if (!(L = p = (LNode*)calloc(1, sizeof(LNode)))) exit(OVERFLOW);  // 分配头结点，L和p都指向头，p等会就挪动

    for (int i = 0; i < n; i++)
    {
        if (!(p->next = (LNode*)calloc(1, sizeof(LNode)))) exit(OVERFLOW);
        p = p->next;       // p指向新的链表尾
        p->data = s[i];    // 赋值
    }
    p->next = NULL;        // 最后一个结点的next置为NULL

    return L;              // 记得返回链表的头结点指针
}

//----------------------------------算法2.12---已知单链表La和Lb元素非递减排列,归并他们并返回结果链表Lc，也非递减排列
LinkList MergeList(LinkList La, LinkList Lb)
{
    if (!La || !Lb) return NULL;  // 传入的链表必须合法有效

    LNode* Lc, * pc, * pa = La->next, * pb = Lb->next;  // pa和pb分别指向各自链表的当前比较元素
    Lc = pc = La;  // 结果链表的表头就是a的表头

    while (pa && pb)  // 两个表当前都有结点可以比较才能继续循环
    {
        if (pa->data <= pb->data)  // 这种情况要把结点a作为下一个结点
        {
            pc->next = pa;
            pc = pa;
            pa = pa->next;
        }
        else
        {
            pc->next = pb;
            pc = pb;
            pb = pb->next;
        }
    }

    // 循环结束看是哪个链表没比较完，直接接到后面
    pc->next = pa ? pa : pb;
    free(Lb);  // 释放Lb的头结点
    return Lc; // 返回Lc
}

//-----------------------------------算法2.13---遍历带头结点的单链表L的每个元素
Status ListTraverse(LinkList L, Status(*Visit)(int))
{
    if (!L) return ERROR;

    LNode* p = L->next;  // 从头结点的下一个开始遍历
    while (p && Visit(p->data))
    {
        p = p->next;
    }

    if (p) return ERROR;  // 此时p不为空是因为Visit报错结束了循环
    return OK;
}

// 获取链表长度
int ListLength(LinkList L)
{
    if (!L) return 0;

    int length = 0;
    LNode* p = L->next;

    while (p)
    {
        length++;
        p = p->next;
    }

    return length;
}

// 清空链表
Status ClearList(LinkList L)
{
    if (!L) return ERROR;

    LNode* p = L->next;
    LNode* q;

    while (p)
    {
        q = p;
        p = p->next;
        free(q);
    }

    L->next = NULL;
    return OK;
}

// 销毁链表
Status DestroyList(LinkList L)
{
    if (!L) return ERROR;

    ClearList(L);  // 先清空链表
    free(L);       // 再释放头结点

    return OK;
}

// 判断链表是否为空
Status ListEmpty(LinkList L)
{
    if (!L) return TRUE;
    return (L->next == NULL) ? TRUE : FALSE;
}

// 获取指定元素的前驱
Status PriorElem(LinkList L, int cur_e, int* pre_e)
{
    if (!L || !L->next) return ERROR;  // 链表为空或只有一个元素

    LNode* p = L->next;
    LNode* prev = L;

    while (p && p->data != cur_e)
    {
        prev = p;
        p = p->next;
    }

    if (!p || prev == L) return ERROR;  // 没找到cur_e，或者cur_e是第一个元素

    *pre_e = prev->data;
    return OK;
}

// 获取指定元素的后继
Status NextElem(LinkList L, int cur_e, int* next_e)
{
    if (!L || !L->next) return ERROR;

    LNode* p = L->next;

    while (p && p->data != cur_e)
    {
        p = p->next;
    }

    if (!p || !p->next) return ERROR;  // 没找到cur_e，或者cur_e是最后一个元素

    *next_e = p->next->data;
    return OK;
}

// 查找元素位置
int LocateElem(LinkList L, int e)
{
    if (!L) return 0;

    int pos = 1;
    LNode* p = L->next;

    while (p && p->data != e)
    {
        p = p->next;
        pos++;
    }

    return p ? pos : 0;  // 找到返回位置，没找到返回0
}

// 反转链表
Status ReverseList(LinkList L)
{
    if (!L || !L->next) return ERROR;  // 链表为空或只有一个元素

    LNode* prev = NULL;
    LNode* curr = L->next;
    LNode* next = NULL;

    while (curr)
    {
        next = curr->next;  // 保存下一个节点
        curr->next = prev;  // 反转指针
        prev = curr;        // 移动prev
        curr = next;        // 移动curr
    }

    L->next = prev;  // 头结点指向反转后的第一个节点
    return OK;
}

// 打印链表
void PrintList(LinkList L)
{
    if (!L)
    {
        printf("链表不存在\n");
        return;
    }

    if (!L->next)
    {
        printf("空链表\n");
        return;
    }

    LNode* p = L->next;
    printf("链表元素: ");
    while (p)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

int main()
{
    printf("======= 单链表测试 =======\n");

    // 1. 初始化链表
    LinkList L = InitList();
    printf("1. 链表初始化成功\n");
    printf("   链表是否为空: %s\n", ListEmpty(L) ? "是" : "否");

    // 2. 插入元素
    printf("\n2. 插入元素测试:\n");
    for (int i = 1; i <= 5; i++)
    {
        if (ListInsert(L, i, i * 10) == OK)
        {
            printf("   在第%d个位置插入元素: %d\n", i, i * 10);
        }
    }
    printf("   当前链表长度: %d\n", ListLength(L));
    PrintList(L);

    // 3. 遍历链表
    printf("\n3. 遍历链表:\n");
    printf("   遍历结果: ");
    ListTraverse(L, Visit);
    printf("\n");

    // 4. 获取元素
    printf("\n4. 获取元素测试:\n");
    for (int i = 1; i <= 6; i++)
    {
        int elem = GetElem(L, i);
        if (elem != ERROR)
        {
            printf("   第%d个元素: %d\n", i, elem);
        }
        else
        {
            printf("   第%d个元素不存在\n", i);
        }
    }

    // 5. 查找元素
    printf("\n5. 查找元素测试:\n");
    int pos = LocateElem(L, 30);
    if (pos > 0)
    {
        printf("   元素30在链表中的位置: %d\n", pos);
    }

    // 6. 删除元素
    printf("\n6. 删除元素测试:\n");
    int deleted = ListDelete(L, 3);
    if (deleted != ERROR)
    {
        printf("   删除第3个元素: %d\n", deleted);
    }
    printf("   删除后链表: ");
    PrintList(L);

    // 7. 前驱和后继
    printf("\n7. 前驱和后继测试:\n");
    int pre_e, next_e;
    if (PriorElem(L, 40, &pre_e) == OK)
    {
        printf("   元素40的前驱是: %d\n", pre_e);
    }
    if (NextElem(L, 20, &next_e) == OK)
    {
        printf("   元素20的后继是: %d\n", next_e);
    }

    // 8. 反转链表
    printf("\n8. 反转链表:\n");
    if (ReverseList(L) == OK)
    {
        printf("   反转后链表: ");
        PrintList(L);
    }

    // 9. 测试CreateList
    printf("\n9. 测试CreateList函数:\n");
    int arr[] = { 100, 200, 300, 400 };
    LinkList L2 = CreateList(4, arr);
    printf("   新创建的链表: ");
    PrintList(L2);

    // 10. 测试MergeList
    printf("\n10. 测试MergeList函数:\n");
    int arr1[] = { 1, 3, 5, 7 };
    int arr2[] = { 2, 4, 6, 8 };
    LinkList La = CreateList(4, arr1);
    LinkList Lb = CreateList(4, arr2);
    printf("   链表La: ");
    PrintList(La);
    printf("   链表Lb: ");
    PrintList(Lb);

    LinkList Lc = MergeList(La, Lb);
    printf("   合并后链表: ");
    PrintList(Lc);

    // 11. 清空和销毁链表
    printf("\n11. 清空和销毁链表:\n");
    if (ClearList(L) == OK)
    {
        printf("   链表L已清空，现在是否为空: %s\n", ListEmpty(L) ? "是" : "否");
    }

    if (DestroyList(L) == OK && DestroyList(L2) == OK && DestroyList(Lc) == OK)
    {
        printf("   所有链表已销毁\n");
    }

    printf("\n======= 测试完成 =======\n");

    return 0;
}