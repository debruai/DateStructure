#!/bin/bash

echo "=== Git配置脚本 ==="
echo "本脚本将帮助您配置Git以便提交和推送代码"
echo ""

# 检查Git是否安装
if ! command -v git &> /dev/null; then
    echo "错误: Git未安装"
    echo "请先安装Git:"
    echo "  Ubuntu/Debian: sudo apt install git"
    echo "  CentOS/RHEL: sudo yum install git"
    echo "  macOS: brew install git"
    exit 1
fi

echo "1. 当前Git版本: $(git --version)"
echo ""

# 检查当前配置
echo "2. 检查当前Git配置:"
echo "   用户名: $(git config user.name 2>/dev/null || echo '未配置')"
echo "   邮箱: $(git config user.email 2>/dev/null || echo '未配置')"
echo ""

# 配置用户名和邮箱
echo "3. 配置Git用户信息"
echo "   请输入您的姓名（用于提交记录）:"
read -r user_name
echo "   请输入您的邮箱（用于提交记录）:"
read -r user_email

if [ -z "$user_name" ] || [ -z "$user_email" ]; then
    echo "错误: 姓名和邮箱不能为空"
    exit 1
fi

git config --global user.name "$user_name"
git config --global user.email "$user_email"

echo "   配置完成！"
echo ""

# 配置凭据缓存
echo "4. 配置Git凭据缓存（避免每次输入密码）"
echo "   是否配置凭据缓存？(y/n):"
read -r configure_cache

if [[ "$configure_cache" == "y" || "$configure_cache" == "Y" ]]; then
    echo "   选择缓存时间:"
    echo "   1. 15分钟"
    echo "   2. 1小时"
    echo "   3. 8小时"
    echo "   4. 1天"
    read -r cache_choice
    
    case $cache_choice in
        1)
            timeout=900
            ;;
        2)
            timeout=3600
            ;;
        3)
            timeout=28800
            ;;
        4)
            timeout=86400
            ;;
        *)
            timeout=3600
            echo "   使用默认值: 1小时"
            ;;
    esac
    
    git config --global credential.helper "cache --timeout=$timeout"
    echo "   凭据缓存已配置: $timeout 秒"
else
    echo "   跳过凭据缓存配置"
fi
echo ""

# 配置别名
echo "5. 配置Git别名（简化常用命令）"
echo "   是否配置常用别名？(y/n):"
read -r configure_aliases

if [[ "$configure_aliases" == "y" || "$configure_aliases" == "Y" ]]; then
    git config --global alias.co checkout
    git config --global alias.br branch
    git config --global alias.ci commit
    git config --global alias.st status
    git config --global alias.lg "log --oneline --graph --all"
    git config --global alias.last "log -1 HEAD"
    git config --global alias.unstage "reset HEAD --"
    
    echo "   别名已配置:"
    echo "   - git co: git checkout"
    echo "   - git br: git branch"
    echo "   - git ci: git commit"
    echo "   - git st: git status"
    echo "   - git lg: git log --oneline --graph --all"
    echo "   - git last: git log -1 HEAD"
    echo "   - git unstage: git reset HEAD --"
else
    echo "   跳过别名配置"
fi
echo ""

# 显示最终配置
echo "6. 最终配置检查:"
echo "=== Git全局配置 ==="
git config --global --list
echo ""

echo "=== 当前仓库配置 ==="
git config --local --list 2>/dev/null || echo "无本地配置"
echo ""

# 创建.gitignore文件建议
echo "7. 创建.gitignore文件建议"
cat > .gitignore.example << 'EOF'
# 编译生成文件
*.o
*.exe
*.out
*.app
*.so
*.dll

# 可执行文件（根据项目需要决定是否忽略）
# BiTree
# SqList
# SqStack
# DulNode
# LNode
# QNode

# 依赖目录
node_modules/
vendor/

# 环境配置文件
.env
.env.local
.env.*.local

# 编辑器文件
.vscode/
.idea/
*.swp
*.swo
*~

# 系统文件
.DS_Store
Thumbs.db

# 日志文件
*.log
logs/

# 临时文件
*.tmp
temp/
tmp/
EOF

echo "   已创建.gitignore.example文件，请根据项目需要修改后重命名为.gitignore"
echo ""

echo "=== 配置完成 ==="
echo ""
echo "下一步建议:"
echo "1. 查看.gitignore.example文件，创建适合项目的.gitignore"
echo "2. 使用 'git status' 查看当前状态"
echo "3. 使用 'git add .' 添加文件"
echo "4. 使用 'git commit -m \"提交说明\"' 提交更改"
echo "5. 使用 'git push' 推送到远程仓库"
echo ""
echo "常用命令速查:"
echo "  git status          # 查看状态"
echo "  git add <文件>      # 添加文件"
echo "  git commit -m \"msg\" # 提交更改"
echo "  git push           # 推送到远程"
echo "  git pull           # 拉取更新"
echo "  git log --oneline  # 查看提交历史"
