// 数据结构第六章树与二叉树
// 二叉树的重要性质：
// 二叉树的第i层至多有2^(i-1)个结点；
// 深度为k的二叉树至多有2^k-1个结点；
// 对于任何一棵二叉树T，若其叶子结点数为n0，度为2的结点数为n2，则n0=n2+1

// 完全二叉树：除最后一层外，每层结点数都达到最大值，并且最后一层的结点都连续集中在最左边
// 完全二叉树的性质：对于完全二叉树，如果结点总数为n，则其深度为log2(n)+1；对于完全二叉树，如果结点总数为n，则其叶子结点数为(n+1)/2
// 
// 满二叉树：每个结点要么没有子树，要么有两个子树，并且所有叶子结点都在同一层上
#include <stdio.h>
#include <stdlib.h>

typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define INFEASIBLE -1
typedef int TElemtype;

// 二叉树的存储结构
// 顺序存储：适合完全二叉树，使用数组来存储二叉树的结点，根结点存储在数组的第一个位置，左子树和右子树分别存储在根结点的下标为2*i+1和2*i+2的位置

// 链式存储：适合一般二叉树，使用链表来存储二叉树的结点，每个结点包含数据域和两个指针域，分别指向左子树和右子树
typedef struct BiTNode
{
    TElemtype data;  // 数据域
    struct BiTNode* lchild;  // 左子树指针
    struct BiTNode* rchild;  // 右子树指针
} BiTNode, * BiTree;  // 结点类型，二叉链表指针类型

// 二叉树的基本操作

// 1.生成一个只有根结点的二叉树并返回二叉树指针，其数据为value
BiTree InitTree(TElemtype value)
{
    BiTree T;
    if (!(T = (BiTree)malloc(sizeof(BiTNode))))
        exit(OVERFLOW);
    T->data = value;
    T->lchild = NULL;
    T->rchild = NULL;
    return T;  
}

// 2.组建并返回一个二叉树，其根节点的值为value，左子树和右子树分别为left和right
BiTree MakeBiTree(TElemtype value, BiTree left, BiTree right)
{
    BiTree T;
    if (!(T = (BiTree)malloc(sizeof(BiTNode))))  
        exit(OVERFLOW);
    T->data = value;
    T->lchild = left;
    T->rchild = right;
    return T;  
}

// 3.回收某个二叉树的全部结点
BiTree FreeBiTree(BiTree T)
{
    if (!T)
        return NULL;
    if (T->lchild)
    {
        FreeBiTree(T->lchild);
    }
    if (T->rchild)
    {
        FreeBiTree(T->rchild);
    }
    free(T);
    return NULL;
}

// 4.判断某个二叉树是否为空，空则返回True
Status BiTEmpty(BiTree T)
{
    if (!T)
        return TRUE;
    return FALSE; 
}

// 5.替换右子树，用已有的子树替换二叉树T的右子树,并返回原来的右子树
BiTree ReplaceRight(BiTree T, BiTree RT)
{
    if (!T)
        return NULL;
    BiTree TempTree = T->rchild;
    T->rchild = RT;
    return TempTree;
}

BiTree ReplaceLeft(BiTree T, BiTree LT)  // 同理替换左子树
{
    if (!T)
        return NULL;
    BiTree TempTree = T->lchild;
    T->lchild = LT;
    return TempTree;
}

// 6.对当前二叉树结点进行操作的函数
Status VisitNode(BiTNode* node)
{
    if (!node)
        return ERROR;  // 修正：如果节点为空，返回错误
    printf("%d ", node->data);
    return OK;
}

// 获取二叉树深度
int BiTreeDepth(BiTree T)
{
    if (!T)
        return 0;

    int leftDepth = BiTreeDepth(T->lchild);
    int rightDepth = BiTreeDepth(T->rchild);

    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}

// 获取二叉树节点总数
int BiTreeNodeCount(BiTree T)
{
    if (!T)
        return 0;

    return 1 + BiTreeNodeCount(T->lchild) + BiTreeNodeCount(T->rchild);
}

// 获取二叉树叶子节点数
int BiTreeLeafCount(BiTree T)
{
    if (!T)
        return 0;

    if (!T->lchild && !T->rchild)  // 叶子节点
        return 1;

    return BiTreeLeafCount(T->lchild) + BiTreeLeafCount(T->rchild);
}

// 查找节点
BiTree FindNode(BiTree T, TElemtype e)
{
    if (!T)
        return NULL;

    if (T->data == e)
        return T;

    BiTree found = FindNode(T->lchild, e);
    if (found)
        return found;

    return FindNode(T->rchild, e);
}

// 复制二叉树
BiTree CopyBiTree(BiTree T)
{
    if (!T)
        return NULL;

    BiTree newTree = (BiTree)malloc(sizeof(BiTNode));
    if (!newTree)
        exit(OVERFLOW);

    newTree->data = T->data;
    newTree->lchild = CopyBiTree(T->lchild);
    newTree->rchild = CopyBiTree(T->rchild);

    return newTree;
}

// 遍历二叉树

// 递归版本
// 先序遍历
Status PreOrderTrav(BiTree T, Status(*VisitNode)(BiTNode* node))
{
    if (T)
    {
        if (VisitNode(T))  // 访问根节点
        {
            if (PreOrderTrav(T->lchild, VisitNode))  // 遍历左子树
            {
                if (PreOrderTrav(T->rchild, VisitNode))  // 遍历右子树
                {
                    return OK;
                }
            }
        }
        return ERROR;
    }
    else
        return OK;
}

// 中序遍历
Status InOrderTrav(BiTree T, Status(*VisitNode)(BiTNode* node))
{
    if (T)
    {
        if (InOrderTrav(T->lchild, VisitNode))  // 先遍历左子树
        {
            if (VisitNode(T))  // 访问根节点
            {
                if (InOrderTrav(T->rchild, VisitNode))  // 再遍历右子树
                {
                    return OK;
                }
            }
        }
        return ERROR;
    }
    else
        return OK;
}

// 后序遍历
Status PostOrderTrav(BiTree T, Status(*VisitNode)(BiTNode* node))
{
    if (T)
    {
        if (PostOrderTrav(T->lchild, VisitNode))  // 先遍历左子树
        {
            if (PostOrderTrav(T->rchild, VisitNode))  // 再遍历右子树
            {
                if (VisitNode(T))  // 最后访问根节点
                {
                    return OK;
                }
            }
        }
        return ERROR;
    }
    else
        return OK;
}


// 测试函数
int main()
{
    printf("======= 二叉树测试 =======\n");

    // 1. 创建二叉树
    // 构建如下二叉树：
    //        1
    //       / \
    //      2   3
    //     / \   \
    //    4   5   6

    BiTree node4 = InitTree(4);
    BiTree node5 = InitTree(5);
    BiTree node6 = InitTree(6);

    BiTree node2 = MakeBiTree(2, node4, node5);
    BiTree node3 = MakeBiTree(3, NULL, node6);
    BiTree root = MakeBiTree(1, node2, node3);

    printf("1. 二叉树创建成功\n");

    // 2. 测试遍历
    printf("\n2. 遍历测试:\n");

    printf("   先序遍历结果: ");
    PreOrderTrav(root, VisitNode);
    printf("\n");

    printf("   中序遍历结果: ");
    InOrderTrav(root, VisitNode);
    printf("\n");

    printf("   后序遍历结果: ");
    PostOrderTrav(root, VisitNode);
    printf("\n");

    // 3. 测试其他操作
    printf("\n3. 其他操作测试:\n");

    printf("   二叉树深度: %d\n", BiTreeDepth(root));
    printf("   二叉树节点总数: %d\n", BiTreeNodeCount(root));
    printf("   二叉树叶子节点数: %d\n", BiTreeLeafCount(root));

    // 4. 测试查找节点
    printf("\n4. 查找节点测试:\n");
    BiTree found = FindNode(root, 5);
    if (found)
    {
        printf("   找到节点: %d\n", found->data);
    }
    else
    {
        printf("   未找到节点\n");
    }

    // 5. 测试复制二叉树
    printf("\n5. 复制二叉树测试:\n");
    BiTree copy = CopyBiTree(root);
    printf("   复制后的二叉树先序遍历: ");
    PreOrderTrav(copy, VisitNode);
    printf("\n");

    // 6. 测试替换子树
    printf("\n6. 替换子树测试:\n");
    BiTree newSubTree = InitTree(7);
    BiTree oldRight = ReplaceRight(node2, newSubTree);
    printf("   替换后二叉树先序遍历: ");
    PreOrderTrav(root, VisitNode);
    printf("\n");

    // 7. 释放二叉树
    printf("\n7. 释放二叉树:\n");
    root = FreeBiTree(root);
    copy = FreeBiTree(copy);
    printf("   二叉树已释放\n");

    printf("\n======= 测试完成 =======\n");

    return 0;
}