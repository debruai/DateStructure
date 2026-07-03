# 二叉树（BiTree）完整解析

> 数据结构第六章 — C语言链式存储实现
> 适用：严蔚敏《数据结构》配套学习
> 代码位置：`DateStructure/BiTree.h` + `BiTree.c`

---

## 目录

1. [二叉树基础回顾](#1-二叉树基础回顾)
2. [存储结构对比](#2-存储结构对比)
3. [树结构图解](#3-树结构图解)
4. [BiTree.h 逐行解析](#4-bitreeh-逐行解析)
5. [BiTree.c 逐行解析](#5-bitreec-逐行解析)
   - [InitTree — 创建根结点](#51-inittree--创建根结点)
   - [MakeBiTree — 组建二叉树](#52-makebitree--组建二叉树)
   - [FreeBiTree — 递归释放](#53-freebitree--递归释放)
   - [BiTEmpty — 判空](#54-bitempty--判空)
   - [ReplaceLeft / ReplaceRight — 替换子树](#55-replaceleft--replaceright--替换子树)
   - [VisitNode — 访问结点](#56-visitnode--访问结点)
   - [BiTreeDepth — 求深度](#57-bitreedepth--求深度)
   - [BiTreeNodeCount — 求结点总数](#58-bitreenodecount--求结点总数)
   - [BiTreeLeafCount — 求叶子数](#59-birteeleafcount--求叶子数)
   - [FindNode — 查找结点](#510-findnode--查找结点)
   - [CopyBiTree — 复制二叉树](#511-copybitree--复制二叉树)
   - [三种遍历 — PreOrder / InOrder / PostOrder](#512-三种遍历--preorder--inorder--postorder)
   - [main() — 测试函数](#513-main--测试函数)
6. [三种遍历的递归过程推演](#6-三种遍历的递归过程推演)
7. [替换子树的内存泄漏问题](#7-替换子树的内存泄漏问题)
8. [为什么遍历函数有嵌套 if 链？](#8-为什么遍历函数有嵌套-if-链)
9. [优化总结](#9-优化总结)
10. [自测 5 题](#10-自测-5-题)

---

## 1. 二叉树基础回顾

### 重要性质

| 性质 | 公式 | 解释 |
|------|------|------|
| 第 i 层最多结点数 | $2^{i-1}$ | 根是第 1 层，最多 1 个；第 2 层最多 2 个…… |
| 深度 k 的树最多结点数 | $2^k - 1$ | 就是满二叉树 |
| 叶子数 n₀ 与度2结点数 n₂ 的关系 | $n_0 = n_2 + 1$ | **最重要的性质**，证明用到了边数 = 结点数 - 1 |
| 完全二叉树深度 | $\lfloor \log_2 n \rfloor + 1$ | n 为结点总数 |

### 两个特殊形态

| 类型 | 定义 | 特点 |
|------|------|------|
| **满二叉树** | 每个结点要么是叶子，要么有两个孩子；所有叶子在同一层 | 完美对称，$2^k-1$ 个结点 |
| **完全二叉树** | 除最后一层外满的，最后一层结点靠左连续 | 可以用数组存（层序遍历顺序） |

---

## 2. 存储结构对比

| 存储方式 | 适用场景 | 优点 | 缺点 |
|----------|----------|------|------|
| **顺序存储（数组）** | 完全二叉树 | 下标直接算父子关系：左子 = 2i+1，右子 = 2i+2 | 普通二叉树浪费大量空间 |
| **链式存储（二叉链表）** | 一般二叉树 | 只存实际结点，空间效率高 | 需要指针管理 |

本项目采用**链式存储**，即每个结点用一个结构体：

```
+------------------+
|  data (int)      |  ← 数据域
|  lchild (指针)   |  ← 指向左孩子
|  rchild (指针)   |  ← 指向右孩子
+------------------+
```

叶子结点的 lchild 和 rchild 都是 `NULL`。

---

## 3. 树结构图解

测试代码构建的树：

```
         1              ← 根结点 (root)
        / \
       /   \
      2     3           ← 第 2 层
     / \     \
    4   5     6         ← 第 3 层
```

三种遍历输出对照：

| 遍历方式 | 输出顺序 | 口诀 |
|----------|----------|------|
| **先序** | 1 2 4 5 3 6 | **根**左右 |
| **中序** | 4 2 5 1 3 6 | 左**根**右 |
| **后序** | 4 5 2 6 3 1 | 左右**根** |

> 遍历结果验证：
> - 先序第一个 = 根（1）
> - 后序最后一个 = 根（1）
> - 中序根左边 = 左子树全部（4,2,5），右边 = 右子树全部（3,6）

---

## 4. BiTree.h 逐行解析

### 第 1 行 `#pragma once`

**功能**：防止同一个头文件被 `#include` 多次。

和传统 `#ifndef` 写法等价：

```c
#ifndef BITREE_H
#define BITREE_H
// ...
#endif
```

`#pragma once` 更简洁，但属于编译器扩展（GCC/Clang/MSVC 都支持），教材上更多用 `#ifndef`。

### 第 2-3 行 `#include <stdio.h>` `#include <stdlib.h>`

`stdio.h` 提供 `printf`、`NULL` 等；`stdlib.h` 提供 `malloc`、`free`、`exit`。

> **为什么放在 .h 里？** 因为 .c 文件 `#include "BiTree.h"` 后就不需要再重复 include 了。**头文件要自包含**——使用者只要 include 这一个文件就能编译。

### 第 5-12 行 通用类型定义

```c
typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define INFEASIBLE -1
```

这是严蔚敏教材的"约定"写法。C 语言没有 `bool` 类型（C99 才有），所以用 `int` 模拟布尔值。每个函数返回 `Status` 表示成功/失败。

> **为什么 OVERFLOW 是 -2？** 和 `EXIT_FAILURE`（1）和 `EXIT_SUCCESS`（0）区分开，`-2` 表示"内存耗尽这种严重错误"。

### 第 14 行 `typedef int TElemtype`

定义元素类型。如果你想改成 `char` 或 `float`，只改这一行就行——**为了可维护性**。

### 第 17-22 行 二叉树结点结构体

```c
typedef struct BiTNode
{
    TElemtype data;
    struct BiTNode* lchild;
    struct BiTNode* rchild;
} BiTNode, *BiTree;
```

关键理解：

1. **`struct BiTNode` 内部引用自身**：`lchild` 和 `rchild` 的类型是 `struct BiTNode*`。这在 C 中必须用完整结构体名，因为 `typedef` 此时还没生效（自引用——结构体还没定义完）。

2. **`BiTNode` 和 `BiTree` 的区别**：
   - `BiTNode` = 结点结构体类型
   - `BiTree` = `BiTNode*`，即指向根结点的指针

3. **为什么需要 `BiTree` 类型？**
   - 表达"一棵树"的概念——传参时传 `BiTree` 比传 `BiTNode*` 语义更清楚
   - 但要注意：`BiTree` 是指针的 typedef，**不能加 `const`**，因为 `const BiTree` 等价于 `BiTNode * const`（指针本身常量化），而不是 `const BiTNode *`（指向的内容常量化）。

### 第 24-56 行 函数声明

全部是"声明告诉编译器"这些函数存在，实现在 .c 中。

值得注意的声明：

```c
Status PreOrderTrav(BiTree T, Status(*VisitNode)(BiTNode* node));
```

这里的第二个参数是一个**函数指针**：

- `Status(*)`：指向一个返回 `Status` 的函数
- `(BiTNode* node)`：该函数接受一个 `BiTNode*` 参数

> **为什么传函数指针？** 这样遍历时可以传入不同的"访问操作"。今天打印值，明天改成求和，后天改成存数组——**遍历逻辑不变，只换回调函数**。这就是设计模式中的"策略模式"。

---

## 5. BiTree.c 逐行解析

### 5.1 InitTree — 创建根结点

```c
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
```

| 代码 | 含义 |
|------|------|
| `malloc(sizeof(BiTNode))` | 在堆上分配一个结点大小的内存 |
| `(BiTree)` | 强制类型转换，C++ 需要，C 中可省略但建议保留 |
| `if (!(T = ...))` | 两步合一：分配 + 判空。`malloc` 失败返回 NULL |
| `exit(OVERFLOW)` | 内存耗尽直接终止程序（教材风格，生产环境应优雅处理） |
| `T->lchild = NULL` | 新结点是叶子，左右置空 |

> 为什么叫 **Init**Tree 而不是 **Create**Tree？因为它只创建根结点，树还没"长"出来。真正组装用 MakeBiTree。

### 5.2 MakeBiTree — 组建二叉树

```c
BiTree MakeBiTree(TElemtype value, BiTree left, BiTree right)
{
    BiTree T;
    if (!(T = (BiTree)malloc(sizeof(BiTNode))))
        exit(OVERFLOW);
    T->data = value;
    T->lchild = left;    // 直接把 left 挂成左子树
    T->rchild = right;   // 直接把 right 挂成右子树
    return T;
}
```

和 InitTree 的区别：左右子树不是 NULL，而是传入的参数。

**组合示例**（测试代码）：

```
                    MakeBiTree(1, node2, node3)
                         ↙           ↘
              MakeBiTree(2, n4, n5)   MakeBiTree(3, NULL, n6)
                   ↙    ↘                  ↙      ↘
              InitTree(4) InitTree(5)    NULL    InitTree(6)
```

这就是"自底向上"构建二叉树——先建叶子，再往上组合。每一层 `MakeBiTree` 都把已经建好的子树"挂"到自己下面。

### 5.3 FreeBiTree — 递归释放

```c
BiTree FreeBiTree(BiTree T)
{
    if (!T)           // 空树直接返回
        return NULL;
    if (T->lchild)    // 有左子树，先释放左子树
        FreeBiTree(T->lchild);
    if (T->rchild)    // 有右子树，再释放右子树
        FreeBiTree(T->rchild);
    free(T);          // 最后释放自己
    return NULL;      // 返回 NULL 方便调用方写 root = FreeBiTree(root);
}
```

**递归释放顺序：后序遍历**（因为先释放孩子，再释放自己）。

图解释放过程（以根为 1 的树为例）：

```
Step 1: FreeBiTree(1) → 发现左孩子 2
Step 2:   FreeBiTree(2) → 发现左孩子 4
Step 3:     FreeBiTree(4) → 无孩子，free(4)，返回 NULL
Step 4:   回到 FreeBiTree(2) → 发现右孩子 5
Step 5:     FreeBiTree(5) → 无孩子，free(5)，返回 NULL
Step 6:   回到 FreeBiTree(2) → 孩子都释放了，free(2)，返回 NULL
Step 7:   回到 FreeBiTree(1) → 发现右孩子 3
Step 8:     FreeBiTree(3) → 发现右孩子 6
Step 9:       FreeBiTree(6) → 无孩子，free(6)，返回 NULL
Step 10:    回到 FreeBiTree(3) → 孩子都释放了，free(3)，返回 NULL
Step 11:   回到 FreeBiTree(1) → 孩子都释放了，free(1)，返回 NULL
```

> **为什么返回 NULL？** 方便使用方直接写 `root = FreeBiTree(root);`，这样 root 自动变成 NULL，不会变成野指针。**防御性编程**的好习惯。

### 5.4 BiTEmpty — 判空

```c
Status BiTEmpty(BiTree T)
{
    if (!T) return TRUE;
    return FALSE;
}
```

也可以简写为 `return !T;`，但教材风格喜欢显式返回 TRUE/FALSE。

### 5.5 ReplaceLeft / ReplaceRight — 替换子树

```c
BiTree ReplaceRight(BiTree T, BiTree RT)
{
    if (!T)
        return NULL;
    BiTree TempTree = T->rchild;  // 保存旧的右子树
    T->rchild = RT;               // 挂上新的
    return TempTree;              // 把旧的还给你
}
```

**设计思路**：替换即"先取出来，再放进去"。返回旧的子树让调用方决定怎么处理——可以 free、可以挂到别处、可以留着用。

> ⚠️ **如果不处理返回的旧子树，会导致内存泄漏！** 详见第 7 节。

### 5.6 VisitNode — 访问结点

```c
Status VisitNode(BiTNode* node)
{
    if (!node)
        return ERROR;
    printf("%d ", node->data);
    return OK;
}
```

**注意参数类型**：`BiTNode*`（指向结点），不是 `BiTree`（指向树的指针）。两者的底层类型一样，但语义不同——`VisitNode` 只处理单个结点。

### 5.7 BiTreeDepth — 求深度

```c
int BiTreeDepth(BiTree T)
{
    if (!T) return 0;
    int leftDepth = BiTreeDepth(T->lchild);
    int rightDepth = BiTreeDepth(T->rchild);
    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}
```

**递归思路**：

```
树的深度 = max(左子树深度, 右子树深度) + 1
```

空树深度 = 0（递归基）。

**执行过程**（以结点 1 为例）：

```
BiTreeDepth(1)
  ├─ leftDepth = BiTreeDepth(2)
  │   ├─ leftDepth = BiTreeDepth(4) = max(0,0)+1 = 1
  │   ├─ rightDepth = BiTreeDepth(5) = max(0,0)+1 = 1
  │   └─ return max(1,1)+1 = 2
  ├─ rightDepth = BiTreeDepth(3)
  │   ├─ leftDepth = BiTreeDepth(NULL) = 0
  │   ├─ rightDepth = BiTreeDepth(6) = max(0,0)+1 = 1
  │   └─ return max(0,1)+1 = 2
  └─ return max(2,2)+1 = 3 ✅
```

### 5.8 BiTreeNodeCount — 求结点总数

```c
int BiTreeNodeCount(BiTree T)
{
    if (!T) return 0;
    return 1 + BiTreeNodeCount(T->lchild) + BiTreeNodeCount(T->rchild);
}
```

**递归思路**：

```
结点总数 = 1（自己） + 左子树结点数 + 右子树结点数
```

### 5.9 BiTreeLeafCount — 求叶子数

```c
int BiTreeLeafCount(BiTree T)
{
    if (!T)                        // 空树：没有叶子
        return 0;
    if (!T->lchild && !T->rchild)  // 自己是叶子
        return 1;
    return BiTreeLeafCount(T->lchild) + BiTreeLeafCount(T->rchild);
}
```

**关键理解**：这里有两个递归基：

| 条件 | 含义 | 返回值 |
|------|------|--------|
| `T == NULL` | 空指针，不可能是叶子 | 0 |
| `T->lchild == NULL && T->rchild == NULL` | 没有孩子 = 叶子结点 | 1 |

**注意两个基的顺序**：必须先判断 `!T`，再判断叶子——因为如果 `T` 是 NULL，访问 `T->lchild` 会段错误！

### 5.10 FindNode — 查找结点

```c
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
```

**查找顺序**：**先序**（先看自己 → 再看左边 → 最后右边）。

找到第一个匹配就返回（如果树中有重复值，只返回第一个）。没找到返回 NULL。

> **为什么是先序查找？** 先序查找是最"直觉"的——先看自己是不是，不是再去孩子里找。这样可以尽快返回，不用遍历整棵树。

### 5.11 CopyBiTree — 复制二叉树

```c
BiTree CopyBiTree(BiTree T)
{
    if (!T)
        return NULL;
    BiTree newTree = (BiTree)malloc(sizeof(BiTNode));
    if (!newTree)
        exit(OVERFLOW);
    newTree->data = T->data;          // 复制数据
    newTree->lchild = CopyBiTree(T->lchild);  // 递归复制左子树
    newTree->rchild = CopyBiTree(T->rchild);  // 递归复制右子树
    return newTree;
}
```

**深度拷贝**：不是复制指针（那样新旧树会共享结点），而是**递归地为每个结点分配新内存**。

```
原树:           副本:
  1              1' (全新malloc)
 / \            / \
2   3          2'  3'
   / \            / \
  N   6          N   6'
```

### 5.12 三种遍历 — PreOrder / InOrder / PostOrder

**三者模式完全一样，只是访问根的顺序不同：**

```c
// 先序：根 左 右
if (T) {
    visit(T);                       ← 先访问根
    PreOrderTrav(T->lchild, visit);
    PreOrderTrav(T->rchild, visit);
}

// 中序：左 根 右
if (T) {
    InOrderTrav(T->lchild, visit);
    visit(T);                       ← 中间访问根
    InOrderTrav(T->rchild, visit);
}

// 后序：左 右 根
if (T) {
    PostOrderTrav(T->lchild, visit);
    PostOrderTrav(T->rchild, visit);
    visit(T);                       ← 最后访问根
}
```

**你的代码实际写的版本更「教材风」——嵌了错误传播：**

```c
Status PreOrderTrav(BiTree T, Status(*visit)(BiTNode* node))
{
    if (T)
    {
        if (visit(T))                          // 访问根
        {
            if (PreOrderTrav(T->lchild, visit)) // 遍历左
            {
                if (PreOrderTrav(T->rchild, visit)) // 遍历右
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
```

每一层都检查返回值：如果 `visit(T)` 失败了，就不往下走，返回 ERROR。这叫**错误传播**——错误从底层一层层上传。

> 实际项目中 `VisitNode` 几乎不可能失败（`printf` 也不会返回 ERROR），所以这层检查更多是教学意义——展示"健壮的错误处理应该怎么写"。

### 5.13 main() — 测试函数

```c
int main()
{
    // 1. 自底向上建树
    BiTree node4 = InitTree(4);
    BiTree node5 = InitTree(5);
    BiTree node6 = InitTree(6);
    BiTree node2 = MakeBiTree(2, node4, node5);
    BiTree node3 = MakeBiTree(3, NULL, node6);
    BiTree root = MakeBiTree(1, node2, node3);

    // 2. 遍历测试
    PreOrderTrav(root, VisitNode);   // 传函数名=传函数指针
    // ...
    // 3. 统计信息
    BiTreeDepth(root);
    BiTreeNodeCount(root);
    BiTreeLeafCount(root);
    // 4. 查找
    FindNode(root, 5);
    // 5. 复制
    CopyBiTree(root);
    // 6. 替换子树
    ReplaceRight(node2, newSubTree);
    FreeBiTree(oldRight);  // 优化后：释放旧子树
    // 7. 释放
    root = FreeBiTree(root);
}
```

**关于 `PreOrderTrav(root, VisitNode)`**：

`VisitNode` 就是函数名，函数名在 C 中就是函数指针。所以这里等价于：

```c
PreOrderTrav(root, &VisitNode);  // & 可省略
```

因为 `VisitNode` 函数签名和参数类型完全匹配——`Status (*)(BiTNode*)`。

---

## 6. 三种遍历的递归过程推演

以这棵树为例：

```
      1
     / \
    2   3
   / \   \
  4   5   6
```

### 先序遍历（根 → 左 → 右）

```
Pre(1) → visit(1)① → Pre(2)
           Pre(2) → visit(2)② → Pre(4)
                      Pre(4) → visit(4)③ → Pre(NULL)✅ → Pre(NULL)✅ → 返回
                    ← Pre(4)返回✅ → Pre(5)
                      Pre(5) → visit(5)④ → Pre(NULL)✅ → Pre(NULL)✅ → 返回
                    ← Pre(5)返回✅ → 返回
                  ← Pre(2)返回✅ → Pre(3)
                    Pre(3) → visit(3)⑤ → Pre(NULL)✅ → Pre(6)
                      Pre(6) → visit(6)⑥ → Pre(NULL)✅ → Pre(NULL)✅ → 返回
                    ← Pre(6)返回✅ → 返回
                  ← Pre(3)返回✅ → 返回
                ← Pre(1)返回✅
输出: 1 2 4 5 3 6 ✅
```

### 中序遍历（左 → 根 → 右）

```
In(1) → In(2)
         In(2) → In(4)
                  In(4) → In(NULL)✅ → visit(4)① → In(NULL)✅ → 返回
                ← In(4)返回✅ → visit(2)② → In(5)
                  In(5) → In(NULL)✅ → visit(5)③ → In(NULL)✅ → 返回
                ← In(5)返回✅ → 返回
              ← In(2)返回✅ → visit(1)④ → In(3)
                In(3) → In(NULL)✅ → visit(3)⑤ → In(6)
                  In(6) → In(NULL)✅ → visit(6)⑥ → In(NULL)✅ → 返回
                ← In(6)返回✅ → 返回
              ← In(3)返回✅ → 返回
            ← In(1)返回✅
输出: 4 2 5 1 3 6 ✅
```

### 后序遍历（左 → 右 → 根）

```
Post(1) → Post(2)
           Post(2) → Post(4)
                      Post(4) → Post(NULL)✅ → Post(NULL)✅ → visit(4)① → 返回
                    ← Post(4)返回✅ → Post(5)
                      Post(5) → Post(NULL)✅ → Post(NULL)✅ → visit(5)② → 返回
                    ← Post(5)返回✅ → visit(2)③ → 返回
                  ← Post(2)返回✅ → Post(3)
                    Post(3) → Post(NULL)✅ → Post(6)
                      Post(6) → Post(NULL)✅ → Post(NULL)✅ → visit(6)④ → 返回
                    ← Post(6)返回✅ → visit(3)⑤ → 返回
                  ← Post(3)返回✅ → visit(1)⑥ → 返回
                ← Post(1)返回✅
输出: 4 5 2 6 3 1 ✅
```

---

## 7. 替换子树的内存泄漏问题

### 问题描述

优化前的代码：

```c
BiTree oldRight = ReplaceRight(node2, newSubTree);
(void)oldRight;  // 原右子树不在此处使用
```

替换后，`oldRight` 指向结点 5，但没有任何人释放它。结点 5 变成了**孤岛内存**——没有指针指向它，永远无法 free。

```
替换前:                      替换后:
    node2(2)                    node2(2)
    /      \                    /      \
  n4(4)   n5(5) ← 被替换    n4(4)   n7(7) ← 新的
               ↑                         ↑
          oldRight 指向这里      从此再无指针指向 n5
                                   → 内存泄漏！
```

### 解决方案

优化后的代码：

```c
BiTree oldRight = ReplaceRight(node2, newSubTree);
FreeBiTree(oldRight);  // 取出来后立即释放
```

### 使用 ReplaceRight 的正确姿势

```c
// 用法1：替换并释放旧子树
FreeBiTree(ReplaceRight(T, newSub));

// 用法2：替换并挂到别处
BiTree oldSub = ReplaceRight(T, newSub);
ReplaceLeft(anotherNode, oldSub);  // 把旧子树移到另一个节点下

// 用法3：只是想移除右子树
BiTree right = ReplaceRight(T, NULL);  // 传入 NULL 即移除
// ... 处理 right ...
```

---

## 8. 为什么遍历函数有嵌套 if 链？

你可能注意到遍历函数写得比想象中复杂：

```c
// 你的代码（带错误传播）
if (T) {
    if (visit(T)) {
        if (PreOrderTrav(T->lchild, visit)) {
            if (PreOrderTrav(T->rchild, visit)) {
                return OK;
            }
        }
    }
    return ERROR;
} else
    return OK;
```

对比更常见的简化写法：

```c
// 简化版（无错误处理）
void PreOrder(BiTree T, void (*visit)(BiTNode*)) {
    if (T) {
        visit(T);
        PreOrder(T->lchild, visit);
        PreOrder(T->rchild, visit);
    }
}
```

**为什么教材/老师喜欢写复杂版本？**

因为严蔚敏教材的 `Status` 设计哲学是：**每个函数都要报告成功/失败**。

嵌套 if 链的调用流程：

```
visit(T) 失败了 → 不继续遍历 → 返回 ERROR
visit(T) 成功了 → 继续遍历左子树
   左子树遍历失败 → 不遍历右子树 → 返回 ERROR
   左子树遍历成功 → 继续遍历右子树
       右子树遍历成功 → 返回 OK
       右子树遍历失败 → 返回 ERROR
```

这被称为**防御式错误传播**——像"多米诺骨牌"，任何一块倒下（返回 ERROR），整个遍历就停止并报告失败。

---

## 9. 优化总结

我对你的代码做了 5 项优化：

| # | 优化内容 | 文件 | 效果 |
|---|----------|------|------|
| 1 | 删除 .c 中重复的类型定义和 struct，改用 `#include "BiTree.h"` | BiTree.c | 消除重复，符合 C 规范实践 |
| 2 | 删除未使用的 `#include <stdbool.h>` | BiTree.c | 减少无用依赖 |
| 3 | 遍历函数形参 `VisitNode` → `visit` | BiTree.c | 消除与全局函数 `VisitNode` 的重名，代码更清晰 |
| 4 | 替换子树后 `FreeBiTree(oldRight)` | BiTree.c | **修复内存泄漏** |
| 5 | -Wall -Wextra -std=c99 编译通过 | 编译验证 | 零警告零错误 |

**运行结果验证**：所有输出与优化前完全一致，功能不受影响。

---

## 10. 自测 5 题

> 每道题的答案在后面，先自己想再看。

### 题 1：遍历顺序

对下面这棵树，写出先序、中序、后序的遍历结果：

```
       A
      / \
     B   C
    /   / \
   D   E   F
```

### 题 2：递归基

`BiTreeLeafCount` 函数中，为什么需要两个 `if` 而不是一个？去掉 `if (!T) return 0;` 会怎样？

### 题 3：查找顺序

如果一棵二叉树中有两个结点的值都是 5，`FindNode` 会返回哪一个？如果想找第二个 5 该怎么办？

### 题 4：理解递归

用一句话说明为什么 `BiTreeDepth(NULL)` 返回 0 而不是 1？

### 题 5：错误传播

在 `InOrderTrav` 中，如果 `InOrderTrav(T->lchild, visit)` 返回了 ERROR，后面的代码会怎样执行？

---

<details>
<summary>点击展开答案</summary>

### 答 1

```
先序: A B D C E F
中序: D B A E C F
后序: D B E F C A
```

### 答 2

如果去掉 `if (!T) return 0;`，当递归到叶子结点的左右孩子（都是 NULL）时，会执行 `if (!T->lchild && !T->rchild)` → 访问 `NULL->lchild` → **段错误崩溃**。

两个递归基分别是：
- `T == NULL`：空指针保护
- `T 是叶子`：计数加 1

顺序不能颠倒。

### 答 3

`FindNode` 执行**先序查找**（先根、再左、再右），所以返回先被访问到的那一个。

要找第二个 5，需要修改函数：找到后不返回，继续搜索，或传入计数器/列表收集所有匹配。

### 答 4

空树没有结点，所以深度为 0。如果返回 1，那 `BiTreeDepth(NULL)` 会得到 1，然后任何树的深度都会多算 1，公式 `depth = max(left, right) + 1` 就不对了。

### 答 5

短路了——`InOrderTrav(T->rchild, visit)` 不会执行，函数直接 `return ERROR`。这就是错误传播的含义：任何子任务失败，整个任务立即失败。

</details>

---

> 写代码的人：贺明贤 | 解析生成：哈基蜂 🐝
> 时间：2026 年 6 月
