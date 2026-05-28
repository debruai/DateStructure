#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define INFEASIBLE -1

// 哈夫曼树节点结构
typedef struct HFTNode
{
    int weight;                 // 权值（字符出现的频次）
    char data;                  // 字符（叶子节点存字符，内部节点存0）
    struct HFTNode* lchild;     // 左孩子指针
    struct HFTNode* rchild;     // 右孩子指针
} HFTNode, * HFTree;

// 编码表条目：一个字符和它对应的哈夫曼编码串
typedef struct
{
    char data;       // 字符
    char* code;      // 哈夫曼编码字符串，如 "110"
} CodeEntry;

// 构建哈夫曼树
HFTree BuildHuffmanTree(int weights[], char data[], int n);

// 计算WPL（带权路径长度）
int CalculateWPL(HFTree root, int depth);

// 判断是否为叶子节点
int IsLeafNode(HFTNode* node);

// 打印树形结构
void PrintHuffmanTree(HFTree root, int depth);

// 释放整棵树
void FreeHuffmanTree(HFTree root);

//---------------------------------------------------------------
// 编码相关（使用全局编码表，省去传参复杂度，方便理解）
//---------------------------------------------------------------

// 初始化编码表
Status InitCodeTable(int n);

// 递归生成所有字符的哈夫曼编码
void GenerateCodes(HFTree root, char* path, int depth);

// 根据字符获取其哈夫曼编码串
char* GetCode(char ch);

// 打印编码表
void PrintCodeTable();

// 编码：将字符串转为哈夫曼编码串（调用者free结果）
char* HuffmanEncode(const char* text);

// 解码：将哈夫曼编码串还原为字符串（调用者free结果）
char* HuffmanDecode(HFTree root, const char* code);

// 释放编码表
void FreeCodeTable();
