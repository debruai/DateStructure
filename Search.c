// 数据结构 第九章 查找
//
// 三种查找方法：
//   1. 静态查找表 — 顺序/折半查找（表结构不变）
//   2. 动态查找表 — 二叉排序树 BST（可增删）
//   3. 平衡二叉树 AVL（BST的改进，自动保持平衡）

#include "Search.h"

//===============================================================
//  静态查找表
//===============================================================

//---------------------------------------------------------------
//  CreateTable — 创建静态查找表
//---------------------------------------------------------------
void CreateTable(SSTable* ST, int keys[], int n)
{
    if (n > MAXSIZE) n = MAXSIZE;
    ST->length = n;
    for (int i = 1; i <= n; i++)
    {
        ST->r[i] = keys[i - 1];
    }
}

//---------------------------------------------------------------
//  SeqSearch — 顺序查找（哨兵）
//
//  普通顺序查找每轮要比较两次：i<length 和 r[i]==key
//  哨兵优化：把 key 放在 r[0]，从后往前找
//  找到 key 即停，位置就是下标
//  如果最终停在 0，说明没找到
//
//  为什么用哨兵？
//     省掉"下标是否越界"的判断，比较次数减半
//---------------------------------------------------------------
int SeqSearch(SSTable* ST, int key)
{
    ST->r[0] = key;          // 哨兵放在 0 号位置
    int i = ST->length;

    while (ST->r[i] != key)  // 从后往前找
    {
        i--;
    }

    return i;  // 找到返回位置，i=0 表示没找到
}

//---------------------------------------------------------------
//  BinarySearch — 折半查找（迭代版）
//
//  前提：表必须有序（从小到大）
//
//  算法步骤：
//     1. low=1, high=length
//     2. mid = (low+high)/2
//     3. 如果 key == r[mid] → 找到了
//     4. 如果 key < r[mid] → high = mid-1（查左半）
//     5. 如果 key > r[mid] → low = mid+1（查右半）
//     6. 重复直到 low > high
//
//  时间复杂度：O(log n)
//---------------------------------------------------------------
int BinarySearch(SSTable* ST, int key)
{
    int low = 1;
    int high = ST->length;

    while (low <= high)
    {
        int mid = (low + high) / 2;

        if (key == ST->r[mid])
        {
            return mid;  // 找到了
        }
        else if (key < ST->r[mid])
        {
            high = mid - 1;  // 在左半
        }
        else
        {
            low = mid + 1;   // 在右半
        }
    }

    return 0;  // 没找到
}

//---------------------------------------------------------------
//  BinarySearchRecur — 折半查找（递归版）
//---------------------------------------------------------------
int BinarySearchRecur(SSTable* ST, int key, int low, int high)
{
    if (low > high) return 0;  // 递归终止

    int mid = (low + high) / 2;

    if (key == ST->r[mid])
    {
        return mid;
    }
    else if (key < ST->r[mid])
    {
        return BinarySearchRecur(ST, key, low, mid - 1);
    }
    else
    {
        return BinarySearchRecur(ST, key, mid + 1, high);
    }
}

//===============================================================
//  二叉排序树 BST
//===============================================================
//
//  BST 的性质（重要！）：
//     左子树所有节点 < 根节点 < 右子树所有节点
//     中序遍历 BST 得到递增有序序列
//
//  查找效率：
//     最好 O(log n) — 平衡时
//     最坏 O(n) — 退化为单链表

//---------------------------------------------------------------
//  BSTInsert — BST 插入
//
//  返回 OK（插入成功）或 ERROR（key已存在）
//---------------------------------------------------------------
Status BSTInsert(BSTree* T, int key)
{
    // 找到空位 → 插入
    if (*T == NULL)
    {
        *T = (BSTree)malloc(sizeof(BSTNode));
        if (!*T) exit(OVERFLOW);
        (*T)->key = key;
        (*T)->lchild = NULL;
        (*T)->rchild = NULL;
        return OK;
    }

    // 递归查找插入位置
    if (key < (*T)->key)
    {
        return BSTInsert(&(*T)->lchild, key);
    }
    else if (key > (*T)->key)
    {
        return BSTInsert(&(*T)->rchild, key);
    }
    else
    {
        return ERROR;  // 已存在，不插入
    }
}

//---------------------------------------------------------------
//  BSTSearch — BST 查找
//
//  返回节点指针，没找到返回 NULL
//---------------------------------------------------------------
BSTree BSTSearch(BSTree T, int key)
{
    if (T == NULL) return NULL;

    if (key == T->key)
    {
        return T;
    }
    else if (key < T->key)
    {
        return BSTSearch(T->lchild, key);
    }
    else
    {
        return BSTSearch(T->rchild, key);
    }
}

//---------------------------------------------------------------
//  BSTDelete — BST 删除（三种情况）
//
//  删除节点有三种情况：
//     1. 叶子节点 → 直接删
//     2. 只有左/右子树 → 用孩子替代
//     3. 有左右子树 → 用中序前驱（或后继）替代，再删前驱
//
//  教材算法 9.3
//---------------------------------------------------------------
Status BSTDelete(BSTree* T, int key)
{
    if (*T == NULL) return ERROR;  // 没找到

    if (key < (*T)->key)
    {
        return BSTDelete(&(*T)->lchild, key);
    }
    else if (key > (*T)->key)
    {
        return BSTDelete(&(*T)->rchild, key);
    }
    else
    {
        // 找到要删除的节点了
        BSTree q = *T;

        // 情况1和2：至少有一个子树为空
        if ((*T)->lchild == NULL)
        {
            *T = (*T)->rchild;  // 右子树顶上（或 NULL）
            free(q);
        }
        else if ((*T)->rchild == NULL)
        {
            *T = (*T)->lchild;  // 左子树顶上
            free(q);
        }
        else
        {
            // 情况3：左右子树都不为空
            // 找中序前驱（左子树中最右边的节点）
            BSTree s = (*T)->lchild;
            BSTree parent = *T;

            while (s->rchild)
            {
                parent = s;
                s = s->rchild;
            }

            // 用前驱的值覆盖当前节点
            (*T)->key = s->key;

            // 删除前驱节点
            if (parent == *T)
            {
                parent->lchild = s->lchild;
            }
            else
            {
                parent->rchild = s->lchild;
            }
            free(s);
        }
        return OK;
    }
}

//---------------------------------------------------------------
//  InOrderBST — BST 中序遍历（递增有序）
//---------------------------------------------------------------
void InOrderBST(BSTree T)
{
    if (T == NULL) return;
    InOrderBST(T->lchild);
    printf("%d ", T->key);
    InOrderBST(T->rchild);
}

//---------------------------------------------------------------
//  PrintBST — 横向打印 BST 树形
//---------------------------------------------------------------
void PrintBST(BSTree T, int depth)
{
    if (T == NULL) return;

    PrintBST(T->rchild, depth + 1);

    for (int i = 0; i < depth; i++) printf("    ");
    printf("%d\n", T->key);

    PrintBST(T->lchild, depth + 1);
}

//---------------------------------------------------------------
//  FreeBST — 释放 BST
//---------------------------------------------------------------
void FreeBST(BSTree T)
{
    if (T == NULL) return;
    FreeBST(T->lchild);
    FreeBST(T->rchild);
    free(T);
}

//===============================================================
//  平衡二叉树 AVL
//===============================================================
//
//  为什么需要 AVL？
//     BST 可能退化成链表（如插入有序序列），查找变 O(n)
//     AVL 保证任一节点的左右子树高度差 ≤ 1
//
//  平衡因子 bf = 左子树高度 - 右子树高度
//  合法值：-1, 0, 1
//  如果 |bf| > 1，需要旋转调整

//---------------------------------------------------------------
//  R_Rotate — 右旋（处理 LL 型失衡）
//
//  失衡情况：某个节点 A 的 bf=2，且左孩子 B 的 bf=1
//  即：在左孩子的左子树中插入了节点
//
//        A ← 失衡节点                    B
//       .  .                           .  .
//      B   AR       右旋 →          C   A
//     .  .                               .  .
//    C   BR                             BR AR
//---------------------------------------------------------------
void R_Rotate(AVLTree* T)
{
    AVLTree L = (*T)->lchild;   // L 是左孩子
    (*T)->lchild = L->rchild;   // 把 L 的右子树给 T 的左边
    L->rchild = *T;             // T 变成 L 的右子树
    *T = L;                     // L 成了新根
}

//---------------------------------------------------------------
//  L_Rotate — 左旋（处理 RR 型失衡）
//
//  失衡情况：某个节点 A 的 bf=-2，且右孩子 B 的 bf=-1
//  即：在右孩子的右子树中插入了节点
//
//    A ← 失衡节点                  B
//   .  .                         .  .
//  AL  B       左旋 →          A   C
//     .  .                   .  .
//    BL  C                  AL  BL
//---------------------------------------------------------------
void L_Rotate(AVLTree* T)
{
    AVLTree R = (*T)->rchild;   // R 是右孩子
    (*T)->rchild = R->lchild;   // 把 R 的左子树给 T 的右边
    R->lchild = *T;             // T 变成 R 的左子树
    *T = R;                     // R 成了新根
}

//---------------------------------------------------------------
//  LeftBalance — 左平衡处理
//
//  当 T 的 bf=2（左子树过高）时，判断是 LL 还是 LR
//
//  分两种情况：
//     LL：左孩子的 bf=1  → 单次右旋
//     LR：左孩子的 bf=-1 → 先左旋左孩子，再右旋 T
//
//  旋转后需要根据原始 bf 更新平衡因子
//---------------------------------------------------------------
void LeftBalance(AVLTree* T)
{
    AVLTree L = (*T)->lchild;

    switch (L->bf)
    {
        case 1:  // LL 型 — 在左孩子的左子树插入
            (*T)->bf = 0;
            L->bf = 0;
            R_Rotate(T);  // 右旋
            break;

        case -1:  // LR 型 — 在左孩子的右子树插入
        {
            AVLTree Lr = L->rchild;  // Lr 是 L 的右孩子

            // 根据 Lr 原来的 bf 决定调整后的 bf
            switch (Lr->bf)
            {
                case 1:
                    (*T)->bf = -1;
                    L->bf = 0;
                    break;
                case -1:
                    (*T)->bf = 0;
                    L->bf = 1;
                    break;
                case 0:
                    (*T)->bf = 0;
                    L->bf = 0;
                    break;
            }
            Lr->bf = 0;

            L_Rotate(&(*T)->lchild);  // 先左旋左孩子
            R_Rotate(T);               // 再右旋 T
            break;
        }
    }
}

//---------------------------------------------------------------
//  RightBalance — 右平衡处理
//
//  当 T 的 bf=-2（右子树过高）时，判断是 RR 还是 RL
//
//  分两种情况：
//     RR：右孩子的 bf=-1 → 单次左旋
//     RL：右孩子的 bf=1  → 先右旋右孩子，再左旋 T
//---------------------------------------------------------------
void RightBalance(AVLTree* T)
{
    AVLTree R = (*T)->rchild;

    switch (R->bf)
    {
        case -1:  // RR 型 — 在右孩子的右子树插入
            (*T)->bf = 0;
            R->bf = 0;
            L_Rotate(T);  // 左旋
            break;

        case 1:  // RL 型 — 在右孩子的左子树插入
        {
            AVLTree Rl = R->lchild;  // Rl 是 R 的左孩子

            switch (Rl->bf)
            {
                case 1:
                    (*T)->bf = 0;
                    R->bf = -1;
                    break;
                case -1:
                    (*T)->bf = 1;
                    R->bf = 0;
                    break;
                case 0:
                    (*T)->bf = 0;
                    R->bf = 0;
                    break;
            }
            Rl->bf = 0;

            R_Rotate(&(*T)->rchild);  // 先右旋右孩子
            L_Rotate(T);               // 再左旋 T
            break;
        }
    }
}

//---------------------------------------------------------------
//  AVLInsert — AVL 插入（递归）
//
//  参数：
//     T      — 树的指针（二级指针，可能修改根）
//     key    — 要插入的关键字
//     taller — 输出参数，表示本层子树是否长高了
//
//  在递归返回的过程中，逐层检查平衡因子
//  如果发现失衡，立即旋转调整
//---------------------------------------------------------------
Status AVLInsert(AVLTree* T, int key, int* taller)
{
    if (*T == NULL)
    {
        // 找到空位，创建新节点
        *T = (AVLTree)malloc(sizeof(AVLNode));
        if (!*T) exit(OVERFLOW);
        (*T)->key = key;
        (*T)->bf = 0;          // 新节点平衡因子为 0
        (*T)->lchild = NULL;
        (*T)->rchild = NULL;
        *taller = 1;           // 告诉父节点：我长高了
        return OK;
    }

    if (key < (*T)->key)
    {
        // 插入左子树
        if (!AVLInsert(&(*T)->lchild, key, taller))
        {
            return ERROR;  // 已存在
        }

        if (*taller)  // 左子树长高了
        {
            switch ((*T)->bf)
            {
                case 1:   // 原来左子树高，现在又长 → 左失衡！
                    LeftBalance(T);
                    *taller = 0;  // 旋转后高度不变
                    break;
                case 0:   // 原来平衡，左子树变高
                    (*T)->bf = 1;
                    *taller = 1;  // 继续告诉上一层
                    break;
                case -1:  // 原来右子树高，现在左子树变高 → 平衡了
                    (*T)->bf = 0;
                    *taller = 0;
                    break;
            }
        }
    }
    else if (key > (*T)->key)
    {
        // 插入右子树
        if (!AVLInsert(&(*T)->rchild, key, taller))
        {
            return ERROR;
        }

        if (*taller)  // 右子树长高了
        {
            switch ((*T)->bf)
            {
                case 1:   // 原来左子树高，现在右子树变高 → 平衡了
                    (*T)->bf = 0;
                    *taller = 0;
                    break;
                case 0:   // 原来平衡，右子树变高
                    (*T)->bf = -1;
                    *taller = 1;
                    break;
                case -1:  // 原来右子树高，现在又长 → 右失衡！
                    RightBalance(T);
                    *taller = 0;
                    break;
            }
        }
    }
    else
    {
        // key 已存在
        *taller = 0;
        return ERROR;
    }

    return OK;
}

//---------------------------------------------------------------
//  InOrderAVL — AVL 中序遍历
//---------------------------------------------------------------
void InOrderAVL(AVLTree T)
{
    if (T == NULL) return;
    InOrderAVL(T->lchild);
    printf("%d(bf=%d) ", T->key, T->bf);
    InOrderAVL(T->rchild);
}

//---------------------------------------------------------------
//  PrintAVL — 横向打印 AVL 树形（含平衡因子）
//---------------------------------------------------------------
void PrintAVL(AVLTree T, int depth)
{
    if (T == NULL) return;
    PrintAVL(T->rchild, depth + 1);
    for (int i = 0; i < depth; i++) printf("    ");
    printf("%d[%d]\n", T->key, T->bf);
    PrintAVL(T->lchild, depth + 1);
}

//---------------------------------------------------------------
//  FreeAVL — 释放 AVL 树
//---------------------------------------------------------------
void FreeAVL(AVLTree T)
{
    if (T == NULL) return;
    FreeAVL(T->lchild);
    FreeAVL(T->rchild);
    free(T);
}

//===============================================================
//  main — 测试查找
//===============================================================
int main()
{
    printf("======= 查找测试 =======\n\n");

    //========================================
    //  第一部分：静态查找表
    //========================================
    printf("========== 一、静态查找表 ==========\n\n");

    int keys[] = {5, 13, 19, 21, 37, 56, 64, 75, 80, 88, 92};
    int n = 11;

    SSTable ST;
    CreateTable(&ST, keys, n);

    printf("1. 顺序查找（哨兵）：\n");
    printf("   查找表：");
    for (int i = 1; i <= n; i++) printf("%d ", ST.r[i]);
    printf("\n");
    int pos = SeqSearch(&ST, 37);
    printf("   查找 37，位置：%d\n", pos);
    pos = SeqSearch(&ST, 99);
    printf("   查找 99，位置：%d（0表示没找到）\n", pos);

    printf("\n2. 折半查找（迭代）：\n");
    pos = BinarySearch(&ST, 37);
    printf("   查找 37，位置：%d\n", pos);
    pos = BinarySearch(&ST, 1);
    printf("   查找 1，位置：%d\n", pos);

    printf("\n3. 折半查找（递归）：\n");
    pos = BinarySearchRecur(&ST, 37, 1, ST.length);
    printf("   查找 37，位置：%d\n", pos);

    //========================================
    //  第二部分：二叉排序树 BST
    //========================================
    printf("\n========== 二、二叉排序树 BST ==========\n\n");

    int bstKeys[] = {45, 24, 53, 12, 28, 90};
    int bstN = 6;

    BSTree BST = NULL;
    printf("1. 依次插入：");
    for (int i = 0; i < bstN; i++)
    {
        printf("%d ", bstKeys[i]);
        BSTInsert(&BST, bstKeys[i]);
    }
    printf("\n");

    printf("\n2. 中序遍历（递增有序）：\n   ");
    InOrderBST(BST);
    printf("\n");

    printf("\n3. BST 树形结构：\n");
    PrintBST(BST, 0);

    printf("\n4. 查找测试：\n");
    BSTree found = BSTSearch(BST, 28);
    printf("   查找 28：%s\n", found ? "找到 ✅" : "未找到 ❌");
    found = BSTSearch(BST, 99);
    printf("   查找 99：%s\n", found ? "找到 ✅" : "未找到 ❌");

    printf("\n5. 删除测试：\n");
    printf("   删除 24（有左右子树）：\n");
    BSTDelete(&BST, 24);
    printf("   删除后中序遍历：");
    InOrderBST(BST);
    printf("\n");
    printf("   删除 28（叶子节点）：\n");
    BSTDelete(&BST, 28);
    printf("   删除后中序遍历：");
    InOrderBST(BST);
    printf("\n");

    FreeBST(BST);

    //========================================
    //  第三部分：平衡二叉树 AVL
    //========================================
    printf("\n========== 三、平衡二叉树 AVL ==========\n\n");

    // 教材例 9.3：{13, 24, 37, 90, 53}
    // 这个序列会依次触发 LL、LR、RR、RL 四种旋转
    int avlKeys[] = {13, 24, 37, 90, 53};
    int avlN = 5;

    AVLTree AVL = NULL;
    int taller = 0;

    printf("1. 依次插入并保持平衡：\n");
    for (int i = 0; i < avlN; i++)
    {
        taller = 0;
        AVLInsert(&AVL, avlKeys[i], &taller);
        printf("   插入 %2d → 中序: ", avlKeys[i]);
        InOrderAVL(AVL);
        printf("\n");
    }

    printf("\n2. AVL 树形（节点[平衡因子]）：\n");
    PrintAVL(AVL, 0);

    printf("\n3. 中序遍历：\n   ");
    InOrderAVL(AVL);
    printf("\n");

    // 再插入一些元素演示进一步平衡
    printf("\n4. 继续插入 {14, 8, 6} 演示更多旋转：\n");
    int moreKeys[] = {14, 8, 6};
    for (int i = 0; i < 3; i++)
    {
        taller = 0;
        AVLInsert(&AVL, moreKeys[i], &taller);
        printf("   插入 %2d → 中序: ", moreKeys[i]);
        InOrderAVL(AVL);
        printf("\n");
    }

    printf("\n5. 最终 AVL 树形：\n");
    PrintAVL(AVL, 0);

    printf("\n6. AVL vs BST 对比：\n");
    printf("   如果按 %d %d %d %d %d 的次序插到 BST 中\n",
           avlKeys[0], avlKeys[1], avlKeys[2], avlKeys[3], avlKeys[4]);
    printf("   BST 会退化为接近链表的形状\n");
    printf("   AVL 通过旋转始终保持 log n 的高度\n");

    FreeAVL(AVL);

    printf("\n======= 测试完成 =======\n");

    return 0;
}
