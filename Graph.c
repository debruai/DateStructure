// 数据结构 第七章 图
//
// 图的存储结构：
//   1. 邻接矩阵 — 适合稠密图，O(1) 判断两点是否相连
//   2. 邻接表 — 适合稀疏图，节省空间
//
// 核心算法（全部实现）：
//   遍历：DFS / BFS
//   最小生成树：Prim / Kruskal
//   最短路径：Dijkstra / Floyd
//   有向无环图：拓扑排序 / 关键路径

#include "Graph.h"

//===============================================================
//  辅助：队列（给 BFS 用）
//===============================================================

typedef struct
{
    int data[MAXVEX];
    int front, rear;
} Queue;

void InitQueue(Queue* Q)
{
    Q->front = Q->rear = 0;
}

int QueueEmpty(Queue* Q)
{
    return Q->front == Q->rear;
}

void EnQueue(Queue* Q, int v)
{
    Q->data[Q->rear++] = v;
}

int DeQueue(Queue* Q)
{
    return Q->data[Q->front++];
}

//===============================================================
//  邻接矩阵 — 创建与打印
//===============================================================

//---------------------------------------------------------------
//  CreateMGraph — 创建无向网（邻接矩阵）
//
//  参数：
//     G      — 图指针
//     vexs[] — 顶点编号数组
//     n      — 顶点数
//     edges[][3] — 边集数组，每行 {u, v, weight}
//     e      — 边数
//---------------------------------------------------------------
void CreateMGraph(MGraph* G, int vexs[], int n, int edges[][3], int e)
{
    G->vexNum = n;
    G->arcNum = e;

    // 存入顶点信息
    for (int i = 0; i < n; i++)
    {
        G->vex[i] = vexs[i];
    }

    // 初始化邻接矩阵为 INF
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            G->arcs[i][j] = (i == j) ? 0 : INF;  // 自己到自己的距离为0
        }
    }

    // 填入边
    for (int k = 0; k < e; k++)
    {
        int i = edges[k][0];    // 起点
        int j = edges[k][1];    // 终点
        int w = edges[k][2];    // 权值
        G->arcs[i][j] = w;
        G->arcs[j][i] = w;      // 无向图对称
    }
}

//---------------------------------------------------------------
//  PrintMGraph — 打印邻接矩阵
//---------------------------------------------------------------
void PrintMGraph(MGraph* G)
{
    printf("   邻接矩阵（INF=∞）：\n   ");
    for (int i = 0; i < G->vexNum; i++)
        printf("%4d ", G->vex[i]);
    printf("\n");

    for (int i = 0; i < G->vexNum; i++)
    {
        printf("   %d ", G->vex[i]);
        for (int j = 0; j < G->vexNum; j++)
        {
            if (G->arcs[i][j] == INF)
                printf("  ∞ ");
            else
                printf("%3d ", G->arcs[i][j]);
        }
        printf("\n");
    }
}

//===============================================================
//  邻接表 — 创建与打印
//===============================================================

//---------------------------------------------------------------
//  CreateALGraph — 创建无向网（邻接表）
//---------------------------------------------------------------
void CreateALGraph(ALGraph* G, int vexs[], int n, int edges[][3], int e)
{
    G->vexNum = n;
    G->arcNum = e;

    // 初始化顶点表
    for (int i = 0; i < n; i++)
    {
        G->vertices[i].data = vexs[i];
        G->vertices[i].firstArc = NULL;
    }

    // 插入边（无向图，每条边插两次）
    for (int k = 0; k < e; k++)
    {
        int i = edges[k][0];
        int j = edges[k][1];
        int w = edges[k][2];

        // 头插法：i → j
        ArcNode* p = (ArcNode*)malloc(sizeof(ArcNode));
        if (!p) exit(OVERFLOW);
        p->adjVex = j;
        p->weight = w;
        p->next = G->vertices[i].firstArc;
        G->vertices[i].firstArc = p;

        // 头插法：j → i
        ArcNode* q = (ArcNode*)malloc(sizeof(ArcNode));
        if (!q) exit(OVERFLOW);
        q->adjVex = i;
        q->weight = w;
        q->next = G->vertices[j].firstArc;
        G->vertices[j].firstArc = q;
    }
}

//---------------------------------------------------------------
//  PrintALGraph — 打印邻接表
//---------------------------------------------------------------
void PrintALGraph(ALGraph* G)
{
    printf("   邻接表：\n");
    for (int i = 0; i < G->vexNum; i++)
    {
        printf("   %d →", G->vertices[i].data);
        ArcNode* p = G->vertices[i].firstArc;
        while (p)
        {
            printf(" %d(%d)", p->adjVex, p->weight);
            p = p->next;
        }
        printf(" → NULL\n");
    }
}

//===============================================================
//  DFS（邻接矩阵版）
//===============================================================
//
//  算法思想（教材）：
//     1. 访问起始顶点 v
//     2. 从 v 的未访问邻接点中选一个继续 DFS
//     3. 直到没有未访问邻接点 → 回溯
//---------------------------------------------------------------
void DFSM(MGraph* G, int v, int visited[])
{
    visited[v] = 1;                       // 标记已访问
    printf("%d ", G->vex[v]);             // 访问

    for (int w = 0; w < G->vexNum; w++)
    {
        // 如果 v 和 w 之间有边，且 w 未被访问
        if (G->arcs[v][w] != 0 && G->arcs[v][w] != INF && !visited[w])
        {
            DFSM(G, w, visited);          // 递归访问 w
        }
    }
}

//===============================================================
//  BFS（邻接矩阵版）
//===============================================================
//
//  算法思想（教材）：
//     1. 访问起始顶点，入队
//     2. 出队，把它的所有未访问邻接点入队
//     3. 重复直到队列为空
//---------------------------------------------------------------
void BFSM(MGraph* G, int v, int visited[])
{
    Queue Q;
    InitQueue(&Q);

    visited[v] = 1;
    printf("%d ", G->vex[v]);
    EnQueue(&Q, v);

    while (!QueueEmpty(&Q))
    {
        int u = DeQueue(&Q);
        for (int w = 0; w < G->vexNum; w++)
        {
            if (G->arcs[u][w] != 0 && G->arcs[u][w] != INF && !visited[w])
            {
                visited[w] = 1;
                printf("%d ", G->vex[w]);
                EnQueue(&Q, w);
            }
        }
    }
}

//===============================================================
//  Prim 算法 — 最小生成树
//===============================================================
//
//  算法思想（贪心）：
//     1. 从某个顶点出发，把它加入 MST
//     2. 每次选择一个"到 MST 距离最短"的顶点加入
//     3. 更新新加入顶点的邻接点到 MST 的距离
//     4. 重复直到所有顶点都在 MST 中
//
//  关键：lowcost 数组
//     lowcost[i] = 顶点 i 到 MST 的最小距离
//     lowcost[i] = 0 表示顶点 i 已经入 MST
//
//  教材算法 7.7
//---------------------------------------------------------------
void Prim(MGraph* G, int start)
{
    int lowcost[MAXVEX];   // 各顶点到 MST 的最小距离
    int adjvex[MAXVEX];    // 记录每个顶点是被谁拉入 MST 的
    int totalWeight = 0;

    // 初始化：从 start 出发
    for (int i = 0; i < G->vexNum; i++)
    {
        lowcost[i] = G->arcs[start][i];
        adjvex[i] = start;
    }
    lowcost[start] = 0;  // start 已入 MST

    printf("   最小生成树（Prim）：\n");
    printf("   从顶点 %d 出发\n", G->vex[start]);

    // 每次加一个顶点，共 n-1 趟
    for (int t = 1; t < G->vexNum; t++)
    {
        // 找 lowcost 中最小且非 0 的顶点
        int min = INF;
        int k = -1;
        for (int i = 0; i < G->vexNum; i++)
        {
            if (lowcost[i] != 0 && lowcost[i] < min)
            {
                min = lowcost[i];
                k = i;
            }
        }

        if (k == -1) break;  // 图不连通

        printf("   边 %d -- %d  权值: %d\n",
               G->vex[adjvex[k]], G->vex[k], lowcost[k]);
        totalWeight += lowcost[k];
        lowcost[k] = 0;  // 顶点 k 入 MST

        // 更新：加入 k 后，更新 lowcost
        for (int j = 0; j < G->vexNum; j++)
        {
            if (lowcost[j] != 0 && G->arcs[k][j] < lowcost[j])
            {
                lowcost[j] = G->arcs[k][j];
                adjvex[j] = k;
            }
        }
    }

    printf("   MST 总权值: %d\n", totalWeight);
}

//===============================================================
//  Kruskal 算法 — 最小生成树
//===============================================================
//
//  算法思想（贪心）：
//     1. 把所有边按权值从小到大排序
//     2. 从小到大选边，如果加入不会形成环就选
//     3. 直到选了 n-1 条边
//
//  判环方法：并查集（Union-Find）
//     每条边加入时，检查两个端点是否在同一集合
//     如果不在同一集合 → 安全，合并集合
//     如果在同一集合 → 会形成环，跳过
//
//  教材算法 7.8
//---------------------------------------------------------------

// 边集结构（给 Kruskal 用）
typedef struct
{
    int u, v;    // 边的两个端点
    int w;       // 权值
} Edge;

// ---------- 并查集（仅 Kruskal 内部使用）----------
int Find(int parent[], int x)
{
    // 带路径压缩的查找
    if (parent[x] != x)
        parent[x] = Find(parent, parent[x]);
    return parent[x];
}

void Union(int parent[], int x, int y)
{
    int rx = Find(parent, x);
    int ry = Find(parent, y);
    if (rx != ry)
        parent[rx] = ry;
}

// ---------- 边排序比较函数 ----------
int CmpEdge(const void* a, const void* b)
{
    return ((Edge*)a)->w - ((Edge*)b)->w;
}

// ---------- Kruskal 主体 ----------
void Kruskal(MGraph* G)
{
    int n = G->vexNum;

    // 提取所有边到边集数组
    Edge edges[MAXVEX * MAXVEX];
    int edgeCount = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)  // 只处理上三角，无向图
        {
            if (G->arcs[i][j] != INF && G->arcs[i][j] != 0)
            {
                edges[edgeCount].u = i;
                edges[edgeCount].v = j;
                edges[edgeCount].w = G->arcs[i][j];
                edgeCount++;
            }
        }
    }

    // 按权值从小到大排序
    qsort(edges, edgeCount, sizeof(Edge), CmpEdge);

    // 初始化并查集
    int parent[MAXVEX];
    for (int i = 0; i < n; i++)
        parent[i] = i;

    printf("   最小生成树（Kruskal）：\n");

    int totalWeight = 0;
    int selected = 0;

    for (int i = 0; i < edgeCount && selected < n - 1; i++)
    {
        int ru = Find(parent, edges[i].u);
        int rv = Find(parent, edges[i].v);

        if (ru != rv)  // 不在同一集合 → 不会形成环
        {
            printf("   边 %d -- %d  权值: %d\n",
                   G->vex[edges[i].u], G->vex[edges[i].v], edges[i].w);
            totalWeight += edges[i].w;
            Union(parent, ru, rv);
            selected++;
        }
    }

    printf("   MST 总权值: %d\n", totalWeight);
}

//===============================================================
//  Dijkstra 算法 — 单源最短路径
//===============================================================
//
//  算法思想（贪心）：
//     1. dist[i] = 从源点到顶点 i 的当前最短距离
//     2. 每次选 dist 最小且未确定的顶点，标记为已确定
//     3. 用该顶点更新它的邻接点的 dist
//     4. 重复直到所有顶点都确定
//
//  教材算法 7.9
//---------------------------------------------------------------
void Dijkstra(MGraph* G, int start)
{
    int n = G->vexNum;
    int dist[MAXVEX];      // 源点到各顶点的最短距离
    int path[MAXVEX];      // 记录前驱顶点
    int finalized[MAXVEX]; // 1=已确定最短路径

    // 初始化
    for (int i = 0; i < n; i++)
    {
        dist[i] = G->arcs[start][i];
        finalized[i] = 0;
        path[i] = (dist[i] < INF && i != start) ? start : -1;
    }
    finalized[start] = 1;  // 源点已确定
    dist[start] = 0;

    // 每次确定一个顶点，共 n-1 次
    for (int t = 1; t < n; t++)
    {
        // 找未确定顶点中 dist 最小的
        int min = INF;
        int k = -1;
        for (int i = 0; i < n; i++)
        {
            if (!finalized[i] && dist[i] < min)
            {
                min = dist[i];
                k = i;
            }
        }

        if (k == -1) break;  // 剩下的不可达
        finalized[k] = 1;    // 确定顶点 k

        // 用 k 更新邻接点
        for (int j = 0; j < n; j++)
        {
            if (!finalized[j]
                && G->arcs[k][j] != INF
                && dist[k] + G->arcs[k][j] < dist[j])
            {
                dist[j] = dist[k] + G->arcs[k][j];
                path[j] = k;
            }
        }
    }

    // 打印结果
    printf("   Dijkstra（从 %d 出发）：\n", G->vex[start]);
    for (int i = 0; i < n; i++)
    {
        if (i == start) continue;

        printf("   到 %d", G->vex[i]);
        if (dist[i] == INF)
        {
            printf("：不可达\n");
        }
        else
        {
            printf("：最短距离 %d 路径：", dist[i]);

            // 逆推路径
            int stack[MAXVEX], top = 0;
            int cur = i;
            while (cur != -1)
            {
                stack[top++] = cur;
                cur = path[cur];
            }
            while (top > 0)
            {
                top--;
                printf("%d", G->vex[stack[top]]);
                if (top > 0) printf(" → ");
            }
            printf("\n");
        }
    }
}

//===============================================================
//  Floyd 算法 — 多源最短路径
//===============================================================
//
//  算法思想（动态规划）：
//     D^{(k)}[i][j] = 从 i 到 j 中间顶点编号 ≤ k 的最短路径长度
//     递推公式：
//       D^{(0)}[i][j] = arcs[i][j]
//       D^{(k)}[i][j] = min(D^{(k-1)}[i][j],
//                           D^{(k-1)}[i][k] + D^{(k-1)}[k][j])
//
//  简单理解：逐个尝试用 k 作为中间点，"绕路会不会更短？"
//
//  教材算法 7.10
//---------------------------------------------------------------
void Floyd(MGraph* G)
{
    int n = G->vexNum;
    int D[MAXVEX][MAXVEX];   // 最短路径长度矩阵
    int P[MAXVEX][MAXVEX];   // 路径矩阵：P[i][j] 是 i→j 的前驱

    // 初始化 D 和 P
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            D[i][j] = G->arcs[i][j];
            P[i][j] = (D[i][j] < INF && i != j) ? i : -1;
        }
    }

    // 三重循环核心
    for (int k = 0; k < n; k++)          // 中间顶点
    {
        for (int i = 0; i < n; i++)      // 起点
        {
            if (D[i][k] == INF) continue;
            for (int j = 0; j < n; j++)  // 终点
            {
                if (D[k][j] == INF) continue;

                // 如果绕道 k 更短，更新
                if (D[i][k] + D[k][j] < D[i][j])
                {
                    D[i][j] = D[i][k] + D[k][j];
                    P[i][j] = P[k][j];  // 更新前驱
                }
            }
        }
    }

    // 打印
    printf("   Floyd 多源最短路径结果：\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j) continue;
            printf("   %d → %d", G->vex[i], G->vex[j]);
            if (D[i][j] == INF)
            {
                printf("：不可达\n");
            }
            else
            {
                printf("：%d  路径：%d", D[i][j], G->vex[i]);
                // 沿着 P 矩阵推路径
                int pre = P[i][j];
                // 从 i 出发，倒推前驱
                int stack[MAXVEX], top = 0;
                while (pre != i && pre != -1)
                {
                    stack[top++] = pre;
                    pre = P[i][pre];
                }
                while (top > 0)
                {
                    top--;
                    printf(" → %d", G->vex[stack[top]]);
                }
                printf(" → %d\n", G->vex[j]);
            }
        }
    }
}

//===============================================================
//  拓扑排序
//===============================================================
//
//  应用场景：AOV 网（Activity On Vertex）
//   顶点表示活动，边表示活动之间的先后依赖关系
//
//  算法思想：
//     1. 计算所有顶点的入度
//     2. 将入度为 0 的顶点入栈（或队）
//     3. 出栈，输出，将该顶点的邻接点入度 -1
//     4. 如果邻接点入度变为 0，入栈
//     5. 重复直到栈空
//     6. 如果输出的顶点数 < 总顶点数 → 图中有环
//
//  教材算法 7.11
//---------------------------------------------------------------
Status TopologicalSort(ALGraph* G)
{
    int n = G->vexNum;
    int indegree[MAXVEX] = {0};  // 入度表

    // 计算入度
    for (int i = 0; i < n; i++)
    {
        ArcNode* p = G->vertices[i].firstArc;
        while (p)
        {
            indegree[p->adjVex]++;  // i → adjVex，adjVex 入度 +1
            p = p->next;
        }
    }

    // 入度为 0 的顶点入栈
    int stack[MAXVEX], top = 0;
    for (int i = 0; i < n; i++)
    {
        if (indegree[i] == 0)
        {
            stack[top++] = i;
        }
    }

    printf("   拓扑排序：");
    int count = 0;  // 已输出的顶点数

    while (top > 0)
    {
        int v = stack[--top];           // 出栈
        printf("%d ", G->vertices[v].data);
        count++;

        // 邻接点入度 -1
        ArcNode* p = G->vertices[v].firstArc;
        while (p)
        {
            int adj = p->adjVex;
            indegree[adj]--;
            if (indegree[adj] == 0)
            {
                stack[top++] = adj;     // 入度变 0 就入栈
            }
            p = p->next;
        }
    }

    printf("\n");

    if (count < n)
    {
        printf("   图中有环！拓扑排序失败（输出了 %d/%d 个顶点）\n", count, n);
        return ERROR;
    }

    printf("   拓扑排序成功（输出了全部 %d 个顶点）\n", count);
    return OK;
}

//===============================================================
//  关键路径
//===============================================================
//
//  应用场景：AOE 网（Activity On Edge）
//   边表示活动，顶点表示事件
//   活动有持续时间，找"最长的路径" → 关键路径
//
//  四个关键量：
//     ve[k] — 事件 k 的最早发生时间
//     vl[k] — 事件 k 的最晚发生时间
//     ee[i] — 活动 i 的最早开始时间（= ve[起点]）
//     el[i] — 活动 i 的最晚开始时间（= vl[终点] - duration）
//
//  关键活动：ee[i] == el[i] 的活动
//  关键路径：由关键活动组成的路径
//
//  教材算法 7.12 — 7.14
//---------------------------------------------------------------
Status CriticalPath(ALGraph* G)
{
    int n = G->vexNum;

    // ---------- 第一步：拓扑排序，求 ve ----------
    int indegree[MAXVEX] = {0};
    int ve[MAXVEX] = {0};       // 最早发生时间
    int topoStack[MAXVEX];      // 拓扑序列栈

    // 计算入度
    for (int i = 0; i < n; i++)
    {
        ArcNode* p = G->vertices[i].firstArc;
        while (p)
        {
            indegree[p->adjVex]++;
            p = p->next;
        }
    }

    // 入度为 0 的顶点入临时栈
    int stack[MAXVEX], top = 0;
    for (int i = 0; i < n; i++)
    {
        if (indegree[i] == 0)
        {
            stack[top++] = i;
        }
    }

    int topoCount = 0;

    while (top > 0)
    {
        int v = stack[--top];
        topoStack[topoCount++] = v;  // 保存拓扑序列

        ArcNode* p = G->vertices[v].firstArc;
        while (p)
        {
            int adj = p->adjVex;
            // 更新 ve：ve[adj] = max(ve[adj], ve[v] + weight)
            if (ve[v] + p->weight > ve[adj])
            {
                ve[adj] = ve[v] + p->weight;
            }

            indegree[adj]--;
            if (indegree[adj] == 0)
            {
                stack[top++] = adj;
            }
            p = p->next;
        }
    }

    if (topoCount < n)
    {
        printf("   图中有环！无法求关键路径\n");
        return ERROR;
    }

    // ---------- 第二步：逆拓扑序列求 vl ----------
    int vl[MAXVEX];
    for (int i = 0; i < n; i++)
    {
        vl[i] = ve[topoStack[topoCount - 1]];  // 初始化为汇点的 ve
    }

    // 逆序遍历拓扑序列
    for (int i = topoCount - 1; i >= 0; i--)
    {
        int v = topoStack[i];
        ArcNode* p = G->vertices[v].firstArc;
        while (p)
        {
            int adj = p->adjVex;
            // vl[v] = min(vl[v], vl[adj] - weight)
            if (vl[adj] - p->weight < vl[v])
            {
                vl[v] = vl[adj] - p->weight;
            }
            p = p->next;
        }
    }

    // ---------- 第三步：求 ee 和 el，找关键活动 ----------
    printf("   关键路径分析：\n");
    printf("   活动    | 最早  最晚   差值  关键活动？\n");
    printf("   --------|-----------------------------\n");

    for (int v = 0; v < n; v++)
    {
        ArcNode* p = G->vertices[v].firstArc;
        while (p)
        {
            int adj = p->adjVex;
            int ee = ve[v];               // 活动最早：起点事件最早
            int el = vl[adj] - p->weight;  // 活动最晚：终点事件最晚 - 时长
            int diff = el - ee;

            printf("   %d → %d  | (时长 %d) 最早=%3d  最晚=%3d  差值=%+2d",
                   G->vertices[v].data, G->vertices[adj].data,
                   p->weight, ee, el, diff);

            if (diff == 0)
            {
                printf("     ★关键活动\n");
            }
            else
            {
                printf("\n");
            }
            p = p->next;
        }
    }

    // 打印 ve 和 vl
    printf("\n   事件参数：\n");
    printf("   顶点 | ve  vl\n");
    printf("   -----|-------\n");
    for (int i = 0; i < n; i++)
    {
        printf("    %d   | %2d  %2d\n", G->vertices[i].data, ve[i], vl[i]);
    }

    printf("\n   关键路径工程总工期: %d\n", ve[topoStack[topoCount - 1]]);

    return OK;
}

//===============================================================
//  main — 测试图的所有算法
//===============================================================
int main()
{
    printf("======= 图测试 =======\n\n");

    // 教材图 7.33 — 无向网（带权图）
    // 顶点：{0, 1, 2, 3, 4, 5}
    // 边：(0,1)=6 (0,2)=1 (0,3)=5 (1,2)=5 (1,4)=3
    //      (2,3)=5 (2,4)=6 (2,5)=4 (3,5)=2 (4,5)=6
    int vexs[] = {0, 1, 2, 3, 4, 5};
    int edges[][3] = {
        {0, 1, 6}, {0, 2, 1}, {0, 3, 5},
        {1, 2, 5}, {1, 4, 3},
        {2, 3, 5}, {2, 4, 6}, {2, 5, 4},
        {3, 5, 2},
        {4, 5, 6}
    };
    int n = 6, e = 10;

    //========================================
    //  第一部分：图的存储
    //========================================
    printf("========== 一、图的存储 ==========\n\n");

    MGraph MG;
    CreateMGraph(&MG, vexs, n, edges, e);
    printf("1. 邻接矩阵：\n");
    PrintMGraph(&MG);

    ALGraph ALG;
    CreateALGraph(&ALG, vexs, n, edges, e);
    printf("\n2. 邻接表：\n");
    PrintALGraph(&ALG);

    //========================================
    //  第二部分：图的遍历
    //========================================
    printf("\n========== 二、图的遍历 ==========\n\n");

    int visited[MAXVEX] = {0};
    printf("1. DFS（从顶点 0 出发）：\n   ");
    DFSM(&MG, 0, visited);
    printf("\n");

    for (int i = 0; i < n; i++) visited[i] = 0;
    printf("\n2. BFS（从顶点 0 出发）：\n   ");
    BFSM(&MG, 0, visited);
    printf("\n");

    //========================================
    //  第三部分：最小生成树
    //========================================
    printf("\n========== 三、最小生成树 ==========\n\n");

    Prim(&MG, 0);
    printf("\n");
    Kruskal(&MG);

    //========================================
    //  第四部分：最短路径
    //========================================
    printf("\n========== 四、最短路径 ==========\n\n");

    Dijkstra(&MG, 0);
    printf("\n");
    Floyd(&MG);

    //========================================
    //  第五部分：拓扑排序（用有向无环图）
    //========================================
    printf("\n========== 五、拓扑排序 ==========\n\n");

    // 教材 AOV 网：6 个顶点 (0,1,2,3,4,5)
    // 边（有向）：
    // 0→1, 0→2, 0→3, 1→4, 2→4, 3→4, 4→5
    // 权值不重要（拓扑排序不看权值）
    int topoVexs[] = {0, 1, 2, 3, 4, 5};
    int topoEdges[][3] = {
        {0, 1, 1}, {0, 2, 1}, {0, 3, 1},
        {1, 4, 1}, {2, 4, 1}, {3, 4, 1},
        {4, 5, 1}
    };
    int topoN = 6, topoE = 7;

    ALGraph topoG;
    // 有向图的邻接表（只插一次）
    topoG.vexNum = topoN;
    topoG.arcNum = topoE;
    for (int i = 0; i < topoN; i++)
    {
        topoG.vertices[i].data = topoVexs[i];
        topoG.vertices[i].firstArc = NULL;
    }
    for (int k = 0; k < topoE; k++)
    {
        ArcNode* p = (ArcNode*)malloc(sizeof(ArcNode));
        if (!p) exit(OVERFLOW);
        p->adjVex = topoEdges[k][1];
        p->weight = topoEdges[k][2];
        p->next = topoG.vertices[topoEdges[k][0]].firstArc;
        topoG.vertices[topoEdges[k][0]].firstArc = p;
    }

    TopologicalSort(&topoG);

    //========================================
    //  第六部分：关键路径
    //========================================
    printf("\n========== 六、关键路径 ==========\n\n");

    // 教材 AOE 网：6 个事件, 8 个活动
    // 顶点：0, 1, 2, 3, 4, 5（0=源点, 5=汇点）
    // 有向边（活动）及其持续时间：
    // 0→1(3)  0→2(2)  1→3(2)  1→4(3)
    // 2→3(4)  2→5(3)  3→4(2)  3→5(1)  4→5(2)
    int cpVexs[] = {0, 1, 2, 3, 4, 5};
    int cpEdges[][3] = {
        {0, 1, 3}, {0, 2, 2},
        {1, 3, 2}, {1, 4, 3},
        {2, 3, 4}, {2, 5, 3},
        {3, 4, 2}, {3, 5, 1},
        {4, 5, 2}
    };
    int cpN = 6, cpE = 9;

    ALGraph cpG;
    cpG.vexNum = cpN;
    cpG.arcNum = cpE;
    for (int i = 0; i < cpN; i++)
    {
        cpG.vertices[i].data = cpVexs[i];
        cpG.vertices[i].firstArc = NULL;
    }
    for (int k = 0; k < cpE; k++)
    {
        ArcNode* p = (ArcNode*)malloc(sizeof(ArcNode));
        if (!p) exit(OVERFLOW);
        p->adjVex = cpEdges[k][1];
        p->weight = cpEdges[k][2];
        p->next = cpG.vertices[cpEdges[k][0]].firstArc;
        cpG.vertices[cpEdges[k][0]].firstArc = p;
    }

    CriticalPath(&cpG);

    // 释放图
    for (int i = 0; i < cpN; i++)
    {
        ArcNode* p = cpG.vertices[i].firstArc;
        while (p)
        {
            ArcNode* q = p;
            p = p->next;
            free(q);
        }
    }
    for (int i = 0; i < topoN; i++)
    {
        ArcNode* p = topoG.vertices[i].firstArc;
        while (p)
        {
            ArcNode* q = p;
            p = p->next;
            free(q);
        }
    }

    printf("\n======= 测试完成 =======\n");

    return 0;
}
