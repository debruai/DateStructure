#!/bin/bash

echo "=== 数据结构项目测试脚本 ==="
echo "当前目录: $(pwd)"
echo "GCC版本: $(gcc --version | head -1)"
echo ""

# 清理并重新编译所有程序
echo "1. 清理并重新编译所有程序..."
make clean
make

echo ""
echo "2. 测试所有程序:"
echo ""

# 测试每个程序
programs=("BiTree" "SqList" "SqStack" "DulNode" "LNode" "QNode")

for prog in "${programs[@]}"; do
    if [ -f "$prog" ]; then
        echo "=== 测试 $prog ==="
        timeout 3 ./"$prog" 2>&1 | head -10
        echo "退出代码: $?"
        echo ""
    else
        echo "错误: $prog 不存在"
    fi
done

echo "=== 测试完成 ==="
echo ""
echo "生成的可执行文件:"
ls -la BiTree SqList SqStack DulNode LNode QNode 2>/dev/null || echo "部分文件不存在"

echo ""
echo "项目结构:"
echo "- BiTree: 二叉树实现"
echo "- SqList: 顺序表实现"
echo "- SqStack: 顺序栈实现"
echo "- DulNode: 循环双向链表实现"
echo "- LNode: 单链表实现"
echo "- QNode: 链式队列实现"
