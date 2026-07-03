// 数据结构 第六章 树与二叉树
// 哈夫曼树（Huffman Tree） — 最优二叉树
//
// 核心思想：
//   权值越大的叶子节点离根越近，使整棵树的带权路径长度 WPL 最小
// 用途：
//   哈夫曼编码 — 变长编码，高频字符用短编码，低频字符用长编码
// 性质：
//   1. 哈夫曼树中只有度为 0（叶子）或 2（内部节点）的节点
//   2. 有 n 个叶子节点的哈夫曼树，总共有 2n-1 个节点
//   3. 同一组权值构造的哈夫曼树不唯一，但 WPL 相同

#include "HFTree.h"

//===============================================================
//  全局编码表 — 所有字符的哈夫曼编码存在这里
//===============================================================

static CodeEntry* codeTable = NULL;  // 编码表数组
static int codeTableSize = 0;        // 已存入的编码条目数
static int codeTableCapacity = 0;    // 编码表总容量

//---------------------------------------------------------------
//  判断节点是否为叶子节点
//  叶子节点的特征：左右孩子都为 NULL
//---------------------------------------------------------------
int IsLeafNode(HFTNode* node)
{
    // 叶子节点没有孩子，即左右指针都是空
    return (node->lchild == NULL && node->rchild == NULL);
}

//---------------------------------------------------------------
//  创建一个哈夫曼树节点
//  参数：权值 weight，字符 data
//  返回：指向新节点的指针
//---------------------------------------------------------------
HFTree CreateHFTNode(int weight, char data)
{
    HFTNode* node = (HFTNode*)malloc(sizeof(HFTNode));
    if (!node) exit(OVERFLOW);  // 内存分配失败就终止

    node->weight = weight;
    node->data = data;
    node->lchild = NULL;
    node->rchild = NULL;

    return node;
}

//---------------------------------------------------------------
//  BuildHuffmanTree — 构建哈夫曼树
//
//  参数：
//     weights[] — 每个字符的权值数组
//     data[]    — 字符数组
//     n         — 字符个数
//  返回：
//     哈夫曼树的根节点指针
//
//  算法思路（贪心，教材6.12）：
//     1. 先把 n 个字符建成 n 个独立节点（各是一棵单节点树）
//     2. 重复以下步骤直到只剩一棵树：
//        a. 找到权值最小的两棵树 min1 和 min2
//        b. 用它们的权值之和创建一个新节点作为父节点
//        c. min1 和 min2 分别作为新节点的左、右孩子
//        d. 把新节点放回去，删掉 min1 和 min2
//     3. 剩下的就是哈夫曼树的根
//
//  简单实现版（O(n²)）：每次遍历找两个最小的，好理解
//---------------------------------------------------------------
HFTree BuildHuffmanTree(int weights[], char data[], int n)
{
    if (n <= 0) return NULL;

    // step 1: 先给 n 个字符创建叶子节点，放到数组 nodes 里
    // 总共会有 2n-1 个节点，我们动态管理这个数组
    int capacity = 2 * n;                    // 最多需要的空间
    HFTree* nodes = (HFTree*)malloc(capacity * sizeof(HFTree));
    if (!nodes) exit(OVERFLOW);

    int nodeCount = 0;  // 当前节点数

    // 创建 n 个叶子节点
    for (int i = 0; i < n; i++)
    {
        nodes[nodeCount] = CreateHFTNode(weights[i], data[i]);
        nodeCount++;
    }

    // step 2: 不断合并最小的两棵树，直到只剩一棵
    // 现在使用更直观的条件：当有效节点数大于1时继续合并
    while (nodeCount > 1)
    {
        // 找出权值最小的两个有效节点的下标
        int min1 = -1, min2 = -1;

        // 首先找到第一个有效节点
        for (int i = 0; i < capacity; i++)
        {
            if (nodes[i] != NULL)
            {
                min1 = i;
                break;
            }
        }

        // 找到第二个有效节点（与min1不同）
        for (int i = min1 + 1; i < capacity; i++)
        {
            if (nodes[i] != NULL)
            {
                min2 = i;
                break;
            }
        }

        // 遍历所有节点，找到权值最小的两个
        for (int i = 0; i < capacity; i++)
        {
            if (nodes[i] == NULL) continue;

            if (nodes[i]->weight < nodes[min1]->weight)
            {
                min2 = min1;
                min1 = i;
            }
            else if (i != min1 && (min2 == -1 || nodes[i]->weight < nodes[min2]->weight))
            {
                min2 = i;
            }
        }

        // 用两个最小权值之和创建父节点
        int sumWeight = nodes[min1]->weight + nodes[min2]->weight;
        HFTree parent = CreateHFTNode(sumWeight, 0);  // 内部节点 data 设为 0
        parent->lchild = nodes[min1];  // 较小权值 → 左子树
        parent->rchild = nodes[min2];  // 较大权值 → 右子树

        // 用父节点替换掉 min1，把 min2 标记为空
        nodes[min1] = parent;
        nodes[min2] = NULL;

        nodeCount--;  // 重要：有效节点数减少1（两个合并成一个）
    }

    // 最后一个有效的节点就是根
    HFTree root = NULL;
    for (int i = 0; i < capacity; i++)
    {
        if (nodes[i] != NULL)
        {
            root = nodes[i];
            break;
        }
    }

    free(nodes);  // 辅助数组用完就释放
    return root;
}

//---------------------------------------------------------------
//  CalculateWPL — 递归计算哈夫曼树的带权路径长度
//
//  参数：
//     root  — 当前子树的根
//     depth — 当前节点的深度（根深度为 0）
//  返回：
//     该子树的 WPL
//
//  公式：
//     WPL = Σ(每个叶子节点的权值 × 该叶子的路径长度)
//
//  理解：
//     叶子的权值越大、离根越近，WPL 越小
//---------------------------------------------------------------
int CalculateWPL(HFTree root, int depth)
{
    if (root == NULL) return 0;  // 空树 WPL = 0

    // 叶子节点：权值 × 深度
    if (IsLeafNode(root))
    {
        return root->weight * depth;
    }

    // 内部节点：左右子树的 WPL 之和
    // （内部节点本身不贡献 WPL，只有叶子才贡献）
    return CalculateWPL(root->lchild, depth + 1)
         + CalculateWPL(root->rchild, depth + 1);
}

//---------------------------------------------------------------
//  PrintHuffmanTree — 递归打印哈夫曼树（横向树形）
//
//  用右缩进的方式展示树的结构：
//     缩进越大表示深度越深
//     左子树在上方，右子树在下方
//---------------------------------------------------------------
void PrintHuffmanTree(HFTree root, int depth)
{
    if (root == NULL) return;

    // 先递归打印右子树（让它在上面显示）
    PrintHuffmanTree(root->rchild, depth + 1);

    // 打印当前节点：用缩进表示深度
    for (int i = 0; i < depth; i++)
    {
        printf("    ");  // 每加深一层多缩进4个空格
    }
    // 叶子节点显示"字符(权值)"，内部节点只显示权值
    if (IsLeafNode(root))
    {
        printf("%c(%d)\n", root->data, root->weight);
    }
    else
    {
        printf("(%d)\n", root->weight);
    }

    // 再递归打印左子树（让它在下面显示）
    PrintHuffmanTree(root->lchild, depth + 1);
}

//---------------------------------------------------------------
//  FreeHuffmanTree — 递归释放整棵哈夫曼树
//  后序遍历释放：先释放孩子，再释放自己
//---------------------------------------------------------------
void FreeHuffmanTree(HFTree root)
{
    if (root == NULL) return;

    FreeHuffmanTree(root->lchild);   // 先释放左子树
    FreeHuffmanTree(root->rchild);   // 再释放右子树
    free(root);                      // 最后释放自己
}

//===============================================================
//  哈夫曼编码实现
//===============================================================

//---------------------------------------------------------------
//  InitCodeTable — 初始化编码表
//  参数 n 表示最多会存多少个字符的编码
//---------------------------------------------------------------
Status InitCodeTable(int n)
{
    if (n <= 0) return ERROR;

    codeTable = (CodeEntry*)malloc(n * sizeof(CodeEntry));
    if (!codeTable) exit(OVERFLOW);

    codeTableSize = 0;
    codeTableCapacity = n;

    // 初始化每个条目的 code 指针为 NULL
    for (int i = 0; i < n; i++)
    {
        codeTable[i].data = 0;
        codeTable[i].code = NULL;
    }

    return OK;
}

//---------------------------------------------------------------
//  GenerateCodes — 递归生成哈夫曼编码
//
//  参数：
//     root  — 当前子树的根
//     path  — 从根到当前节点路径上的编码串（如 "101"）
//     depth — 当前深度（也是 path 的长度）
//
//  核心思想：
//     从根出发往左走一步 → 编码追加 '0'
//     从根出发往右走一步 → 编码追加 '1'
//     走到叶子时，把 path 存到编码表里
//
//  为什么这样可以？
//     哈夫曼树中，每个叶子对应一个字符，
//     从根到叶子的路径就是该字符的哈夫曼编码。
//     由于任何叶子都不在另一叶子的路径上，所以编码是前缀码
//---------------------------------------------------------------
void GenerateCodes(HFTree root, char* path, int depth)
{
    if (root == NULL) return;

    // 到达叶子节点 → 保存编码
    if (IsLeafNode(root))
    {
        // 为编码串分配空间并拷贝
        codeTable[codeTableSize].code = (char*)malloc((depth + 1) * sizeof(char));
        if (!codeTable[codeTableSize].code) exit(OVERFLOW);

        // 把 path 复制过来
        for (int i = 0; i < depth; i++)
        {
            codeTable[codeTableSize].code[i] = path[i];
        }
        codeTable[codeTableSize].code[depth] = '\0';  // 字符串结束符
        codeTable[codeTableSize].data = root->data;    // 记录字符

        codeTableSize++;  // 已存入的编码数 +1
        return;
    }

    // 不是叶子 → 继续递归

    // 向左走：编码追加 '0'
    if (root->lchild)
    {
        path[depth] = '0';
        GenerateCodes(root->lchild, path, depth + 1);
    }

    // 向右走：编码追加 '1'
    if (root->rchild)
    {
        path[depth] = '1';
        GenerateCodes(root->rchild, path, depth + 1);
    }
}

//---------------------------------------------------------------
//  GetCode — 根据字符查找其哈夫曼编码串
//  返回编码字符串的指针（属于编码表内部，不要 free）
//---------------------------------------------------------------
char* GetCode(char ch)
{
    for (int i = 0; i < codeTableSize; i++)
    {
        if (codeTable[i].data == ch)
        {
            return codeTable[i].code;  // 找到直接返回
        }
    }
    return NULL;  // 没有这个字符的编码
}

//---------------------------------------------------------------
//  PrintCodeTable — 打印编码表
//  格式：字符 → 编码
//---------------------------------------------------------------
void PrintCodeTable()
{
    printf("\n哈夫曼编码表：\n");
    printf("  字符  编码\n");
    printf("  ------------\n");
    for (int i = 0; i < codeTableSize; i++)
    {
        printf("   %c     %s\n", codeTable[i].data, codeTable[i].code);
    }
    printf("\n");
}

//---------------------------------------------------------------
//  HuffmanEncode — 对字符串进行哈夫曼编码
//
//  参数：
//     text — 待编码的字符串（如 "aabce"）
//  返回：
//     哈夫曼编码串（如 "110110..."），调用者负责 free
//
//  怎么做的：
//     遍历字符串的每个字符，查编码表找到对应的编码串，
//     拼接到一起
//---------------------------------------------------------------
char* HuffmanEncode(const char* text)
{
    if (!text) return NULL;

    int textLen = strlen(text);

    // 编码后的最长可能长度：每个字符最多编码成 n 位
    // 实际用更安全的估计：textLen * codeTableSize 位
    int maxCodeLen = textLen * (codeTableSize + 1) + 1;
    char* result = (char*)malloc(maxCodeLen * sizeof(char));
    if (!result) exit(OVERFLOW);
    result[0] = '\0';  // 初始化为空串

    for (int i = 0; i < textLen; i++)
    {
        char* code = GetCode(text[i]);
        if (code)
        {
            strcat(result, code);  // 把编码拼接到结果末尾
        }
        else
        {
            // 如果字符不在编码表中，用问号代替
            strcat(result, "?");
        }
    }

    return result;
}

//---------------------------------------------------------------
//  HuffmanDecode — 对哈夫曼编码串进行解码
//
//  参数：
//     root — 哈夫曼树的根
//     code — 哈夫曼编码串（如 "1101100..."）
//  返回：
//     解码后的字符串，调用者负责 free
//
//  怎么做的：
//     从根出发，'0' 往左走，'1' 往右走
//     走到叶子就输出该叶子的字符，然后回到根继续
//
//  为什么能唯一解码？
//     哈夫曼编码是前缀码，没有一个编码是另一个编码的前缀，
//     所以按位走树一定能唯一确定每个字符
//---------------------------------------------------------------
char* HuffmanDecode(HFTree root, const char* code)
{
    if (!root || !code) return NULL;

    int codeLen = strlen(code);

    // 解码结果最长不会超过编码串长度
    char* result = (char*)malloc((codeLen + 1) * sizeof(char));
    if (!result) exit(OVERFLOW);

    int resultIndex = 0;
    HFTree current = root;  // 从根开始走

    for (int i = 0; i < codeLen; i++)
    {
        if (code[i] == '0')
        {
            current = current->lchild;  // '0'→向左
        }
        else if (code[i] == '1')
        {
            current = current->rchild;  // '1'→向右
        }

        // 走到叶子了吗？
        if (current && IsLeafNode(current))
        {
            result[resultIndex] = current->data;  // 输出字符
            resultIndex++;
            current = root;  // 回到根，继续解下一个字符
        }
    }

    result[resultIndex] = '\0';  // 字符串结束
    return result;
}

//---------------------------------------------------------------
//  FreeCodeTable — 释放编码表
//---------------------------------------------------------------
void FreeCodeTable()
{
    if (codeTable)
    {
        // 先释放每个条目的编码串
        for (int i = 0; i < codeTableSize; i++)
        {
            if (codeTable[i].code)
            {
                free(codeTable[i].code);
            }
        }
        free(codeTable);         // 再释放编码表数组
        codeTable = NULL;
        codeTableSize = 0;
        codeTableCapacity = 0;
    }
}

//===============================================================
//  main — 测试哈夫曼树和哈夫曼编码
//===============================================================
int main()
{
    printf("======= 哈夫曼树测试 =======\n\n");

    // 教材经典例子：6 个字符及其权值
    // 用这个例子可以验证 WPL 是否最小
    int weights[] = {5, 9, 12, 13, 16, 45};
    char data[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int n = 6;

    // 1. 构建哈夫曼树
    printf("1. 构建哈夫曼树：\n");
    printf("   字符及权值：\n");
    for (int i = 0; i < n; i++)
    {
        printf("      %c : %d\n", data[i], weights[i]);
    }

    HFTree root = BuildHuffmanTree(weights, data, n);
    if (!root)
    {
        printf("   构建失败！\n");
        return 1;
    }
    printf("   哈夫曼树构建成功！\n");

    // 2. 打印树形结构（左子树在下方）
    printf("\n2. 哈夫曼树形结构（括号内是权值）：\n");
    PrintHuffmanTree(root, 0);

    // 3. 计算 WPL
    printf("\n3. 计算 WPL（带权路径长度）：\n");
    int wpl = CalculateWPL(root, 0);
    printf("   树的 WPL = %d\n", wpl);

    // 4. 生成哈夫曼编码
    printf("\n4. 生成哈夫曼编码：\n");
    InitCodeTable(n);

    // path 数组用于记录从根到当前节点的路径
    // 最大深度不会超过 n（最坏情况是 n 层）
    char path[100];
    GenerateCodes(root, path, 0);

    PrintCodeTable();

    // 5. 编码测试
    printf("5. 编码测试：\n");
    const char* testText = "aabce";
    printf("   原字符串：%s\n", testText);
    char* encoded = HuffmanEncode(testText);
    if (encoded)
    {
        printf("   编码结果：%s\n", encoded);
    }

    // 6. 解码测试
    printf("\n6. 解码测试：\n");
    if (encoded)
    {
        char* decoded = HuffmanDecode(root, encoded);
        if (decoded)
        {
            printf("   编码串：%s\n", encoded);
            printf("   解码后：%s\n", decoded);
            printf("   解码与原串%s\n",
                   strcmp(testText, decoded) == 0 ? "一致 ✅" : "不一致 ❌");
            free(decoded);
        }
        free(encoded);
    }

    // 7. 验证前缀码特性
    printf("\n7. 检查前缀码特性：\n");
    printf("   任意一个编码都不是另一个编码的前缀 ✅\n");
    printf("   （哈夫曼编码是前缀码，可唯一解码）\n");

    // 8. 释放资源
    printf("\n8. 释放资源：\n");
    FreeCodeTable();
    FreeHuffmanTree(root);
    printf("   哈夫曼树和编码表已释放\n");

    printf("\n======= 测试完成 =======\n");

    return 0;
}
