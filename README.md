# 数据结构算法实现

> 通信2401 贺明贤 — 数据结构课程学习仓库

本项目用于系统学习数据结构，逐步实现教材中的经典数据结构和算法。每个 `.c` 文件独立可编译运行，配有测试用例。

---

## 项目结构

```
DateStructure/
├── BiTree.c          # 二叉树（链式存储）：遍历、深度、节点计数、查找、复制、替换
├── SqList.c          # 顺序表（动态分配）：增删改查、归并
├── SqList.h          # 顺序表头文件
├── SqStack.c         # 顺序栈：Push/Pop、进制转换、括号匹配
├── LNode.c           # 单链表（带头结点）：增删改查、反转、归并
├── DulNode.c         # 循环双向链表：插入、删除、双向遍历
├── QNode.c           # 链式队列：入队、出队、清空
├── Makefile          # GNU Make 构建脚本
├── test_all.sh       # 一键编译 + 测试所有程序
└── .gitignore        # 忽略编译产物
```

---

## 已实现的数据结构

| 文件 | 结构 | 核心操作 |
|------|------|----------|
| `BiTree.c` | 二叉树（链式） | 先序/中序/后序遍历、深度、节点/叶子计数、查找、复制、子树替换 |
| `SqList.c` | 顺序表 | 初始化、插入、删除、查找、修改、前驱/后继、归并 |
| `SqStack.c` | 顺序栈 | Push/Pop/GetTop、扩容、进制转换、括号匹配 |
| `LNode.c` | 单链表 | 插入、删除、遍历、查找、前驱/后继、反转、归并 |
| `DulNode.c` | 循环双向链表 | 插入、删除、正反向遍历、前后驱链接 |
| `QNode.c` | 链式队列 | 入队、出队、队头查询、清空、释放 |

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
./BiTree      # 二叉树
./SqList      # 顺序表
./SqStack     # 顺序栈
./LNode       # 单链表
./DulNode     # 循环双向链表
./QNode       # 链式队列
```

### 一键测试

```bash
./test_all.sh
```

### 手动编译单个文件

```bash
gcc -Wall -g BiTree.c -o BiTree && ./BiTree
```

---

## 开发流程

```bash
# 1. 拉取最新代码
git pull

# 2. 编写/修改代码（例如新增 Graph.c）
vim Graph.c

# 3. 编译测试
make && ./Graph

# 4. 提交
git add Graph.c Makefile
git commit -m "新增：图的邻接表实现"

# 5. 推送
git push
```

---

## 待实现

- [ ] 串的模式匹配（KMP）
- [ ] 广义表
- [ ] 树的应用（哈夫曼树、并查集）
- [ ] 图的存储和遍历（DFS/BFS）
- [ ] 最小生成树（Prim / Kruskal）
- [ ] 最短路径（Dijkstra / Floyd）
- [ ] 排序算法（插入、希尔、快速、堆、归并、基数）
- [ ] 查找算法（二分、B树、哈希表）

---

## 环境

- 编译器：GCC 14+
- 系统：Manjaro Linux / 任意类 Unix
- 标准：C99+

---

*最后更新：2025-05-22*
