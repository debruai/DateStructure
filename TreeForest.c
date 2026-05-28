// 数据结构 第六章 树与二叉树
// 树与森林 — 存储结构、遍历、与二叉树的转换
//
// 核心知识点（考试重点）：
//   1. 孩子兄弟表示法：树的通用链式存储
//   2. 树 → 二叉树：同一结构体，不同视角
//   3. 森林 → 二叉树：根连右兄弟
//   4. 逆转换同样重要
//
// 为什么能转换？
//   用"孩子兄弟"表示树时，每个节点只有两个指针
//   （firstchild 和 nextsibling）
//   这和二叉树的（lchild 和 rchild）在物理结构上完全一致！
//   所以"转换"其实就是重新解释指针的含义

#include "TreeForest.h"

//---------------------------------------------------------------
//  CreateCSNode — 创建一个孩子兄弟表示法的节点
//---------------------------------------------------------------
CSTree CreateCSNode(TElemtype data)
{
    CSTree node = (CSTree)malloc(sizeof(CSNode));
    if (!node) exit(OVERFLOW);
    node->data = data;
    node->firstchild = NULL;
    node->nextsibling = NULL;
    return node;
}

//---------------------------------------------------------------
//  FreeTree — 释放树（递归）
//---------------------------------------------------------------
void FreeTree(CSTree T)
{
    if (T == NULL) return;
    FreeTree(T->firstchild);   // 先释放孩子子树
    FreeTree(T->nextsibling);  // 再释放兄弟链表
    free(T);
}

//===============================================================
//  构建示例树和森林
//===============================================================

//---------------------------------------------------------------
//  CreateSampleTree — 构建一棵示例树
//
//  树的结构（教材风格）：
//          A
//       / | |
//       B  C  D
//          / |
//         E   F
//
//  孩子兄弟表示法视角：
//   A → firstchild → B → nextsibling → C → nextsibling → D
//                                       ↓              ↓
//                                      NULL        firstchild
//                                                     ↓
//                                                     E → nextsibling → F
//---------------------------------------------------------------
CSTree CreateSampleTree()
{
    // 先创建所有节点
    CSTree A = CreateCSNode('A');
    CSTree B = CreateCSNode('B');
    CSTree C = CreateCSNode('C');
    CSTree D = CreateCSNode('D');
    CSTree E = CreateCSNode('E');
    CSTree F = CreateCSNode('F');

    // 建立"A的三个孩子：B C D"
    A->firstchild = B;       // B 是 A 的第一个孩子
    B->nextsibling = C;      // C 是 B 的下一个兄弟
    C->nextsibling = D;      // D 是 C 的下一个兄弟

    // 建立"D的两个孩子：E F"
    D->firstchild = E;       // E 是 D 的第一个孩子
    E->nextsibling = F;      // F 是 E 的下一个兄弟

    return A;
}

//---------------------------------------------------------------
//  CreateSampleForest — 构建一片示例森林
//
//  森林 = 三棵独立的树：
//    Tree1:     Tree2:     Tree3:
//      A          C          F
//      |         / |
//      B        D   E
//
//  用数组存放三棵树的根节点
//---------------------------------------------------------------
CSTree* CreateSampleForest(int* forestSize)
{
    *forestSize = 3;

    // Tree1: A → B
    CSTree A = CreateCSNode('A');
    CSTree B = CreateCSNode('B');
    A->firstchild = B;

    // Tree2: C → D → E
    CSTree C = CreateCSNode('C');
    CSTree D = CreateCSNode('D');
    CSTree E = CreateCSNode('E');
    C->firstchild = D;
    D->nextsibling = E;

    // Tree3: F（单节点）
    CSTree F = CreateCSNode('F');

    // 森林数组
    CSTree* forest = (CSTree*)malloc(3 * sizeof(CSTree));
    forest[0] = A;
    forest[1] = C;
    forest[2] = F;

    return forest;
}

//===============================================================
//  树与二叉树的转换（考试核心）
//===============================================================

//---------------------------------------------------------------
//  TreeToBinaryTree — 树 → 二叉树
//
//  转换原则（教材）：
//     原树的 firstchild → 二叉树的 lchild
//     原树的 nextsibling → 二叉树的 rchild
//
//  重要理解：
//     因为孩子兄弟表示法本身就是二叉链表，
//     所以"转换"实质上是"重新解释"指针的含义。
//     物理结构不变，逻辑解释变了！
//
//  对考试来说：画图时
//     1. 兄弟之间加水平连线
//     2. 去掉除第一个孩子外的垂直连线
//     3. 顺时针旋转45°
//---------------------------------------------------------------
CSTree TreeToBinaryTree(CSTree tree)
{
    // 结构上，孩子兄弟树本来就是二叉树的形式
    // 转换只是概念上的，不需要修改指针
    // 所以我们直接返回原指针
    // "在理解上，把 firstchild 读作 lchild，
    //  把 nextsibling 读作 rchild"
    return tree;
}

//---------------------------------------------------------------
//  BinaryTreeToTree — 二叉树 → 树
//
//  逆转换原则：
//     二叉树的 lchild → 原树的 firstchild
//     二叉树的 rchild → 原树的 nextsibling
//
//  和正向转换一样，是概念上的重新解释
//---------------------------------------------------------------
CSTree BinaryTreeToTree(CSTree binaryTree)
{
    // 同样是概念转换，结构不变
    return binaryTree;
}

//---------------------------------------------------------------
//  ForestToBinaryTree — 森林 → 二叉树
//
//  转换原则（教材）：
//     1. 将森林中每棵树分别转换为二叉树
//     2. 第一棵树的根作为整个二叉树的根
//     3. 第二棵树的根作为第一棵树根的右子树
//     4. 第三棵树的根作为第二棵树根的右子树 ...
//
//  概括：森林中树的根之间互为"兄弟"，
//       所以用 nextsibling（右指针）连接
//---------------------------------------------------------------
CSTree ForestToBinaryTree(CSTree* forest, int forestSize)
{
    if (!forest || forestSize <= 0) return NULL;

    // 第一棵树的根作为整个二叉树的根
    CSTree root = forest[0];

    // 后续树的根依次作为前一棵树根的"右孩子"
    CSTree current = root;
    for (int i = 1; i < forestSize; i++)
    {
        current->nextsibling = forest[i];  // 连右兄弟
        current = forest[i];
    }

    return root;
}

//---------------------------------------------------------------
//  BinaryTreeToForest — 二叉树 → 森林
//
//  逆转换原则：
//     从根节点开始，沿 rchild（即 nextsibling）不断拆分
//     每个拆出来的节点都是一棵树的根
//
//  返回：森林数组，forestSize 记录树的数量
//---------------------------------------------------------------
CSTree* BinaryTreeToForest(CSTree binaryTree, int* forestSize)
{
    if (!binaryTree)
    {
        *forestSize = 0;
        return NULL;
    }

    // 第一轮：统计有多少棵树（沿 rchild 数节点）
    int count = 1;
    CSTree p = binaryTree->nextsibling;
    while (p)
    {
        count++;
        p = p->nextsibling;
    }

    *forestSize = count;

    // 分配森林数组
    CSTree* forest = (CSTree*)malloc(count * sizeof(CSTree));
    if (!forest) exit(OVERFLOW);

    // 逐个取出树的根
    p = binaryTree;
    for (int i = 0; i < count; i++)
    {
        forest[i] = p;                    // 当前根放入森林
        CSTree next = p->nextsibling;     // 保存下一个兄弟
        p->nextsibling = NULL;            // 断开兄弟链
        p = next;
    }

    return forest;
}

//===============================================================
//  遍历
//===============================================================

//---------------------------------------------------------------
//  PreOrderTree — 树的先根遍历
//
//  等价于对应二叉树的先序遍历
//  步骤：访问根 → 先根遍历第一个孩子 → 先根遍历下一个兄弟
//---------------------------------------------------------------
void PreOrderTree(CSTree T)
{
    if (T == NULL) return;

    printf("%c ", T->data);            // 访问根节点
    PreOrderTree(T->firstchild);       // 先根遍历第一个孩子
    PreOrderTree(T->nextsibling);      // 先根遍历下一个兄弟
}

//---------------------------------------------------------------
//  PostOrderTree — 树的后根遍历
//
//  等价于对应二叉树的中序遍历
//  步骤：后根遍历第一个孩子 → 访问根 → 后根遍历下一个兄弟
//---------------------------------------------------------------
void PostOrderTree(CSTree T)
{
    if (T == NULL) return;

    PostOrderTree(T->firstchild);      // 后根遍历第一个孩子
    printf("%c ", T->data);            // 访问根节点
    PostOrderTree(T->nextsibling);     // 后根遍历下一个兄弟
}

//---------------------------------------------------------------
//  PrintTree — 按树形打印（横向）
//
//  用缩进表示树层次：
//     深度越深缩进越多
//     先打印兄弟链，再打印孩子子树
//---------------------------------------------------------------
void PrintTree(CSTree T, int depth)
{
    if (T == NULL) return;

    // 先打印兄弟子树（让它显示在同一层）
    PrintTree(T->nextsibling, depth);

    // 打印当前节点
    for (int i = 0; i < depth; i++)
    {
        printf("    ");
    }
    printf("%c\n", T->data);

    // 再打印孩子子树（缩进更深）
    PrintTree(T->firstchild, depth + 1);
}

//===============================================================
//  辅助函数：打印一棵树的孩子兄弟结构（线性展示）
//---------------------------------------------------------------
void PrintCSList(CSTree T)
{
    if (T == NULL) return;

    printf("%c", T->data);

    if (T->firstchild)
    {
        printf("(");
        PrintCSList(T->firstchild);
        printf(")");
    }

    if (T->nextsibling)
    {
        printf(", ");
        PrintCSList(T->nextsibling);
    }
}

//===============================================================
//  main — 测试树与森林
//===============================================================
int main()
{
    printf("======= 树与森林测试 =======\n\n");

    //----------------------------------------
    //  第一部分：树的表示与遍历
    //----------------------------------------
    printf("========== 第一部分：树的表示与遍历 ==========\n\n");

    CSTree tree = CreateSampleTree();
    if (!tree)
    {
        printf("创建树失败！\n");
        return 1;
    }

    printf("1. 创建一棵树（孩子兄弟表示法）：\n");
    printf("          A\n");
    printf("       / | |\n");
    printf("       B  C  D\n");
    printf("          / |\n");
    printf("         E   F\n");

    printf("\n2. 孩子兄弟线性表示：\n   ");
    PrintCSList(tree);
    printf("\n");

    printf("\n3. 树的先根遍历：\n   ");
    PreOrderTree(tree);
    printf("\n");

    printf("\n4. 树的后根遍历：\n   ");
    PostOrderTree(tree);
    printf("\n");

    printf("\n5. 树形结构（横向，上边=兄弟，下边=孩子）：\n");
    PrintTree(tree, 0);
    printf("\n");

    //----------------------------------------
    //  第二部分：树 → 二叉树 转换
    //----------------------------------------
    printf("========== 第二部分：树 ↔ 二叉树转换 ==========\n\n");

    // 重新创建一棵树用于转换演示
    CSTree tree2 = CreateSampleTree();

    printf("6. 树 → 二叉树转换：\n");
    printf("   转换原则：\n");
    printf("     firstchild → lchild（左孩子）\n");
    printf("     nextsibling → rchild（右孩子）\n");
    CSTree binaryTree = TreeToBinaryTree(tree2);
    printf("   转换完成！\n");
    printf("   转换后的二叉树（孩子兄弟表示法）：\n   ");
    PrintCSList(binaryTree);
    printf("\n");
    printf("   \n");
    printf("   重要理解：树和二叉树用的是同一个结构体，\n");
    printf("   转换只是【重新解释】指针的含义，不改变指针本身\n");

    // 二叉树 → 树
    printf("\n7. 二叉树 → 树（逆转换）：\n");
    CSTree treeBack = BinaryTreeToTree(binaryTree);
    printf("   逆转换完成！先根遍历：\n   ");
    PreOrderTree(treeBack);
    printf("\n");

    // 释放
    FreeTree(tree2);

    //----------------------------------------
    //  第三部分：森林 → 二叉树
    //----------------------------------------
    printf("\n========== 第三部分：森林 ↔ 二叉树转换 ==========\n\n");

    int forestSize;
    CSTree* forest = CreateSampleForest(&forestSize);

    printf("8. 原始森林（%d棵树）：\n", forestSize);
    for (int i = 0; i < forestSize; i++)
    {
        printf("   第%d棵树：", i + 1);
        PrintCSList(forest[i]);
        printf("\n");
    }

    printf("\n9. 森林 → 二叉树：\n");
    printf("   转换原则：每棵树的根之间互为右兄弟\n");
    CSTree forestBinary = ForestToBinaryTree(forest, forestSize);
    printf("   转换后的二叉树（先根遍历）：\n   ");
    PreOrderTree(forestBinary);
    printf("\n");

    printf("\n10. 二叉树 → 森林：\n");
    int newForestSize;
    CSTree* newForest = BinaryTreeToForest(forestBinary, &newForestSize);
    printf("    拆分为 %d 棵树：\n", newForestSize);
    for (int i = 0; i < newForestSize; i++)
    {
        printf("    第%d棵：", i + 1);
        PreOrderTree(newForest[i]);
        printf("\n");
    }

    // 释放资源
    printf("\n11. 释放资源：\n");
    for (int i = 0; i < newForestSize; i++)
    {
        FreeTree(newForest[i]);
    }
    free(newForest);
    free(forest);  // forest 被转换后其中的指针已变为 forestBinary
                    // 但 forest 数组本身需要释放
    printf("    所有资源已释放\n");

    printf("\n======= 测试完成 =======\n");

    return 0;
}
