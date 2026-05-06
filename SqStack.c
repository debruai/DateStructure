#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define INFEASIBLE -1

// 第三章栈与队列------------栈的数据结构与算法实现
typedef int ElemType;

// 栈结构定义
typedef struct
{
    ElemType* base;  // 基地址，也叫栈底指针，初始化时指向分配的元素存储空间
    ElemType* top;   // 栈顶指针
    int size;        // 元素存储容量
} SqStack, * StackPtr;  // 这里使用SqStack作为结构体类型，StackPtr作为指针类型

// 初始化栈
StackPtr InitStack(int size)
{
    // 构造并返回一个空的顺序栈
    StackPtr S;

    // 分配栈结构体内存
    if (!(S = (StackPtr)malloc(sizeof(SqStack))))
    {
        exit(OVERFLOW);
    }

    // 分配元素存储空间
    if (!(S->base = (ElemType*)malloc(size * sizeof(ElemType))))
    {
        free(S);
        exit(OVERFLOW);
    }

    S->top = S->base;  // 栈里还没有元素，所以栈顶和栈底暂时重合
    S->size = size;    // 设置栈的容量

    return S;
}

// 获取栈顶元素
Status GetTop(StackPtr S, ElemType* e)
{
    // 若栈不为空，则返回栈顶元素，否则返回错误
    if (!S || S->top == S->base)
        return ERROR;

    *e = *(S->top - 1);
    return OK;
}

// 销毁栈
Status DestroyStack(StackPtr S)
{
    // 销毁栈的函数
    if (!S)
        return ERROR;

    if (S->base)
    {
        free(S->base);
        S->base = NULL;
    }

    free(S);
    return OK;
}

// 入栈
Status Push(StackPtr S, ElemType e)
{
    // 入栈，插入元素e，作为新的栈顶元素
    if (!S)
        return ERROR;

    // 如果需要扩容
    if (S->top - S->base >= S->size)
    {
        int new_size = S->size * 2;
        ElemType* new_base = (ElemType*)realloc(S->base, new_size * sizeof(ElemType));
        if (!new_base)
            return ERROR;

        S->base = new_base;
        S->top = S->base + S->size;  // 更新栈顶指针
        S->size = new_size;           // 更新容量
    }

    *(S->top) = e;  // 将元素放入栈顶
    S->top++;       // 栈顶指针上移

    return OK;
}

// 出栈
Status Pop(StackPtr S, ElemType* e)
{
    if (!S || S->top == S->base)
        return ERROR;

    S->top--;           // 栈顶指针下移
    *e = *(S->top);     // 获取原栈顶元素
    return OK;
}

// 判断栈是否为空
bool StackEmpty(StackPtr S)
{
    if (!S)
        return true;
    return S->top == S->base;
}

// 获取栈的当前长度
int StackLength(StackPtr S)
{
    if (!S)
        return 0;
    return S->top - S->base;
}

// 清空栈
Status ClearStack(StackPtr S)
{
    if (!S)
        return ERROR;

    S->top = S->base;  // 直接将栈顶指针指向栈底
    return OK;
}

// 数值转换，利用栈来实现
void Conversion(int N, int x)
{
    // 将非负十进制数N转为x进制，并且打印输出
    if (x < 2 || x > 16)
    {
        printf("进制必须在2-16之间\n");
        return;
    }

    if (N == 0)
    {
        printf("0\n");
        return;
    }

    StackPtr S = InitStack(10);
    int temp = N;

    while (temp > 0)
    {
        Push(S, temp % x);
        temp = temp / x;
    }

    printf("%d的%d进制是: ", N, x);

    ElemType digit;
    while (!StackEmpty(S))
    {
        Pop(S, &digit);
        if (digit < 10)
            printf("%d", digit);
        else
            printf("%c", 'A' + digit - 10);
    }
    printf("\n");

    DestroyStack(S);  // 用完要销毁栈
}

// 括号匹配检查
Status BracketMatch(const char* str)
{
    if (!str)
        return ERROR;

    StackPtr S = InitStack(10);
    int i = 0;
    ElemType ch, top_ch;

    while (str[i] != '\0')
    {
        ch = str[i];

        // 如果是左括号，入栈
        if (ch == '(' || ch == '[' || ch == '{')
        {
            Push(S, ch);
        }
        // 如果是右括号
        else if (ch == ')' || ch == ']' || ch == '}')
        {
            if (StackEmpty(S))
            {
                DestroyStack(S);
                return ERROR;  // 栈空，不匹配
            }

            GetTop(S, &top_ch);

            // 检查括号是否匹配
            if ((ch == ')' && top_ch == '(') ||
                (ch == ']' && top_ch == '[') ||
                (ch == '}' && top_ch == '{'))
            {
                Pop(S, &top_ch);
            }
            else
            {
                DestroyStack(S);
                return ERROR;  // 括号不匹配
            }
        }
        i++;
    }

    // 最后栈应该为空
    bool result = StackEmpty(S);
    DestroyStack(S);

    return result ? OK : ERROR;
}

// 打印栈
void PrintStack(StackPtr S)
{
    if (!S)
    {
        printf("栈不存在\n");
        return;
    }

    if (StackEmpty(S))
    {
        printf("空栈\n");
        return;
    }

    ElemType* p = S->base;
    printf("栈元素(从底到顶): ");
    while (p < S->top)
    {
        printf("%d ", *p);
        p++;
    }
    printf("\n");
}

// 测试函数
int main()
{
    printf("======= 顺序栈测试 =======\n");

    // 1. 初始化栈
    StackPtr S = InitStack(3);
    printf("1. 栈初始化成功\n");
    printf("   栈是否为空: %s\n", StackEmpty(S) ? "是" : "否");

    // 2. 入栈操作
    printf("\n2. 入栈操作:\n");
    for (int i = 1; i <= 5; i++)
    {
        if (Push(S, i * 10) == OK)
        {
            printf("   入栈元素: %d\n", i * 10);
        }
    }
    printf("   当前栈长度: %d\n", StackLength(S));
    PrintStack(S);

    // 3. 获取栈顶元素
    printf("\n3. 获取栈顶元素:\n");
    ElemType top_elem;
    if (GetTop(S, &top_elem) == OK)
    {
        printf("   栈顶元素: %d\n", top_elem);
    }

    // 4. 出栈操作
    printf("\n4. 出栈操作:\n");
    ElemType popped;
    for (int i = 0; i < 3; i++)
    {
        if (Pop(S, &popped) == OK)
        {
            printf("   出栈元素: %d\n", popped);
        }
    }
    printf("   当前栈长度: %d\n", StackLength(S));
    PrintStack(S);

    // 5. 再次入栈
    printf("\n5. 再次入栈操作:\n");
    Push(S, 100);
    Push(S, 200);
    printf("   当前栈长度: %d\n", StackLength(S));
    PrintStack(S);

    // 6. 清空栈
    printf("\n6. 清空栈:\n");
    if (ClearStack(S) == OK)
    {
        printf("   栈已清空\n");
    }
    printf("   清空后栈是否为空: %s\n", StackEmpty(S) ? "是" : "否");
    PrintStack(S);

    // 7. 释放栈
    printf("\n7. 释放栈:\n");
    if (DestroyStack(S) == OK)
    {
        printf("   栈释放成功\n");
    }

    // 8. 测试数值转换
    printf("\n8. 测试数值转换:\n");
    Conversion(100, 2);    // 100转二进制
    Conversion(100, 8);    // 100转八进制
    Conversion(100, 16);   // 100转十六进制
    Conversion(255, 16);   // 255转十六进制

    // 9. 测试括号匹配
    printf("\n9. 测试括号匹配:\n");
    const char* test_str1 = "({[]})";
    const char* test_str2 = "({[}])";
    const char* test_str3 = "((())";

    printf("   表达式 \"%s\" 括号匹配: %s\n",
        test_str1, BracketMatch(test_str1) == OK ? "成功" : "失败");
    printf("   表达式 \"%s\" 括号匹配: %s\n",
        test_str2, BracketMatch(test_str2) == OK ? "成功" : "失败");
    printf("   表达式 \"%s\" 括号匹配: %s\n",
        test_str3, BracketMatch(test_str3) == OK ? "成功" : "失败");

    printf("\n======= 测试完成 =======\n");

    return 0;
}