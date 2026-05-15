# C语言项目编译和运行指南

## 项目结构
这是一个C语言数据结构项目，包含多个独立的数据结构实现文件：
- `BiTree.c` - 二叉树实现（包含main函数）
- `SqList.c` - 顺序表实现（包含main函数）
- `SqStack.c` - 顺序栈实现
- `DulNode.c` - 双向链表实现
- `LNode.c` - 单链表实现
- `QNode.c` - 队列节点实现
- `SqList.h` - 顺序表头文件

## 编译方法

### 方法1：单独编译每个文件
每个.c文件都可以独立编译和运行：

```bash
# 编译二叉树程序
gcc BiTree.c -o BiTree

# 编译顺序表程序
gcc SqList.c -o SqList

# 编译其他程序
gcc SqStack.c -o SqStack
gcc DulNode.c -o DulNode
gcc LNode.c -o LNode
gcc QNode.c -o QNode
```

### 方法2：使用Makefile（推荐）
创建一个Makefile来简化编译过程：

```makefile
CC = gcc
CFLAGS = -Wall -g

TARGETS = BiTree SqList SqStack DulNode LNode QNode

all: $(TARGETS)

BiTree: BiTree.c
	$(CC) $(CFLAGS) -o $@ $<

SqList: SqList.c
	$(CC) $(CFLAGS) -o $@ $<

SqStack: SqStack.c
	$(CC) $(CFLAGS) -o $@ $<

DulNode: DulNode.c
	$(CC) $(CFLAGS) -o $@ $<

LNode: LNode.c
	$(CC) $(CFLAGS) -o $@ $<

QNode: QNode.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
```

保存为`Makefile`，然后运行：
```bash
# 编译所有程序
make

# 清理生成的可执行文件
make clean
```

### 方法3：使用Visual Studio项目文件
如果你在Windows上并且安装了Visual Studio，可以使用：
- `DateStructure.slnx` - Visual Studio解决方案文件
- `DateStructure.vcxproj` - Visual Studio项目文件

## 运行程序

编译成功后，运行可执行文件：

```bash
# 运行二叉树程序
./BiTree

# 运行顺序表程序
./SqList

# 运行其他程序
./SqStack
./DulNode
./LNode
./QNode
```

## 调试程序

### 使用gdb调试
```bash
# 编译时添加调试信息
gcc -g BiTree.c -o BiTree_debug

# 启动gdb调试
gdb ./BiTree_debug

# 在gdb中常用命令：
#   break main      # 在main函数设置断点
#   run             # 运行程序
#   next            # 执行下一行
#   print variable  # 打印变量值
#   quit            # 退出gdb
```

### 使用valgrind检查内存泄漏
```bash
valgrind --leak-check=full ./BiTree
```

## 常见问题解决

### 1. 编译错误：找不到头文件
确保头文件在正确的位置，或者使用`-I`选项指定头文件路径：
```bash
gcc -I. BiTree.c -o BiTree
```

### 2. 链接错误：未定义的引用
检查是否包含了所有必要的源文件：
```bash
gcc BiTree.c SqList.c -o program
```

### 3. 运行时错误：段错误
使用gdb调试或添加打印语句定位问题。

### 4. 权限问题
确保可执行文件有执行权限：
```bash
chmod +x BiTree
```

## 快速开始

1. 打开终端
2. 进入项目目录：
   ```bash
   cd /home/hemingxian/Desktop/DateStructure
   ```
3. 编译程序：
   ```bash
   gcc BiTree.c -o BiTree
   ```
4. 运行程序：
   ```bash
   ./BiTree
   ```

## 注意事项
- 每个.c文件都是独立的，可以单独编译运行
- 如果修改了代码，需要重新编译
- 建议使用`-Wall`选项开启所有警告
- 调试时使用`-g`选项添加调试信息
