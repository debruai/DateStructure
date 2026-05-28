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

// 散列表元素的三种状态
#define EMPTY 0     // 空
#define OCCUPIED 1  // 已占用
#define DELETED -1  // 已删除（惰性删除标记）

//===============================================================
//  开放定址法 — 线性探测 / 二次探测
//===============================================================

// 散列表元素
typedef struct
{
    int key;      // 关键字
    int flag;     // 状态：EMPTY / OCCUPIED / DELETED
} HashElem;

// 散列表结构
typedef struct
{
    HashElem* elem;   // 元素数组
    int size;         // 当前表长
    int count;        // 已存元素个数
} HashTable;

// 初始化散列表
Status InitHashTable(HashTable* HT, int size);

// 哈希函数（除留余数法）
int Hash(int key, int size);

// 线性探测重散列
int LinearProbe(int di, int size);

// 二次探测重散列
int QuadraticProbe(int di, int size);

// 插入（线性探测）
Status InsertLinear(HashTable* HT, int key);

// 插入（二次探测）
Status InsertQuadratic(HashTable* HT, int key);

// 查找
int SearchHash(HashTable* HT, int key, int* addr);

// 删除（惰性删除）
Status DeleteHash(HashTable* HT, int key);

// 打印散列表
void PrintHashTable(HashTable* HT);

// 计算平均查找长度 ASL
float CalculateASL(HashTable* HT);

// 再散列（扩容并重新插入）
Status Rehash(HashTable* HT, int newSize);

// 销毁散列表
void DestroyHashTable(HashTable* HT);

//===============================================================
//  链地址法 — 拉链法
//===============================================================

// 链地址法的链表节点
typedef struct HashNode
{
    int key;                    // 关键字
    struct HashNode* next;      // 指向下一个同义词
} HashNode, * HashNodePtr;

// 链地址法散列表
typedef struct
{
    HashNodePtr* buckets;   // 桶数组，每个桶是一个链表的头指针
    int size;               // 桶的个数（表长）
    int count;              // 元素总数
} ChainHash;

// 初始化链地址法散列表
Status InitChainHash(ChainHash* CHT, int size);

// 插入
Status InsertChain(ChainHash* CHT, int key);

// 查找
HashNode* SearchChain(ChainHash* CHT, int key);

// 删除
Status DeleteChain(ChainHash* CHT, int key);

// 打印
void PrintChainHash(ChainHash* CHT);

// 计算链地址法 ASL
float CalculateChainASL(ChainHash* CHT);

// 销毁
void DestroyChainHash(ChainHash* CHT);
