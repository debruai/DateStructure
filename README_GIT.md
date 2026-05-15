# Git配置与使用文件说明

## 创建的文件列表

### 1. 脚本文件（Shell Scripts）

1. **setup_git.sh** - Git配置脚本
   - 用途：交互式配置Git用户名、邮箱、凭据缓存和别名
   - 使用方法：`./setup_git.sh`
   - 功能：
     - 检查Git安装
     - 配置用户信息
     - 配置凭据缓存（避免重复输入密码）
     - 配置常用别名（简化命令）

2. **git_demo.sh** - Git使用演示脚本
   - 用途：演示Git基本工作流程
   - 使用方法：`./git_demo.sh`
   - 功能：
     - 展示Git状态检查
     - 演示添加文件、提交更改
     - 展示分支操作
     - 提供常用命令总结

3. **test_all.sh** - 项目测试脚本
   - 用途：测试所有数据结构程序
   - 使用方法：`./test_all.sh`
   - 功能：
     - 清理并重新编译所有程序
     - 运行每个程序进行测试
     - 显示测试结果

### 2. 文档文件（Markdown Documents）

1. **GIT_CONFIG_GUIDE.md** - Git配置详细指南
   - 内容：完整的Git配置、使用教程和最佳实践
   - 包含：安装、配置、基本命令、分支管理、远程操作等

2. **GCC_INTRODUCTION.md** - GCC编译器介绍
   - 内容：GCC编译器使用指南
   - 包含：GCC功能、编译过程、常用选项等

3. **COMPILE_GUIDE.md** - 项目编译指南（已存在）
   - 内容：项目编译说明

### 3. 配置文件

1. **.gitignore** - Git忽略文件
   - 用途：指定不需要版本控制的文件
   - 忽略内容：编译生成的可执行文件、中间文件、IDE文件等

2. **Makefile** - 项目构建文件（已存在）
   - 用途：自动化编译项目

### 4. 源代码文件

1. **SqList.h** - 顺序表头文件（已修复编码问题）
2. **SqList.c** - 顺序表实现
3. **BiTree.c** - 二叉树实现
4. **SqStack.c** - 顺序栈实现
5. **DulNode.c** - 循环双向链表实现
6. **LNode.c** - 单链表实现
7. **QNode.c** - 链式队列实现

## 快速开始指南

### 步骤1：配置Git
```bash
./setup_git.sh
```
按照提示输入您的姓名和邮箱，配置Git环境。

### 步骤2：学习Git使用
```bash
./git_demo.sh
```
查看Git工作流程演示，学习基本命令。

### 步骤3：测试项目
```bash
./test_all.sh
```
确保所有数据结构程序都能正常编译和运行。

### 步骤4：提交代码到Git
```bash
# 查看当前状态
git status

# 添加所有文件
git add .

# 提交更改
git commit -m "修复SqList.h编码问题，添加Git配置文档"

# 推送到远程仓库（如果需要）
git push
```

## 文件位置
所有文件都创建在项目根目录：`/home/hemingxian/Desktop/DateStructure/`

## 项目结构
```
DateStructure/
├── 脚本文件/
│   ├── setup_git.sh      # Git配置脚本
│   ├── git_demo.sh       # Git演示脚本
│   └── test_all.sh       # 项目测试脚本
├── 文档文件/
│   ├── GIT_CONFIG_GUIDE.md  # Git配置指南
│   ├── GCC_INTRODUCTION.md  # GCC编译器指南
│   ├── COMPILE_GUIDE.md     # 编译指南
│   └── README.md           # 项目说明
├── 配置文件/
│   ├── .gitignore         # Git忽略文件
│   └── Makefile           # 构建文件
├── 源代码文件/
│   ├── SqList.h          # 顺序表头文件
│   ├── SqList.c          # 顺序表实现
│   ├── BiTree.c          # 二叉树实现
│   ├── SqStack.c         # 顺序栈实现
│   ├── DulNode.c         # 循环双向链表
│   ├── LNode.c           # 单链表
│   └── QNode.c           # 链式队列
└── 可执行文件/
    ├── BiTree            # 二叉树程序
    ├── SqList            # 顺序表程序
    ├── SqStack           # 顺序栈程序
    ├── DulNode           # 循环双向链表程序
    ├── LNode             # 单链表程序
    └── QNode             # 链式队列程序
```

## 注意事项
1. 运行脚本前确保有执行权限：`chmod +x *.sh`
2. Git配置只需要进行一次（全局配置）
3. .gitignore文件可以根据需要修改
4. 可执行文件通常不提交到Git（已在.gitignore中忽略）

## 帮助
如果有任何问题，可以：
1. 查看详细文档：`cat GIT_CONFIG_GUIDE.md`
2. 运行演示脚本学习：`./git_demo.sh`
3. 检查Git状态：`git status`
