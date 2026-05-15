#!/bin/bash

echo "=== Git使用示例演示 ==="
echo "本脚本演示Git的基本工作流程"
echo ""

# 1. 检查Git状态
echo "1. 检查当前Git状态:"
git status
echo ""

# 2. 查看.gitignore文件
echo "2. 查看.gitignore文件内容:"
cat .gitignore
echo ""

# 3. 添加文件到暂存区
echo "3. 添加文件到暂存区:"
echo "  添加.gitignore文件..."
git add .gitignore
echo "  添加SqList.h文件（已修改）..."
git add SqList.h
echo "  添加其他源文件..."
git add *.c *.h Makefile README.md COMPILE_GUIDE.md GCC_INTRODUCTION.md GIT_CONFIG_GUIDE.md
echo ""

# 4. 再次检查状态
echo "4. 添加文件后的状态:"
git status
echo ""

# 5. 提交更改
echo "5. 提交更改:"
git commit -m "修复SqList.h编码问题，添加.gitignore和Git配置指南"
echo ""

# 6. 查看提交历史
echo "6. 查看提交历史:"
git log --oneline -5
echo ""

# 7. 推送到远程仓库（可选）
echo "7. 推送到远程仓库（可选）:"
echo "  当前远程仓库:"
git remote -v
echo ""
echo "  注意: 推送需要权限，您可以选择是否推送"
echo "  如果要推送，请运行: git push"
echo ""

# 8. 创建和切换分支示例
echo "8. 分支操作示例:"
echo "  创建新分支: git checkout -b feature/improvement"
echo "  切换回主分支: git checkout main"
echo "  查看所有分支: git branch -a"
echo ""

# 9. 常用命令总结
echo "9. Git常用命令总结:"
echo "  git init                     # 初始化仓库"
echo "  git clone <url>              # 克隆远程仓库"
echo "  git add <file>               # 添加文件到暂存区"
echo "  git commit -m \"message\"     # 提交更改"
echo "  git status                   # 查看状态"
echo "  git log --oneline            # 查看简洁提交历史"
echo "  git push                     # 推送到远程"
echo "  git pull                     # 拉取更新"
echo "  git branch                   # 查看分支"
echo "  git checkout <branch>        # 切换分支"
echo "  git merge <branch>           # 合并分支"
echo "  git diff                     # 查看差异"
echo ""

# 10. 实际工作流程
echo "10. 实际开发工作流程示例:"
cat << 'EOF'
# 1. 拉取最新代码
git pull

# 2. 创建功能分支
git checkout -b feature/new-feature

# 3. 开发代码...
# 4. 添加修改
git add .

# 5. 提交更改
git commit -m "实现新功能"

# 6. 推送到远程
git push -u origin feature/new-feature

# 7. 创建Pull Request（在GitHub/GitLab界面上操作）

# 8. 代码审查通过后，合并到主分支
git checkout main
git pull
git merge feature/new-feature
git push

# 9. 删除功能分支
git branch -d feature/new-feature
git push origin --delete feature/new-feature
EOF

echo ""
echo "=== 演示完成 ==="
echo "提示: 要实际运行这些命令，请取消注释相应的行"
echo "      或者直接运行脚本中的命令"
