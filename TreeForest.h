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
typedef char TElemtype;  // 用 char 方便表示树中的字母节点

//---------------------------------------------------------------
//  孩子兄弟表示法（树的二叉链表）
//
//  这是"树"的标准存储结构，也是"树→二叉树"转换的基础
//  firstchild — 指向第一个孩子（相当于二叉树的左孩子）
//  nextsibling — 指向下一个兄弟（相当于二叉树的右孩子）
//
//  关键理解：
//     从"孩子兄弟"的角度看，这就是一棵"二叉树"
//     所以树和二叉树可以用同一个结构体互相转换
//---------------------------------------------------------------
typedef struct CSNode
{
    TElemtype data;               // 数据域
    struct CSNode* firstchild;    // 指向第一个孩子
    struct CSNode* nextsibling;   // 指向下一个兄弟
} CSNode, * CSTree;

// 创建节点
CSTree CreateCSNode(TElemtype data);

// 构建一棵示例树（用于测试）
CSTree CreateSampleTree();

// 构建一片示例森林（用于测试）
CSTree* CreateSampleForest(int* forestSize);

//---------------------------------------------------------------
//  树 → 二叉树 转换
//  根本原理：
//     树的"孩子兄弟表示法"本身就是二叉树的结构
//     所以"转换"其实就是：把 firstchild 看作 lchild，
//     把 nextsibling 看作 rchild
//  返回的 BiTree 用的是之前 BiTree.c 中相同的结构
//---------------------------------------------------------------

// 树/森林的双亲表示法 → 也可以直接复用 CSNode
// 这里我们直接用 CSNode 构建，然后转换

// 树 → 二叉树（基于孩子兄弟表示法）
// 实际上孩子兄弟树本身就可视为二叉树，但为了理解转换过程
// 我们显式地实现一个 T 型转换函数
CSTree TreeToBinaryTree(CSTree tree);

// 二叉树 → 树
CSTree BinaryTreeToTree(CSTree binaryTree);

// 森林 → 二叉树
// 先把森林的每棵树转为二叉树，然后根节点依次作为右子树连接
CSTree ForestToBinaryTree(CSTree* forest, int forestSize);

// 二叉树 → 森林
// 从根节点的右子树不断拆分
CSTree* BinaryTreeToForest(CSTree binaryTree, int* forestSize);

//---------------------------------------------------------------
//  遍历
//---------------------------------------------------------------

// 树的先根遍历（基于孩子兄弟表示）
void PreOrderTree(CSTree T);

// 树的后根遍历（基于孩子兄弟表示）
void PostOrderTree(CSTree T);

// 按"树形"打印树
void PrintTree(CSTree T, int depth);

// 释放树
void FreeTree(CSTree T);
