# Git配置与使用指南

## 1. 检查Git安装

首先检查Git是否已安装：

```bash
git --version
```

如果未安装，使用以下命令安装：
- Ubuntu/Debian: `sudo apt install git`
- CentOS/RHEL: `sudo yum install git`
- macOS: `brew install git`

## 2. 配置Git全局用户信息

配置你的用户名和邮箱（这些信息会出现在你的提交记录中）：

```bash
git config --global user.name "你的姓名"
git config --global user.email "你的邮箱@example.com"
```

示例：
```bash
git config --global user.name "张三"
git config --global user.email "zhangsan@example.com"
```

## 3. 查看当前配置

查看所有配置：
```bash
git config --list
```

查看特定配置：
```bash
git config user.name
git config user.email
```

## 4. 配置Git凭据缓存（避免每次输入密码）

### 方法1：缓存凭据（推荐）
```bash
# 缓存15分钟（900秒）
git config --global credential.helper cache
git config --global credential.helper 'cache --timeout=900'

# 或者缓存1小时（3600秒）
git config --global credential.helper 'cache --timeout=3600'
```

### 方法2：使用SSH密钥（更安全）
1. 生成SSH密钥：
```bash
ssh-keygen -t rsa -b 4096 -C "你的邮箱@example.com"
```

2. 将公钥添加到GitHub/GitLab：
```bash
cat ~/.ssh/id_rsa.pub
```
复制输出的内容，然后添加到你的GitHub/GitLab账户的SSH密钥设置中。

3. 测试SSH连接：
```bash
ssh -T git@github.com
```

## 5. 初始化本地仓库

### 场景1：将现有项目初始化为Git仓库
```bash
# 进入项目目录
cd /path/to/your/project

# 初始化Git仓库
git init

# 添加所有文件到暂存区
git add .

# 提交更改
git commit -m "初始提交"
```

### 场景2：克隆线上仓库到本地
```bash
# 克隆仓库（HTTPS方式）
git clone https://github.com/用户名/仓库名.git

# 克隆仓库（SSH方式）
git clone git@github.com:用户名/仓库名.git

# 克隆到指定目录
git clone https://github.com/用户名/仓库名.git 自定义目录名
```

## 6. 基本Git工作流程

### 6.1 查看仓库状态
```bash
git status
```

### 6.2 添加文件到暂存区
```bash
# 添加单个文件
git add 文件名

# 添加所有文件
git add .

# 添加所有修改过的文件
git add -u
```

### 6.3 提交更改
```bash
# 提交并添加提交信息
git commit -m "提交说明"

# 提交并自动添加所有已跟踪文件的修改
git commit -am "提交说明"

# 修改上一次提交（不创建新的提交记录）
git commit --amend
```

### 6.4 查看提交历史
```bash
# 查看简洁的提交历史
git log --oneline

# 查看详细的提交历史
git log

# 查看图形化的提交历史
git log --graph --oneline --all
```

## 7. 分支管理

### 7.1 创建和切换分支
```bash
# 创建新分支
git branch 分支名

# 切换到新分支
git checkout 分支名

# 创建并切换到新分支（一步完成）
git checkout -b 分支名
```

### 7.2 查看分支
```bash
# 查看所有分支
git branch

# 查看所有分支（包括远程分支）
git branch -a
```

### 7.3 合并分支
```bash
# 切换到主分支
git checkout main

# 合并其他分支到当前分支
git merge 分支名
```

### 7.4 删除分支
```bash
# 删除本地分支
git branch -d 分支名

# 强制删除分支
git branch -D 分支名

# 删除远程分支
git push origin --delete 分支名
```

## 8. 远程仓库操作

### 8.1 添加远程仓库
```bash
# 添加远程仓库
git remote add origin https://github.com/用户名/仓库名.git

# 查看远程仓库
git remote -v
```

### 8.2 推送代码到远程仓库
```bash
# 首次推送（设置上游分支）
git push -u origin main

# 后续推送
git push

# 推送到特定分支
git push origin 分支名
```

### 8.3 拉取远程更新
```bash
# 拉取远程更新（不会自动合并）
git fetch

# 拉取并合并远程更新
git pull

# 拉取特定分支
git pull origin 分支名
```

## 9. 撤销操作

### 9.1 撤销工作区的修改
```bash
# 撤销单个文件的修改
git checkout -- 文件名

# 撤销所有修改
git checkout -- .
```

### 9.2 撤销暂存区的文件
```bash
# 将文件从暂存区移回工作区
git reset HEAD 文件名

# 将所有文件从暂存区移回工作区
git reset HEAD
```

### 9.3 撤销提交
```bash
# 撤销上一次提交，但保留修改
git reset --soft HEAD~1

# 撤销上一次提交，不保留修改
git reset --hard HEAD~1
```

## 10. 忽略文件（.gitignore）

创建`.gitignore`文件来指定不需要版本控制的文件：

```bash
# 创建.gitignore文件
touch .gitignore
```

示例`.gitignore`内容：
```
# 编译生成文件
*.o
*.exe
*.out
*.app

# 依赖目录
node_modules/
vendor/

# 环境配置文件
.env
.env.local

# 编辑器文件
.vscode/
.idea/
*.swp
*.swo

# 系统文件
.DS_Store
Thumbs.db

# 日志文件
*.log
```

## 11. 常用Git命令速查表

| 命令 | 说明 |
|------|------|
| `git init` | 初始化仓库 |
| `git clone <url>` | 克隆远程仓库 |
| `git add <file>` | 添加文件到暂存区 |
| `git commit -m "msg"` | 提交更改 |
| `git status` | 查看仓库状态 |
| `git log` | 查看提交历史 |
| `git branch` | 查看分支 |
| `git checkout <branch>` | 切换分支 |
| `git merge <branch>` | 合并分支 |
| `git pull` | 拉取远程更新 |
| `git push` | 推送到远程仓库 |
| `git remote -v` | 查看远程仓库 |
| `git diff` | 查看差异 |

## 12. 实际工作流程示例

### 示例1：日常开发流程
```bash
# 1. 拉取最新代码
git pull

# 2. 创建功能分支
git checkout -b feature/new-feature

# 3. 开发代码...
# 4. 添加修改
git add .

# 5. 提交更改
git commit -m "添加新功能"

# 6. 推送到远程
git push -u origin feature/new-feature

# 7. 创建Pull Request（在GitHub/GitLab界面上操作）
```

### 示例2：修复bug流程
```bash
# 1. 从主分支创建bug修复分支
git checkout main
git pull
git checkout -b hotfix/bug-fix

# 2. 修复bug...
# 3. 提交修复
git add .
git commit -m "修复bug"

# 4. 合并到主分支
git checkout main
git merge hotfix/bug-fix
git push

# 5. 删除临时分支
git branch -d hotfix/bug-fix
```

## 13. 高级配置

### 配置别名（简化常用命令）
```bash
# 添加别名
git config --global alias.co checkout
git config --global alias.br branch
git config --global alias.ci commit
git config --global alias.st status
git config --global alias.lg "log --oneline --graph --all"

# 使用别名
git co main      # 相当于 git checkout main
git st           # 相当于 git status
git lg           # 查看图形化日志
```

### 配置默认编辑器
```bash
# 使用VSCode作为默认编辑器
git config --global core.editor "code --wait"

# 使用Vim作为默认编辑器
git config --global core.editor "vim"
```

## 14. 故障排除

### 常见问题1：提交了错误的文件
```bash
# 修改上一次提交
git add 正确的文件
git commit --amend
```

### 常见问题2：冲突解决
```bash
# 拉取时发生冲突
git pull

# 手动解决冲突后
git add .
git commit -m "解决冲突"
git push
```

### 常见问题3：误删文件恢复
```bash
# 恢复误删的文件
git checkout -- 文件名
```

## 15. 学习资源

1. **官方文档**：https://git-scm.com/doc
2. **GitHub学习**：https://docs.github.com/zh/get-started
3. **交互式教程**：https://learngitbranching.js.org/
4. **Pro Git书籍**：https://git-scm.com/book/zh/v2

---

**总结**：Git是一个强大的版本控制工具，掌握基本命令后，你可以高效地管理代码版本、协作开发和备份项目。建议从简单的项目开始练习，逐步掌握更高级的功能。
