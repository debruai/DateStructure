#!/bin/bash

# 一键编译和测试所有数据结构程序
# 通信2401 贺明贤 - 数据结构课程学习仓库
# 运行方式：在 DateStructure 目录下执行 ./test_all.sh

echo "=== 开始编译所有数据结构程序 ==="
echo ""

# 切换到脚本所在目录
cd "$(dirname "$0")"

# 清理之前的编译结果
make clean

# 编译所有程序
echo "1. 编译所有程序..."
make
if [ $? -ne 0 ]; then
    echo "编译失败！"
    exit 1
fi
echo "编译完成！"
echo ""

# 测试每个程序
echo "2. 测试各个程序："
echo ""

# 测试顺序表
echo "--- 测试顺序表 (SqList) ---"
./SqList
echo ""

# 测试顺序栈
echo "--- 测试顺序栈 (SqStack) ---"
./SqStack
echo ""

# 测试单链表
echo "--- 测试单链表 (LNode) ---"
./LNode
echo ""

# 测试循环双向链表
echo "--- 测试循环双向链表 (DulNode) ---"
./DulNode
echo ""

# 测试链式队列
echo "--- 测试链式队列 (QNode) ---"
./QNode
echo ""

# 测试二叉树
echo "--- 测试二叉树 (BiTree) ---"
./BiTree
echo ""

# 测试串（字符串）
echo "--- 测试串 (String) ---"
./String
echo ""

# 测试哈夫曼树
echo "--- 测试哈夫曼树 (HFTree) ---"
./HFTree
echo ""

# 测试线索二叉树
echo "--- 测试线索二叉树 (ThreadBiTree) ---"
./ThreadBiTree
echo ""

# 测试树与森林
echo "--- 测试树与森林 (TreeForest) ---"
./TreeForest
echo ""

# 测试散列表
echo "--- 测试散列表 (HashTable) ---"
./HashTable
echo ""

# 测试排序
echo "--- 测试内部排序 (Sort) ---"
./Sort
echo ""

# 测试查找
echo "--- 测试查找 (Search) ---"
./Search
echo ""

# 测试图
echo "--- 测试图 (Graph) ---"
./Graph
echo ""

echo "=== 所有测试完成 ==="
echo ""
echo "已测试的程序："
echo " 1. SqList       - 顺序表"
echo " 2. SqStack      - 顺序栈"
echo " 3. LNode        - 单链表"
echo " 4. DulNode      - 循环双向链表"
echo " 5. QNode        - 链式队列"
echo " 6. BiTree       - 二叉树"
echo " 7. String       - 串（字符串/KMP）"
echo " 8. HFTree       - 哈夫曼树"
echo " 9. ThreadBiTree - 线索二叉树"
echo "10. TreeForest   - 树与森林"
echo "11. HashTable    - 散列表"
echo "12. Sort         - 内部排序（9种）"
echo "13. Search       - 查找（折半/BST/AVL）"
echo "14. Graph        - 图（遍历/MST/最短路径/拓扑/关键路径）"
echo ""
echo "所有程序编译和测试成功！"
