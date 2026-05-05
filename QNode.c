#include <stdio.h>
#include <stdlib.h>
typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define INFEASIBLE -1
typedef int Elemtype;

// 数据结构队列，链式实现

// 结构1，队列结点
typedef struct QNode
{
    Elemtype data;          // 数据域
    struct QNode* next;     // 指针域，这是一个指向下一个结点结构体的指针
} QNode, * QueuePtr;

// 结构2，队列本身
typedef struct
{
    QueuePtr front;  // 队头指针
    QueuePtr rear;   // 队尾指针
} LinkQueue;

// 基本算法实现-------------------------------------------------------------------------------------------------------------------------------
LinkQueue* initQueue()
{
    // 初始化一个队列的函数，返回一个队列指针
    LinkQueue* Q;
    if (!(Q = (LinkQueue*)malloc(sizeof(LinkQueue)))) exit(OVERFLOW);  // 创建队列结构体

    if (!(Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode))))  // 分配头结点的存储空间
    {
        free(Q);
        exit(OVERFLOW);
    }

    Q->front->next = NULL;  // 空队列的头指针和尾指针都指向队头
    return Q;
}

Status FreeQueueL(LinkQueue* Q)
{
    // 回收队列Q的函数
    if (!Q) return ERROR;  // 如果没有队列指针Q就不能释放

    QueuePtr p = Q->front;  // 指向头结点
    QueuePtr q;

    while (p)
    {
        q = p;
        p = p->next;
        free(q);
    }

    free(Q);
    return OK;
}

Status ClearQueueL(LinkQueue* Q)
{
    // 清空队列的函数，将Q置为空队列，有头结点
    if (!Q || Q->front == Q->rear) return ERROR;  // 如果Q不存在或者本来就是空队列

    QueuePtr p = Q->front->next;  // 指向第一个有效结点
    QueuePtr q;

    while (p)
    {
        q = p;
        p = p->next;
        free(q);
    }

    Q->rear = Q->front;   // 重置队尾指针指向头结点
    Q->front->next = NULL;  // 头结点的next置为NULL
    return OK;
}

int QueueLenL(LinkQueue* Q)
{
    int length = 0;
    if (!Q || Q->front == Q->rear) return 0;

    QueuePtr p = Q->front->next;  // 从第一个有效结点开始计数
    while (p)
    {
        length++;
        p = p->next;
    }

    return length;
}

Status EnQueue(LinkQueue* Q, Elemtype newdata)
{
    // 入队列的函数
    if (!Q) return ERROR;

    QueuePtr p;
    if (!(p = (QueuePtr)malloc(sizeof(QNode)))) return ERROR;  // 创建新的结点并且分配内存空间

    p->data = newdata;
    p->next = NULL;

    Q->rear->next = p;  // 将新结点连接到队尾
    Q->rear = p;        // 更新队尾指针

    return OK;
}

Status DeQueueL(LinkQueue* Q, Elemtype* e)
{
    // 出队列的函数
    if (!Q || Q->front == Q->rear) return ERROR;  // 队列为空

    QueuePtr p = Q->front->next;  // 临时结点指针目前指向队头结点的下一个结点
    *e = p->data;                 // 保存要删除的结点数据

    Q->front->next = p->next;     // 修改头结点的next指针

    // 如果被删除的结点是最后一个结点，那就让尾指针指回头结点
    if (Q->rear == p)
    {
        Q->rear = Q->front;
    }

    free(p);
    return OK;
}

Elemtype GetHeadL(LinkQueue* Q)
{
    // 若队列不为空，返回队头元素，否则返回ERROR
    if (!Q || Q->front == Q->rear) return ERROR;

    QueuePtr p = Q->front->next;
    return p->data;
}

Status PrintQueue(LinkQueue* Q)
{
    // 打印队列所有元素
    if (!Q || Q->front == Q->rear)
    {
        printf("队列为空\n");
        return ERROR;
    }

    QueuePtr p = Q->front->next;
    printf("队列元素: ");
    while (p)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");

    return OK;
}

// 测试主函数
int main()
{
    printf("======= 链式队列测试 =======\n");

    // 1. 初始化队列
    LinkQueue* Q = initQueue();
    printf("1. 队列初始化成功\n");
    printf("   当前队列长度: %d\n", QueueLenL(Q));

    // 2. 入队操作
    printf("\n2. 入队操作:\n");
    for (int i = 1; i <= 5; i++)
    {
        EnQueue(Q, i * 10);
        printf("   入队元素: %d\n", i * 10);
    }
    printf("   当前队列长度: %d\n", QueueLenL(Q));
    PrintQueue(Q);

    // 3. 获取队头元素
    printf("\n3. 获取队头元素:\n");
    printf("   队头元素: %d\n", GetHeadL(Q));

    // 4. 出队操作
    printf("\n4. 出队操作:\n");
    Elemtype e;
    for (int i = 0; i < 3; i++)
    {
        if (DeQueueL(Q, &e) == OK)
        {
            printf("   出队元素: %d\n", e);
        }
    }
    printf("   当前队列长度: %d\n", QueueLenL(Q));
    PrintQueue(Q);

    // 5. 再次入队
    printf("\n5. 再次入队操作:\n");
    EnQueue(Q, 100);
    EnQueue(Q, 200);
    printf("   当前队列长度: %d\n", QueueLenL(Q));
    PrintQueue(Q);

    // 6. 清空队列
    printf("\n6. 清空队列:\n");
    ClearQueueL(Q);
    printf("   清空后队列长度: %d\n", QueueLenL(Q));
    PrintQueue(Q);

    // 7. 再次测试
    printf("\n7. 清空后再次入队:\n");
    EnQueue(Q, 999);
    EnQueue(Q, 888);
    printf("   当前队列长度: %d\n", QueueLenL(Q));
    PrintQueue(Q);

    // 8. 释放队列
    printf("\n8. 释放队列:\n");
    if (FreeQueueL(Q) == OK)
    {
        printf("   队列释放成功\n");
    }

    printf("======= 测试完成 =======\n");

    return 0;
}