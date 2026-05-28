// 数据结构 第九章 查找
// 散列表（Hash Table / 哈希表）
//
// 核心思想：
//   通过哈希函数 H(key) 将关键字映射到表中的存储位置
//   理想情况下 O(1) 时间完成查找
//
// 两个关键问题：
//   1. 哈希函数的设计（除留余数法最常用）
//   2. 冲突处理（开放定址法 / 链地址法）
//
// 性能指标：
//   装填因子 α = 表中元素数 / 表长
//   α 越大，冲突概率越高，ASL 越大

#include "HashTable.h"

//===============================================================
//  开放定址法 — 通用基础函数
//===============================================================

//---------------------------------------------------------------
//  Hash — 哈希函数，除留余数法
//  H(key) = key % size
//  size 一般取不大于表长的最大质数
//---------------------------------------------------------------
int Hash(int key, int size)
{
    // 处理负数：取绝对值后取模
    return (key >= 0 ? key : -key) % size;
}

//---------------------------------------------------------------
//  LinearProbe — 线性探测再散列
//  di = 1, 2, 3, ... 即逐个往后找空位
//  公式：Hi = (H(key) + di) % size
//---------------------------------------------------------------
int LinearProbe(int di, int size)
{
    return di;  // 第 di 次探测，偏移量为 di
}

//---------------------------------------------------------------
//  QuadraticProbe — 二次探测再散列
//  di = 1², -1², 2², -2², 3², ...
//  公式：Hi = (H(key) + di²) % size 或 (H(key) - di²) % size
//  好处：避免一次聚集，但可能出现探测不到空位的情况
//---------------------------------------------------------------
int QuadraticProbe(int di, int size)
{
    // 奇数次探测用正平方，偶数次用负平方
    int half = (di + 1) / 2;         // 第几个平方数
    if (di % 2 == 1)
    {
        // 第 1, 3, 5, ... 次探测：+1², +2², +3²
        return half * half;
    }
    else
    {
        // 第 2, 4, 6, ... 次探测：-1², -2², -3²
        return -(half * half);
    }
}

//---------------------------------------------------------------
//  InitHashTable — 初始化散列表
//---------------------------------------------------------------
Status InitHashTable(HashTable* HT, int size)
{
    if (size <= 0) return ERROR;

    HT->elem = (HashElem*)malloc(size * sizeof(HashElem));
    if (!HT->elem) exit(OVERFLOW);

    HT->size = size;
    HT->count = 0;

    // 全部标记为空
    for (int i = 0; i < size; i++)
    {
        HT->elem[i].flag = EMPTY;
        HT->elem[i].key = 0;
    }

    return OK;
}

//---------------------------------------------------------------
//  SearchHash — 在散列表中查找关键字 key
//
//  参数：
//     HT    — 散列表
//     key   — 要查找的关键字
//     addr  — 输出参数：找到的位置 / 应插入的位置
//  返回：
//     OK    — 找到，addr 存位置
//     ERROR — 没找到，addr 存"应该插入的位置"
//
//  查找过程（线性探测）：
//     1. 计算 H(key)
//     2. 如果该位置为空 → 没找到
//     3. 如果该位置被占用且 key 相等 → 找到了
//     4. 否则按探测序列继续找下一个位置
//---------------------------------------------------------------
int SearchHash(HashTable* HT, int key, int* addr)
{
    int H0 = Hash(key, HT->size);  // 初始哈希地址
    int di = 0;                     // 探测次数

    // 按探测序列逐个检查
    while (HT->elem[(H0 + di) % HT->size].flag != EMPTY
           && HT->elem[(H0 + di) % HT->size].flag != DELETED)
    {
        // 如果当前元素是目标 key
        if (HT->elem[(H0 + di) % HT->size].key == key)
        {
            *addr = (H0 + di) % HT->size;
            return OK;  // 找到了
        }
        di++;  // 继续探测下一个位置
    }

    // 找到空位但没找到 key → 查找失败
    *addr = (H0 + di) % HT->size;
    return ERROR;
}

//---------------------------------------------------------------
//  InsertLinear — 线性探测法插入
//
//  步骤：
//     1. 查找 key 是否已存在
//     2. 如果存在 → 不插入
//     3. 如果不存在 → 插到查找返回的 addr 位置
//     4. 装填因子超过阈值时进行再散列
//---------------------------------------------------------------
Status InsertLinear(HashTable* HT, int key)
{
    if (!HT) return ERROR;

    // 先检查 key 是否已存在
    int addr;
    if (SearchHash(HT, key, &addr) == OK)
    {
        // printf("  关键字 %d 已存在\n", key);
        return ERROR;
    }

    // 插入到 addr 位置
    HT->elem[addr].key = key;
    HT->elem[addr].flag = OCCUPIED;
    HT->count++;

    // 检查是否需要再散列（装填因子 > 0.75 时扩容）
    float loadFactor = (float)HT->count / HT->size;
    if (loadFactor > 0.75f)
    {
        // printf("  装填因子 %.2f > 0.75，触发再散列\n", loadFactor);
        Rehash(HT, HT->size * 2 + 1);  // 扩展为约2倍的质数
    }

    return OK;
}

//---------------------------------------------------------------
//  InsertQuadratic — 二次探测法插入
//
//  用二次探测序列寻找空位
//  比线性探测复杂一些，但避免了"一次聚集"
//---------------------------------------------------------------
Status InsertQuadratic(HashTable* HT, int key)
{
    if (!HT) return ERROR;

    int H0 = Hash(key, HT->size);

    // 二次探测最多探测 size 次
    for (int di = 1; di <= HT->size; di++)
    {
        int offset = QuadraticProbe(di, HT->size);
        int addr = (H0 + offset) % HT->size;
        if (addr < 0) addr += HT->size;  // 处理负数

        // 当前位置为空或被删除 → 可以插入
        if (HT->elem[addr].flag == EMPTY || HT->elem[addr].flag == DELETED)
        {
            HT->elem[addr].key = key;
            HT->elem[addr].flag = OCCUPIED;
            HT->count++;
            return OK;
        }

        // 如果 key 已存在 → 不插入
        if (HT->elem[addr].key == key)
        {
            return ERROR;
        }
    }

    // 探测了 size 次都没找到空位
    // printf("  二次探测无法插入 %d\n", key);
    return ERROR;
}

//---------------------------------------------------------------
//  DeleteHash — 删除（惰性删除）
//
//  为什么不用真删除？
//     真删除会把位置清空，导致后续查找时
//     沿着探测序列找到空位就停，误以为"不存在"
//     所以用 DELETED 标记，表示"之前有元素但被删了"
//---------------------------------------------------------------
Status DeleteHash(HashTable* HT, int key)
{
    int addr;
    if (SearchHash(HT, key, &addr) == ERROR)
    {
        return ERROR;  // 没找到
    }

    HT->elem[addr].flag = DELETED;  // 惰性删除
    HT->count--;
    return OK;
}

//---------------------------------------------------------------
//  PrintHashTable — 打印散列表
//---------------------------------------------------------------
void PrintHashTable(HashTable* HT)
{
    if (!HT || !HT->elem) return;

    printf("   下标: ");
    for (int i = 0; i < HT->size; i++)
    {
        printf("%3d ", i);
    }
    printf("\n");

    printf("   关键字: ");
    for (int i = 0; i < HT->size; i++)
    {
        if (HT->elem[i].flag == OCCUPIED)
        {
            printf("%3d ", HT->elem[i].key);
        }
        else if (HT->elem[i].flag == DELETED)
        {
            printf("  X ");  // 已删除
        }
        else
        {
            printf("    ");  // 空
        }
    }
    printf("\n");

    printf("   状态: ");
    for (int i = 0; i < HT->size; i++)
    {
        if (HT->elem[i].flag == OCCUPIED) printf("  O ");
        else if (HT->elem[i].flag == DELETED) printf("  D ");
        else printf("  E ");
    }
    printf("   (O=占用 D=删除 E=空)\n");

    printf("   表长=%d  元素数=%d  装填因子=%.2f\n",
           HT->size, HT->count, (float)HT->count / HT->size);
}

//---------------------------------------------------------------
//  CalculateASL — 计算平均查找长度（ASLsucc）
//
//  对表中每个已存在的元素，统计查找它需要的探测次数
//  求和后除以元素总数
//---------------------------------------------------------------
float CalculateASL(HashTable* HT)
{
    if (!HT || HT->count == 0) return 0.0f;

    int totalProbes = 0;
    int foundCount = 0;

    // 对每个已占用的元素，模拟查找过程
    for (int i = 0; i < HT->size; i++)
    {
        if (HT->elem[i].flag != OCCUPIED) continue;

        int key = HT->elem[i].key;
        int H0 = Hash(key, HT->size);
        int probes = 0;

        // 模拟查找
        for (int di = 0; di < HT->size; di++)
        {
            int addr = (H0 + di) % HT->size;
            probes++;
            if (HT->elem[addr].flag == OCCUPIED && HT->elem[addr].key == key)
            {
                break;
            }
        }
        totalProbes += probes;
        foundCount++;
    }

    return (float)totalProbes / foundCount;
}

//---------------------------------------------------------------
//  Rehash — 再散列
//
//  什么时候触发？
//     装填因子 > 0.75 时（经验值）
//
//  怎么做？
//     1. 保存旧表
//     2. 分配新表（更大的空间）
//     3. 把旧表中所有有效元素重新插入新表
//     4. 释放旧表
//
//  为什么叫"再散列"？
//     因为表长变了，哈希地址 H(key) = key % size 也变了
//     所以每个元素都要用新表长重新计算位置
//---------------------------------------------------------------
Status Rehash(HashTable* HT, int newSize)
{
    if (!HT || newSize <= HT->count) return ERROR;

    // 保存旧数据
    HashElem* oldElem = HT->elem;
    int oldSize = HT->size;

    // 分配新表
    HT->elem = (HashElem*)malloc(newSize * sizeof(HashElem));
    if (!HT->elem) exit(OVERFLOW);
    HT->size = newSize;
    HT->count = 0;

    for (int i = 0; i < newSize; i++)
    {
        HT->elem[i].flag = EMPTY;
    }

    // 重新插入所有有效元素
    for (int i = 0; i < oldSize; i++)
    {
        if (oldElem[i].flag == OCCUPIED)
        {
            InsertLinear(HT, oldElem[i].key);
        }
    }

    // 释放旧表
    free(oldElem);

    // printf("  再散列完成：%d → %d\n", oldSize, newSize);
    return OK;
}

//---------------------------------------------------------------
//  DestroyHashTable — 销毁散列表
//---------------------------------------------------------------
void DestroyHashTable(HashTable* HT)
{
    if (HT && HT->elem)
    {
        free(HT->elem);
        HT->elem = NULL;
        HT->size = 0;
        HT->count = 0;
    }
}

//===============================================================
//  链地址法 — 拉链法
//===============================================================

//---------------------------------------------------------------
//  InitChainHash — 初始化链地址法散列表
//---------------------------------------------------------------
Status InitChainHash(ChainHash* CHT, int size)
{
    if (size <= 0) return ERROR;

    CHT->buckets = (HashNodePtr*)malloc(size * sizeof(HashNodePtr));
    if (!CHT->buckets) exit(OVERFLOW);

    CHT->size = size;
    CHT->count = 0;

    // 每个桶初始化为空链表
    for (int i = 0; i < size; i++)
    {
        CHT->buckets[i] = NULL;
    }

    return OK;
}

//---------------------------------------------------------------
//  InsertChain — 链地址法插入
//
//  步骤：
//     1. 计算 H(key) 找到桶号
//     2. 在该桶的链表中查找 key 是否已存在
//     3. 不存在则头插到链表中
//---------------------------------------------------------------
Status InsertChain(ChainHash* CHT, int key)
{
    if (!CHT) return ERROR;

    int addr = Hash(key, CHT->size);  // 桶号

    // 检查该桶中是否已有相同的 key
    HashNode* p = CHT->buckets[addr];
    while (p)
    {
        if (p->key == key) return ERROR;  // 已存在
        p = p->next;
    }

    // 头插法：新节点插入链表头部
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    if (!newNode) exit(OVERFLOW);
    newNode->key = key;
    newNode->next = CHT->buckets[addr];
    CHT->buckets[addr] = newNode;

    CHT->count++;
    return OK;
}

//---------------------------------------------------------------
//  SearchChain — 链地址法查找
//
//  在 key 对应的桶的链表中顺序查找
//  返回指向节点的指针，没找到返回 NULL
//---------------------------------------------------------------
HashNode* SearchChain(ChainHash* CHT, int key)
{
    if (!CHT) return NULL;

    int addr = Hash(key, CHT->size);
    HashNode* p = CHT->buckets[addr];

    while (p)
    {
        if (p->key == key) return p;
        p = p->next;
    }

    return NULL;
}

//---------------------------------------------------------------
//  DeleteChain — 链地址法删除
//---------------------------------------------------------------
Status DeleteChain(ChainHash* CHT, int key)
{
    if (!CHT) return ERROR;

    int addr = Hash(key, CHT->size);
    HashNode* p = CHT->buckets[addr];
    HashNode* q = NULL;  // 前驱节点

    while (p)
    {
        if (p->key == key)
        {
            if (q == NULL)
            {
                // 删除的是头节点
                CHT->buckets[addr] = p->next;
            }
            else
            {
                q->next = p->next;
            }
            free(p);
            CHT->count--;
            return OK;
        }
        q = p;
        p = p->next;
    }

    return ERROR;  // 没找到
}

//---------------------------------------------------------------
//  PrintChainHash — 打印链地址法散列表
//---------------------------------------------------------------
void PrintChainHash(ChainHash* CHT)
{
    if (!CHT || !CHT->buckets) return;

    printf("   链地址法散列表：\n");
    for (int i = 0; i < CHT->size; i++)
    {
        printf("   桶[%d]: ", i);
        HashNode* p = CHT->buckets[i];
        if (p == NULL)
        {
            printf("NULL");
        }
        else
        {
            while (p)
            {
                printf("%d", p->key);
                if (p->next) printf(" → ");
                p = p->next;
            }
        }
        printf("\n");
    }
    printf("   桶数=%d  元素总数=%d  平均链长=%.2f\n",
           CHT->size, CHT->count, (float)CHT->count / CHT->size);
}

//---------------------------------------------------------------
//  CalculateChainASL — 计算链地址法的 ASLsucc
//
//  ASLsucc = (每个桶的链长 × 该链中查找每个元素需比较次数) / 元素总数
//  每个链中查找成功比较次数 = (1 + 链长) / 2 × 链长... 
//  实际上就是：每个元素的查找比较次数 = 它在链中的位置
//---------------------------------------------------------------
float CalculateChainASL(ChainHash* CHT)
{
    if (!CHT || CHT->count == 0) return 0.0f;

    int totalProbes = 0;

    for (int i = 0; i < CHT->size; i++)
    {
        int pos = 1;
        HashNode* p = CHT->buckets[i];

        while (p)
        {
            totalProbes += pos;  // 第 pos 个元素需要比较 pos 次
            pos++;
            p = p->next;
        }
    }

    return (float)totalProbes / CHT->count;
}

//---------------------------------------------------------------
//  DestroyChainHash — 销毁链地址法散列表
//---------------------------------------------------------------
void DestroyChainHash(ChainHash* CHT)
{
    if (!CHT || !CHT->buckets) return;

    for (int i = 0; i < CHT->size; i++)
    {
        HashNode* p = CHT->buckets[i];
        while (p)
        {
            HashNode* q = p;
            p = p->next;
            free(q);
        }
    }

    free(CHT->buckets);
    CHT->buckets = NULL;
    CHT->size = 0;
    CHT->count = 0;
}

//===============================================================
//  main — 测试散列表
//===============================================================
int main()
{
    printf("======= 散列表测试 =======\n\n");

    // 教材例 8.3 / 8.4 的测试数据
    // 关键字集合：{19, 14, 23, 1, 68, 20, 84, 27, 55, 11, 10, 79}
    int keys[] = {19, 14, 23, 1, 68, 20, 84, 27, 55, 11, 10, 79};
    int n = 12;

    //----------------------------------------
    //  第一部分：线性探测法
    //----------------------------------------
    printf("========== 一、线性探测法 ==========\n\n");

    HashTable HT;
    InitHashTable(&HT, 16);  // 表长 16

    printf("1. 线性探测法插入关键字：\n   ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", keys[i]);
        InsertLinear(&HT, keys[i]);
    }
    printf("\n\n");

    printf("2. 线性探测法散列表：\n");
    PrintHashTable(&HT);

    printf("\n3. 计算 ASLsucc（成功平均查找长度）：\n");
    float asl = CalculateASL(&HT);
    printf("   ASLsucc = %.2f\n", asl);
    printf("   （即平均查找每个元素需要探测 %.2f 次）\n", asl);

    // 测试查找
    printf("\n4. 查找测试：\n");
    int addr;
    if (SearchHash(&HT, 68, &addr) == OK)
        printf("   找到关键字 68，位置在 %d\n", addr);
    if (SearchHash(&HT, 99, &addr) == ERROR)
        printf("   关键字 99 不存在，应插入位置 %d\n", addr);

    // 测试删除
    printf("\n5. 删除测试（删除 68）：\n");
    DeleteHash(&HT, 68);
    PrintHashTable(&HT);

    // 恢复删除
    InsertLinear(&HT, 68);

    //----------------------------------------
    //  第二部分：二次探测法
    //----------------------------------------
    printf("\n========== 二、二次探测法 ==========\n\n");

    HashTable HT2;
    InitHashTable(&HT2, 16);

    printf("1. 二次探测法插入关键字：\n   ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", keys[i]);
        InsertQuadratic(&HT2, keys[i]);
    }
    printf("\n\n");

    printf("2. 二次探测法散列表：\n");
    PrintHashTable(&HT2);

    //----------------------------------------
    //  第三部分：链地址法
    //----------------------------------------
    printf("\n========== 三、链地址法（拉链法） ==========\n\n");

    ChainHash CHT;
    InitChainHash(&CHT, 13);  // 教材例 8.4，表长 13（质数）

    printf("1. 链地址法插入关键字：\n   ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", keys[i]);
        InsertChain(&CHT, keys[i]);
    }
    printf("\n\n");

    printf("2. 链地址法散列表：\n");
    PrintChainHash(&CHT);

    printf("\n3. 计算 ASLsucc：\n");
    float casl = CalculateChainASL(&CHT);
    printf("   ASLsucc = %.2f\n", casl);

    // 测试查找
    printf("\n4. 查找测试：\n");
    HashNode* found = SearchChain(&CHT, 20);
    if (found)
        printf("   找到关键字 20\n");
    found = SearchChain(&CHT, 99);
    if (!found)
        printf("   关键字 99 不存在\n");

    // 测试删除
    printf("\n5. 删除测试（删除 20）：\n");
    if (DeleteChain(&CHT, 20) == OK)
    {
        printf("   删除成功\n");
        PrintChainHash(&CHT);
    }

    //----------------------------------------
    //  第四部分：再散列演示
    //----------------------------------------
    printf("\n========== 四、再散列演示 ==========\n\n");

    // 故意用小表，让装填因子迅速超过阈值触发再散列
    HashTable HT3;
    InitHashTable(&HT3, 7);  // 小表，很快触发再散列

    printf("1. 往小表（size=%d）中插入元素，触发再散列：\n", 7);
    printf("   插入元素：");
    int smallKeys[] = {12, 25, 38, 19, 8, 5};
    for (int i = 0; i < 6; i++)
    {
        printf("%d ", smallKeys[i]);
        InsertLinear(&HT3, smallKeys[i]);
    }
    printf("\n\n");
    printf("   最终散列表：\n");
    PrintHashTable(&HT3);

    //----------------------------------------
    //  第五部分：三种方法对比
    //----------------------------------------
    printf("\n========== 五、三种方法 ASL 对比 ==========\n\n");
    printf("   方法          | 表长 | ASLsucc\n");
    printf("   -------------|------|--------\n");
    printf("   线性探测法    |  %2d  |  %.2f\n", HT.size, CalculateASL(&HT));
    printf("   二次探测法    |  %2d  |  %.2f\n", HT2.size, CalculateASL(&HT2));
    printf("   链地址法      |  %2d  |  %.2f\n", CHT.size, CalculateChainASL(&CHT));

    // 释放资源
    printf("\n6. 释放资源：\n");
    DestroyHashTable(&HT);
    DestroyHashTable(&HT2);
    DestroyChainHash(&CHT);
    DestroyHashTable(&HT3);
    printf("   所有资源已释放\n");

    printf("\n======= 测试完成 =======\n");

    return 0;
}
