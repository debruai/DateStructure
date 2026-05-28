#pragma once
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

// 二叉树的链式存储结构
typedef struct BiTNode
{
    TElemtype data;  // 数据域
    struct BiTNode* lchild;  // 左子树指针
    struct BiTNode* rchild;  // 右子树指针
} BiTNode, * BiTree;  // 结点类型，二叉链表指针类型

// 二叉树的基本操作

// 1.生成一个只有根结点的二叉树并返回二叉树指针，其数据为value
BiTree InitTree(TElemtype value);

// 2.组建并返回一个二叉树，其根节点的值为value，左子树和右子树分别为left和right
BiTree MakeBiTree(TElemtype value, BiTree left, BiTree right);

// 3.回收某个二叉树的全部结点
BiTree FreeBiTree(BiTree T);

// 4.判断某个二叉树是否为空，空则返回True
Status BiTEmpty(BiTree T);

// 5.替换右子树，用已有的子树替换二叉树T的右子树,并返回原来的右子树
BiTree ReplaceRight(BiTree T, BiTree RT);

// 6.替换左子树，用已有的子树替换二叉树T的左子树,并返回原来的左子树
BiTree ReplaceLeft(BiTree T, BiTree LT);

// 7.对当前二叉树结点进行操作的函数
Status VisitNode(BiTNode* node);

// 8.获取二叉树深度
int BiTreeDepth(BiTree T);

// 9.获取二叉树节点总数
int BiTreeNodeCount(BiTree T);

// 10.获取二叉树叶子节点数
int BiTreeLeafCount(BiTree T);

// 11.查找节点
BiTree FindNode(BiTree T, TElemtype e);

// 12.复制二叉树
BiTree CopyBiTree(BiTree T);

// 13.先序遍历
Status PreOrderTrav(BiTree T, Status(*VisitNode)(BiTNode* node));

// 14.中序遍历
Status InOrderTrav(BiTree T, Status(*VisitNode)(BiTNode* node));

// 15.后序遍历
Status PostOrderTrav(BiTree T, Status(*VisitNode)(BiTNode* node));
