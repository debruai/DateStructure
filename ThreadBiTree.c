// 数据结构 第六章 树与二叉树
// 线索二叉树（Threaded Binary Tree）
//
// 为什么要线索化？
//   普通二叉树有 n+1 个空指针域（n 个节点有 2n 个指针，用了 n-1 个）
//   线索化就是利用这些空指针指向"遍历序列"中的前驱和后继
//
// 核心思想：
//   ltag = 0 → lchild 指向左孩子
//   ltag = 1 → lchild 指向前驱（线索）
//   rtag = 0 → rchild 指向右孩子
//   rtag = 1 → rchild 指向后继（线索）
//
// 好处：
//   线索化后可以不用递归也不用栈就能遍历二叉树
//   考试重点：中序线索化的递归过程、pre 指针的维护

#include "ThreadBiTree.h"

//---------------------------------------------------------------
//  CreateNode — 创建一个二叉树节点
//---------------------------------------------------------------
BiThrTree CreateNode(TElemtype data)
{
    BiThrTree node = (BiThrTree)malloc(sizeof(BiThrNode));
    if (!node) exit(OVERFLOW);

    node->data = data;
    node->lchild = NULL;
    node->rchild = NULL;
    node->ltag = 0;  // 默认都是孩子指针
    node->rtag = 0;

    return node;
}

//---------------------------------------------------------------
//  CreateBiTree — 用先序序列创建二叉树
//
//  参数：
//     preOrder[] — 先序遍历序列，'#' 表示空子树
//     index      — 当前处理的数组下标（指针，用于递归）
//     maxLen     — 数组最大长度
//  返回：
//     构建的二叉树根节点
//
//  例如先序序列 {1, 2, 4, '#', '#', 5, '#', '#', 3, '#', 6, '#', '#'}
//  对应：
//         1
//        / |
//       2   3
//      / |   |
//     4   5   6
//---------------------------------------------------------------
BiThrTree CreateBiTree(TElemtype* preOrder, int* index, int maxLen)
{
    if (*index >= maxLen) return NULL;

    TElemtype val = preOrder[*index];
    (*index)++;  // 下标后移

    if (val == '#') return NULL;  // '#' 表示空子树

    BiThrTree node = CreateNode(val);
    node->lchild = CreateBiTree(preOrder, index, maxLen);
    node->rchild = CreateBiTree(preOrder, index, maxLen);

    return node;
}

//---------------------------------------------------------------
//  InThread — 中序线索化（递归核心算法）
//
//  教材算法 6.6
//
//  参数：
//     p   — 当前处理的节点
//     pre — 指向"刚刚访问过的节点"的指针（指针的指针）
//
//  执行过程（中序遍历框架）：
//     1. 递归线索化左子树
//     2. 处理当前节点 p：
//        a. 如果 p 没有左孩子 → lchild 指向前驱（pre所指节点），ltag 置 1
//        b. 如果 pre 没有右孩子 → pre 的 rchild 指向后继（p），rtag 置 1
//        c. 将 pre 更新为 p（p 成了"刚刚访问过的节点"）
//     3. 递归线索化右子树
//
//  理解要点：
//     "前驱"和"后继"是站在中序遍历序列的角度看的
//     中序序列中，pre 紧挨在 p 的前面
//     所以 p 的 lchild 线索指 pre（前驱）
//      pre 的 rchild 线索指 p（后继）
//---------------------------------------------------------------
void InThread(BiThrTree p, BiThrTree* pre)
{
    if (p == NULL) return;

    // 第1步：线索化左子树
    InThread(p->lchild, pre);

    // 第2步：处理当前节点 p
    // 如果 p 没有左孩子 → 建立前驱线索
    if (p->lchild == NULL)
    {
        p->lchild = *pre;  // 左指针指向前驱（线索）
        p->ltag = 1;       // 标记为线索
    }

    // 如果 pre 没有右孩子 → 建立后继线索
    if (*pre != NULL && (*pre)->rchild == NULL)
    {
        (*pre)->rchild = p;   // pre 的右指针指向后继（p）
        (*pre)->rtag = 1;     // 标记为线索
    }

    // 更新 pre：当前节点 p 变成了"刚刚访问过的节点"
    *pre = p;

    // 第3步：线索化右子树
    InThread(p->rchild, pre);
}

//---------------------------------------------------------------
//  InOrderThreading — 带头结点的中序线索化
//
//  教材算法 6.7
//
//  作用：
//     创建一个头结点 Thrt，让遍历的起点和终点更容易管理
//
//  头结点的设计：
//     Thrt->lchild 指向根节点（非线索）
//     Thrt->rchild 指向"最后一个节点"（线索）
//     中序遍历的第一个节点的 lchild 指向头结点（线索）
//     中序遍历的最后一个节点的 rchild 指向头结点（线索）
//
//  这样设计的好处：
//     从 Thrt 开始遍历，不用关心根在哪里
//     遍历到尾时自动回到 Thrt
//---------------------------------------------------------------
BiThrTree InOrderThreading(BiThrTree T)
{
    // 创建头结点
    BiThrTree Thrt = (BiThrTree)malloc(sizeof(BiThrNode));
    if (!Thrt) exit(OVERFLOW);

    Thrt->ltag = 0;            // 左指针指向根（非线索）
    Thrt->rtag = 1;            // 右指针是线索
    Thrt->rchild = Thrt;       // 先让右指针指向自己（空树时也成立）

    if (T == NULL)
    {
        Thrt->lchild = Thrt;   // 空树时左指针也指向自己
    }
    else
    {
        Thrt->lchild = T;      // 头结点的左孩子指向根
        BiThrTree pre = Thrt;  // pre 初始指向头结点

        InThread(T, &pre);     // 中序线索化整棵树

        // 线索化完成后，pre 指向中序序列的最后一个节点
        // 最后一个节点的右指针应该指向头结点（成环）
        pre->rchild = Thrt;
        pre->rtag = 1;

        // 头结点的右指针指向最后一个节点（成环）
        Thrt->rchild = pre;
    }

    return Thrt;
}

//---------------------------------------------------------------
//  InOrderTraverse — 遍历中序线索二叉树（非递归）
//
//  教材算法 6.8
//
//  为什么不用递归也不用栈？
//     因为线索代替了递归回溯：
//     当走到某个节点发现右孩子是"线索"时，
//     直接沿着线索走到后继，不用栈保存父节点
//
//  遍历步骤：
//     1. 从头结点出发，沿 lchild 找到根
//     2. 沿 lchild（向左下）找到中序序列的第一个节点
//     3. 访问该节点
//     4. 如果有右线索 → 直接沿线索到后继
//     5. 否则从右子树的左下方找下一个
//     6. 回到头结点时结束
//---------------------------------------------------------------
Status InOrderTraverse(BiThrTree Thrt)
{
    if (!Thrt) return ERROR;

    // 从头结点出发，先找到真正的根
    BiThrTree p = Thrt->lchild;

    // 循环直到回到头结点
    while (p != Thrt)
    {
        // 沿左孩子向下走，找到中序序列的第一个节点
        // 注意：只有 ltag=0 时才走左孩子
        // ltag=1 时走的是线索，那说明已经到最左了
        while (p->ltag == 0)
        {
            p = p->lchild;
        }

        // 访问当前节点（中序的第一个节点/后继节点）
        printf("%d ", p->data);

        // 沿右线索走到后继
        // rtag=1 表示右指针是线索，直接沿着走
        // 每走一步都访问，直到右指针不是线索
        while (p->rtag == 1 && p->rchild != Thrt)
        {
            p = p->rchild;
            printf("%d ", p->data);
        }

        // 到这里说明 rtag=0，p 有右子树
        // 转向右子树，下一轮循环会找它的最左下方
        p = p->rchild;
    }

    printf("\n");
    return OK;
}

//---------------------------------------------------------------
//  FreeBiTree — 释放二叉树
//  注意：线索二叉树的释放要忽略线索，只释放真正的孩子节点
//---------------------------------------------------------------
void FreeBiTree(BiThrTree T)
{
    if (T == NULL) return;

    // 只释放真正的孩子（ltag=0 / rtag=0），不追线索
    if (T->ltag == 0)
    {
        FreeBiTree(T->lchild);
    }
    if (T->rtag == 0)
    {
        FreeBiTree(T->rchild);
    }
    free(T);
}

//===============================================================
//  main — 测试线索二叉树
//===============================================================
int main()
{
    printf("======= 线索二叉树测试 =======\n\n");

    // 构建如下二叉树（用先序序列）：
    //         1
    //        / |
    //       2   3
    //      / |   |
    //     4   5   6
    //
    // 中序遍历结果应为：4 2 5 1 3 6
    //
    // 先序序列（'#' 表示空子树）：
    TElemtype preOrder[] = {1, 2, 4, '#', '#', 5, '#', '#', 3, '#', 6, '#', '#'};
    int index = 0;

    // 1. 创建二叉树
    printf("1. 用先序序列创建二叉树：\n");
    BiThrTree root = CreateBiTree(preOrder, &index, 13);
    if (!root)
    {
        printf("   创建失败！\n");
        return 1;
    }
    printf("   二叉树创建成功\n");
    printf("   先序序列：1 2 4 # # 5 # # 3 # 6 # #\n");
    printf("   中序遍历结果应为：4 2 5 1 3 6\n");

    // 2. 中序线索化
    printf("\n2. 中序线索化：\n");
    BiThrTree Thrt = InOrderThreading(root);
    printf("   线索化成功！\n");
    printf("   ltag=1 的节点，其 lchild 指向前驱\n");
    printf("   rtag=1 的节点，其 rchild 指向后继\n");

    // 3. 遍历中序线索二叉树
    printf("\n3. 遍历中序线索二叉树（非递归，无需栈）：\n");
    printf("   中序遍历结果：");
    InOrderTraverse(Thrt);

    // 4. 线索的作用演示
    printf("\n4. 线索的作用：\n");
    printf("   - 不用递归，不用栈，就能完成中序遍历\n");
    printf("   - 遍历过程中直接沿线索跳转，O(n) 时间复杂度\n");
    printf("   - 每个节点被访问一次，线索被走过一次\n");

    // 5. 释放资源
    printf("\n5. 释放二叉树：\n");
    // 注意：用 InOrderThreading 后，Thrt 是头结点
    // 需要先释放真正的树，再释放头结点
    BiThrTree realRoot = Thrt->lchild;
    // 先清除线索，避免重复释放
    // 实际上线索指向的节点都是树中的节点
    // 我们释放真正的树时，需要递归释放 ltag=0 和 rtag=0 的孩子
    // 但头结点需要单独释放
    FreeBiTree(realRoot);
    free(Thrt);
    printf("   二叉树和头结点已释放\n");

    printf("\n======= 测试完成 =======\n");

    return 0;
}
