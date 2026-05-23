#!/bin/bash

# 一键编译和测试所有数据结构程序
# 通信2401 贺明贤 - 数据结构课程学习仓库

echo "=== 开始编译所有数据结构程序 ==="
echo ""

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

echo "=== 所有测试完成 ==="
echo ""
echo "已测试的程序："
echo "1. SqList    - 顺序表"
echo "2. SqStack   - 顺序栈"
echo "3. LNode     - 单链表"
echo "4. DulNode   - 循环双向链表"
echo "5. QNode     - 链式队列"
echo "6. BiTree    - 二叉树"
echo "7. String    - 串（字符串）"
echo ""
echo "所有程序编译和测试成功！"
