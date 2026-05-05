#include<stdio.h>
#include<stdlib.h>
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
	//定义链表结点的结构体
	int data;//数据域
	struct LNode* next;//指针域，一个指向下一结点结构体的指针
}LNode,*Linklist;


Linklist InitList()
{//创建一个新链表的函数
	Linklist L;
	if (!(L = (LNode*)calloc(1, sizeof(LNode))))
	{
		exit(OVERFLOW);
	}
	return L;//返回生成的头结点指针
}

//----------------------------------算法2.8---L为头结点指针的单链表，若第i个元素存在则返回其值，否则返回ERROR
int GetElem(Linklist L, int i)
{
	int count = 0;
	if (!L || i < 1) return ERROR;//如果L为空指针或者要找的元素序号小于一就有问题，不找了
	LNode* p = L->next; count++;//count代表现在所处的元素序号
	while (p && count < i)//当当前结点存在并且序号还小于i时循环继续
	{
		p = p->next;
		count++;
	}
	if (!p) return ERROR;
	return p->data;
}

//----------------------------------算法2.9---在带头结点的单链表L中第i个元素之前插入元素e
Status ListInsert(Linklist L, int i, int e)
{
	if (!L || i < 1) return ERROR;//L不存在或者i不合法就报错
	LNode* s, * p = L; int count = 0;//指向结点结构体的指针s和p暂时都是头结点指针，再生成计数器count
	while (p&&count<i-1)//注意是插在i号之前，所以循环结束的时候p应该指向i-1号结点,同时每次循环要保证p指针不为空，结点存在
	{
		p = p->next;
		++count;
	}
	//循环结束
	if (!p) return ERROR;
	//结点指针S用于生成新的结点
	if (!(s = (Linklist)malloc(sizeof(LNode)))) exit(OVERFLOW);
	s->data = e;
	s->next = p->next;
	p->next = s;
	return OK;
}

//-----------------------------------算法2.10---在带头结点的单链表L中，删除第i个元素，并返回其值
int ListDelete(Linklist L, int i)
{
	int count = 0;
	if (!L || i < 1) return ERROR;
	LNode* q, * p = L;
	while (p->next && count < i - 1) { p = p->next; count++; }//循环结束时让p指向i-1结点
	if (!p->next) return ERROR;//p后面没节点了就错误
	q = p->next; p->next = q->next;
	int e = q->data;
	free(q);//释放结点
	return e;
}

//-----------------------------------算法2.11---由s的前n个元素构建并且返回带头结点的单链表
Linklist CreateList(int n, int s[])
{
	if (!s || !s) return NULL;//参数不合法
	Linklist L, p;
	if (!(L = p = (LNode*)calloc(1, sizeof(LNode)))) exit(OVERFLOW);//分配头结点，L和p都指向头，p等会就挪动
	for (int i = 0; i < n; i++)
	{
		if (!(p->next = (LNode*)calloc(1, sizeof(LNode)))) exit(OVERFLOW);
		p = p->next;//p指向新的链表尾
		p->data = s[i];//赋值
	}
	return L;//记得返回链表的头结点指针
}

//----------------------------------算法2.12---已知单链表La和Lb元素非递减排列,归并他们并返回结果链表Lc，也非递减排列
Linklist MergeList(Linklist La, Linklist Lb)
{
	if (!La || !Lb) return NULL;//传入的链表必须合法有效
	LNode* Lc, * pc, * pa = La->next, * pb = Lb->next;//pa和pb分别指向各自链表的当前比较元素
	Lc = pc = La;//结果链表的表头就是a的表头
	while (pa && pb)//两个表当前都有结点可以比较才能继续循环
	{
		if (pa->data <= pb->data)//这种情况要把结点a作为下一个结点
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
	//循环结束看是哪个链表没比较完，直接接到后面
	pc->next = pa ? pa : pb;
	free(Lb); return Lc;//释放Lb的头结点，返回Lc
}

//-----------------------------------算法2.13---遍历带头结点的单链表L的每个元素
Status ListTraverse(Linklist L,Status (*Vistit)(int) )
{
	if (!L) return ERROR;
	L = L->next;
	while (L && Vistit(L->data)) L = L->next;
	if (L) return ERROR;//此时L不为空是因为Visit报错结束了循环
	return OK;
}


//单链表实现一员多项式的相加

//定义一元稀疏多项式的抽象类型
typedef struct ploy//项
{
	int exp;//这一项的幂
	int coef;
	struct poly* next;
}Term,*TermList;

TermList  add_poly(TermList  Ah, TermList  Bh)
{
	//核心函数，实现两个一员多项式的相加
	Term* pa, * pb, * r ;//创建指向一个项结构体的指针们
	pa = Ah->next; pb =Bh->next;//pa和pb指向各自多项式的当前项
	r = Ah;//r指向结果链表的尾结点
	while (pa && pb)//两个链表都还没处理完就循环继续
	{
		if (pa->exp > pb->exp)//如果pa幂高于pb的幂，pb加入结果链表，然后往后
		{
			r->next = pb;
			r = pb;
			pb = pb->next;
		}
		else if(pa->exp<pb->exp)
		{
			r->next = pa;
			r = pa;
			pa = pa->next;
		}
		else//两个项的幂相等，执行相加操作，然后都往后推
		{
			pa->coef += pb->coef;
			if (pa->coef == 0)//加完发现这一项变成0了
			{
				pa = pa->next;
				pb = pb->next;
			}
			else
			{
				r->next = pa;
				r = pa;
				pa = pa->next;
				pb = pb->next;
			}
		}
	}
	r ->next= pa ? pa : pb;
	return Ah;//返回结果链表的头结点
}


// ------------------- 测试：创建多项式（按指数非递减排序）
TermList CreatePoly(int coefs[], int exps[], int n) {
	TermList head = (Term*)malloc(sizeof(Term));
	head->next = NULL;
	Term* tail = head;

	for (int i = 0; i < n; i++) {
		Term* t = (Term*)malloc(sizeof(Term));
		t->coef = coefs[i];
		t->exp = exps[i];
		t->next = NULL;

		tail->next = t;
		tail = t;
	}
	return head;
}

// ------------------- 测试：打印多项式
void PrintPoly(TermList L) 
{
	Term* p = L->next;
	while (p) {
		printf("%dx^%d ", p->coef, p->exp);
		if (p->next) printf("+ ");
		p = p->next;
	}
	printf("\n");
}

//// ------------------- 主函数测试
//int main() 
//{
//	// 多项式 A
//	int coefA[] = { 3, -5, 6 };
//	int expA[] = { 1, 3, 5 };
//	TermList A = CreatePoly(coefA, expA, 3);
//
//	// 多项式 B
//	int coefB[] = { 2, 5, 8, 9 };
//	int expB[] = { 0, 3, 4, 6 };
//	TermList B = CreatePoly(coefB, expB, 4);
//
//	printf("A: ");
//	PrintPoly(A);
//	printf("B: ");
//	PrintPoly(B);
//
//	// 相加
//	TermList C = add_poly(A, B);
//	printf("\nA + B: ");
//	PrintPoly(C);
//
//	return 0;
//}