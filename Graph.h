#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define INFEASIBLE -1

#define INF INT_MAX   // 表示无穷大（无边）
#define MAXVEX 20     // 最大顶点数

//===============================================================
//  邻接矩阵存储结构
//===============================================================

typedef struct
{
    int arcs[MAXVEX][MAXVEX];  // 邻接矩阵
    int vex[MAXVEX];           // 顶点表（存顶点编号）
    int vexNum;                // 顶点数
    int arcNum;                // 边数
} MGraph;

// 创建无向网（带权图，以邻接矩阵存储）
void CreateMGraph(MGraph* G, int vexs[], int n, int edges[][3], int e);

// 打印邻接矩阵
void PrintMGraph(MGraph* G);

//===============================================================
//  邻接表存储结构
//===============================================================

// 边表节点
typedef struct ArcNode
{
    int adjVex;               // 邻接点编号
    int weight;               // 权值
    struct ArcNode* next;     // 指向下一条边
} ArcNode;

// 顶点表节点
typedef struct VNode
{
    int data;                 // 顶点信息
    ArcNode* firstArc;        // 第一条边
} VNode, AdjList[MAXVEX];

// 邻接表图
typedef struct
{
    AdjList vertices;         // 顶点数组
    int vexNum;               // 顶点数
    int arcNum;               // 边数
} ALGraph;

// 创建无向网（邻接表）
void CreateALGraph(ALGraph* G, int vexs[], int n, int edges[][3], int e);

// 打印邻接表
void PrintALGraph(ALGraph* G);

//===============================================================
//  图的遍历
//===============================================================

// DFS（邻接矩阵版）
void DFSM(MGraph* G, int v, int visited[]);

// BFS（邻接矩阵版）
void BFSM(MGraph* G, int v, int visited[]);

//===============================================================
//  最小生成树
//===============================================================

// Prim 算法
void Prim(MGraph* G, int start);

// Kruskal 算法（内部带并查集）
void Kruskal(MGraph* G);

//===============================================================
//  最短路径
//===============================================================

// Dijkstra 算法（单源最短路径）
void Dijkstra(MGraph* G, int start);

// Floyd 算法（多源最短路径）
void Floyd(MGraph* G);

//===============================================================
//  拓扑排序
//===============================================================

// 拓扑排序（基于邻接表）
Status TopologicalSort(ALGraph* G);

//===============================================================
//  关键路径
//===============================================================

// 关键路径（基于邻接表，AOE网）
Status CriticalPath(ALGraph* G);
