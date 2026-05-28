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

//===============================================================
//  静态查找表 — 顺序查找 / 折半查找
//===============================================================

#define MAXSIZE 100

// 查找表
typedef struct
{
    int r[MAXSIZE + 1];  // r[0] 闲置或用作哨兵
    int length;
} SSTable;

// 初始化
void CreateTable(SSTable* ST, int keys[], int n);

// 顺序查找（哨兵） — 返回位置，0表示没找到
int SeqSearch(SSTable* ST, int key);

// 折半查找（迭代） — 要求有序
int BinarySearch(SSTable* ST, int key);

// 折半查找（递归）
int BinarySearchRecur(SSTable* ST, int key, int low, int high);

//===============================================================
//  二叉排序树 BST
//===============================================================

typedef struct BSTNode
{
    int key;
    struct BSTNode* lchild;
    struct BSTNode* rchild;
} BSTNode, * BSTree;

// 插入
Status BSTInsert(BSTree* T, int key);

// 查找
BSTree BSTSearch(BSTree T, int key);

// 删除
Status BSTDelete(BSTree* T, int key);

// 中序遍历（得到有序序列）
void InOrderBST(BSTree T);

// 打印树形
void PrintBST(BSTree T, int depth);

// 销毁
void FreeBST(BSTree T);

//===============================================================
//  平衡二叉树 AVL
//===============================================================

typedef struct AVLNode
{
    int key;
    int bf;                        // 平衡因子：左子树高度 - 右子树高度
    struct AVLNode* lchild;
    struct AVLNode* rchild;
} AVLNode, * AVLTree;

// 插入
Status AVLInsert(AVLTree* T, int key, int* taller);

// 四种旋转
void R_Rotate(AVLTree* T);   // 右旋（LL型）
void L_Rotate(AVLTree* T);   // 左旋（RR型）
void LR_Rotate(AVLTree* T);  // 先左后右
void RL_Rotate(AVLTree* T);  // 先右后左

// 左平衡处理（左子树过高）
void LeftBalance(AVLTree* T);

// 右平衡处理（右子树过高）
void RightBalance(AVLTree* T);

// 中序 + 打印树形
void InOrderAVL(AVLTree T);
void PrintAVL(AVLTree T, int depth);
void FreeAVL(AVLTree T);
