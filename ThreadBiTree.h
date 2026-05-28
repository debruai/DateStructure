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

// 线索二叉树的节点结构
// ltag = 0: lchild 指向左孩子
// ltag = 1: lchild 指向前驱（线索）
// rtag = 0: rchild 指向右孩子
// rtag = 1: rchild 指向后继（线索）
typedef struct BiThrNode
{
    TElemtype data;                    // 数据域
    struct BiThrNode* lchild;          // 左孩子指针 / 前驱线索
    struct BiThrNode* rchild;          // 右孩子指针 / 后继线索
    int ltag;                          // 左标志（0=孩子, 1=线索）
    int rtag;                          // 右标志（0=孩子, 1=线索）
} BiThrNode, * BiThrTree;

// 创建二叉树节点
BiThrTree CreateNode(TElemtype data);

// 用先序序列构建二叉树（'#'表示空子树）
BiThrTree CreateBiTree(TElemtype* preOrder, int* index, int maxLen);

// 中序线索化（核心算法）
void InThread(BiThrTree p, BiThrTree* pre);

// 带头节点的中序线索化（方便遍历）
BiThrTree InOrderThreading(BiThrTree T);

// 遍历中序线索二叉树（非递归，无需栈）
Status InOrderTraverse(BiThrTree Thrt);

// 释放二叉树
void FreeBiTree(BiThrTree T);
