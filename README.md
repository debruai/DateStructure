# 数据结构算法实现

> 通信2401 贺明贤 — 数据结构课程学习仓库

本项目用于系统学习数据结构，逐步实现教材中的经典数据结构和算法。每个 `.c` 文件独立可编译运行，配有测试用例。

---

## 项目结构

```
DateStructure/
├── BiTree.c            # 二叉树（链式存储）：遍历、深度、节点计数
├── BiTree_Analysis.md  # 二叉树算法分析笔记
├── SqList.c/h          # 顺序表（动态分配）：增删改查、归并
├── SqStack.c           # 顺序栈：Push/Pop、进制转换、括号匹配
├── LNode.c             # 单链表（带头结点）：增删改查、反转、归并
├── DulNode.c           # 循环双向链表：插入、删除、双向遍历
├── QNode.c             # 链式队列：入队、出队、清空
├── String.c            # 串（堆分配存储）：BF/KMP模式匹配
├── HFTree.c/h          # 哈夫曼树：建树/WPL/编码/解码
├── ThreadBiTree.c/h    # 线索二叉树：中序线索化/遍历
├── TreeForest.c/h      # 树与森林：孩子兄弟/转换/遍历
├── HashTable.c/h       # 散列表：线性探测/二次探测/链地址/再散列
├── Sort.c/h            # 内部排序（9种）：插排/希尔/快排/堆排/归并/基数等
├── Search.c/h          # 查找：顺序/折半/BST/AVL
├── Graph.c/h           # 图：邻接矩阵/邻接表/DFS/BFS/Prim/Kruskal/...
│                       #   Dijkstra/Floyd/拓扑排序/关键路径
├── Makefile            # GNU Make 构建脚本
├── test_all.sh         # 一键编译 + 测试所有程序
├── BiTree_Analysis.md  # 二叉树算法分析笔记
├── Makefile            # GNU Make 构建脚本
└── .gitignore          # 忽略编译产物
```

---

## 已实现的数据结构

| 文件 | 结构 | 核心操作 |
|------|------|----------|
| `BiTree.c` | 二叉树（链式） | 先序/中序/后序遍历、深度、节点/叶子计数、查找、复制、替换 |
| `SqList.c` | 顺序表 | 初始化、插入、删除、查找、修改、前驱/后继、归并 |
| `SqStack.c` | 顺序栈 | Push/Pop/GetTop、扩容、进制转换、括号匹配 |
| `LNode.c` | 单链表 | 插入、删除、遍历、查找、前驱/后继、反转、归并 |
| `DulNode.c` | 循环双向链表 | 插入、删除、正反向遍历、前后驱链接 |
| `QNode.c` | 链式队列 | 入队、出队、队头查询、清空、释放 |
| `String.c` | 串（堆分配） | 赋值、复制、连接、子串、BF/KMP匹配 |
| `HFTree.c` | 哈夫曼树 | 建树、WPL计算、编码表生成、编码/解码 |
| `ThreadBiTree.c` | 线索二叉树 | 中序线索化、带头结点遍历 |
| `TreeForest.c` | 树与森林 | 孩子兄弟表示法、树↔二叉树转换、森林↔二叉树转换、先根/后根遍历 |
| `HashTable.c` | 散列表 | 线性探测/二次探测插入查找、链地址法、再散列、ASL计算 |
| `Sort.c` | 内部排序 | 直接插入/折半插入/希尔/冒泡/快速/选择/堆/归并/基数（含稳定性注释） |
| `Search.c` | 查找 | 顺序查找(哨兵)/折半查找/BST(插入/删除)/AVL(四种旋转) |
| `Graph.c` | 图 | 邻接矩阵+邻接表、DFS/BFS、Prim/Kruskal、Dijkstra/Floyd、拓扑排序、关键路径 |

---

## 快速开始

### 编译

```bash
cd ~/Desktop/DateStructure
make          # 编译全部程序
make clean    # 清理可执行文件
```

### 运行

```bash
./BiTree       # 二叉树
./SqList       # 顺序表
./SqStack      # 顺序栈
./LNode        # 单链表
./DulNode      # 循环双向链表
./QNode        # 链式队列
./String       # 串（字符串）
./HFTree       # 哈夫曼树
./ThreadBiTree # 线索二叉树
./TreeForest   # 树与森林
./HashTable    # 散列表
./Sort         # 内部排序
./Search       # 查找
./Graph        # 图
```

### 一键测试

```bash
./test_all.sh
```

### 手动编译单个文件

```bash
gcc -Wall -g Graph.c -o Graph && ./Graph
```

---

## 开发流程

```bash
# 1. 编译测试
make && ./Graph

# 2. 提交
git add Graph.c Makefile
git commit -m "新增：图的邻接表实现"

# 3. 推送
git push
```

---

## 学习顺序建议

1. **顺序表 SqList** — 线性表基础
2. **单链表 LNode / 双向链表 DulNode** — 链式结构
3. **栈 SqStack / 队列 QNode** — 受限线性表
4. **串 String** — KMP 模式匹配
5. **二叉树 BiTree** — 树结构基础
6. **线索二叉树 ThreadBiTree** — 二叉树进阶
7. **树与森林 TreeForest** — 树/森林↔二叉树转换
8. **哈夫曼树 HFTree** — 最优二叉树 + 编码
9. **图 Graph** — 图的存储和算法
10. **查找 Search** — BST / AVL
11. **散列表 HashTable** — 冲突处理 + ASL
12. **排序 Sort** — 9种算法对比

---

## 环境

- 编译器：GCC 14+
- 系统：Manjaro Linux / 任意类 Unix
- 标准：C99+

---

*最后更新：2026-06-24*
