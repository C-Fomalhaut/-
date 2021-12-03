// Reference: https://blog.csdn.net/qq_18824403/article/details/105699989
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#define MAX_V   100
#define INF     ~0U >> 1

// typedef pair<int, int> SPT;
typedef struct Node // Construct edge table in adjacency linked list
{
    int adjVertex;          // Record the other point that shares the edge with a vertex
    int weight;             // Record the weight of the edge
    struct Node *next;      // Point to the next edge
} Node;

typedef struct Vertex // Construct vertex table in adjacency linked list
{
    int data;               // Record the index of the vertex
    int mark;               // Records whether the vertex has been accessed
    int prev;               // Record the predecessor in DFS
    struct Node *first;     // Point to the edge table
} Vertex;

typedef struct Edge // Construct the edge set
{
    int v1, v2, weight;     // Record two vertices and weight of the edge
} Edge;

typedef struct Graph // Construct the adjacency linked list
{
    int a, b;                       // Record the number of vertex and edge
    int *p;                         // Record the vertex we are accessing
    struct Edge *arr;               // Point to the edge set
    struct Vertex adjList[MAX_V];   // The adjacency array
} Graph;

typedef struct MST // Record the edge in MST
{
    int x, y;
} MST;

int num = 0;    // Use to judge the connectivity of graph
int init = 0;   // Starting point of DFS

void sortWeight(Graph *g) // Sort from small to large according to the weight of the edges
{
    for (int i = 0; i < g->b; ++ i)
	{
		for (int j = g->b - 1; j > i; -- j)
		{
			if (g->arr[i].weight > g->arr[j].weight)
			{
				g->arr[g->b] = g->arr[i];
				g->arr[i] = g->arr[j];
				g->arr[j] = g->arr[g->b];
			}
		}
	}
    // for (int i = 0;  i < g->b; ++ i)
    // {
    //     printf("%d, %d, %d\n", g->arr[i].v1, g->arr[i].v2, g->arr[i].weight); 
    // }
}

void dfs(Graph *g, int v) // DFS traversal
{
    Node *p;
	int w;
    if (g->adjList[v].data == init)
    {
        num ++;
        g->adjList[v].prev = init;
    }
    else
    {
	    printf("%d--%d ", g->adjList[v].prev, g->adjList[v].data);
    }
    
	g->adjList[v].mark = 1;
	p = g->adjList[v].first;

	while (NULL != p)
	{
		w = p->adjVertex;
        g->adjList[w].prev = v;
		if (g->adjList[w].mark == 0)
		{
            num ++;
			dfs(g, w);
		}
		p = p->next;
	}
}

int findSet(int *parent, int p) // Find points that are not added to the edge set
{
    while (parent[p] != -1) // Trace the parent point until hit the root
    {
        p = parent[p];
    }
    return p;
}

int finish(Graph *g, int *parent) // Determine whether the MST is completed
{
    int cnt = 0;
    for (int i = 0; i < g->a; ++ i)
    {
        if (parent[i] != -1)
        {
            cnt ++;
        }
    }

    if (cnt == g->a - 1) // The completion flag is that the edge of the MST is equal to the number of vertices minus 1
    {
        return 1;
    }
    return 0;
}

int kruskal(Graph *g, MST *m)
{
    int pos = 0;
    // int sum = 0;
    int parent[MAX_V]; // Trace the parent node
    // int height[MAX_V];

    if (g->b == 1) // Only one vertex
    {
        m[pos].x = g->arr[0].v1;
        m[pos].y = g->arr[0].v2;
        return 1;
    }

    for (int i = 0; i < g->b; ++ i) // Initialize
    {
        parent[i] = -1;
        // height[i] = 0;
    }

    for (int i = 0; i < g->b; ++ i)
    {
        int a = findSet(parent, g->arr[i].v1);
        int b = findSet(parent, g->arr[i].v2);

        if (a != b) // a and b are not on the same spanning tree
        {
            parent[a] = b; // Connect a as a child of B
            // printf("%d->%d = %d\n", g->arr[i].v1, g->arr[i].v2, g->arr[i].weight);
            m[pos].x = g->arr[i].v1; // Record the edge into the edge set
            m[pos].y = g->arr[i].v2;
            pos ++;
            // sum += g->arr[i].weight;
        }

        if (finish(g, parent)) // Finish to construct MST
        {
            return 1;
        }
    }
    return 0;
}

void dijkstra(Graph *g, int v)
{
    Node *p;
    /*
        pw: Point being accessed
        min: Temporarily record current minimum value
        dist: Record the current path length
        pred: Record predecessor node
        color: Record whether the point has been accessed
    */
    int pw = -1, min, dist[MAX_V], pred[MAX_V], color[MAX_V];
    for (int i = 0; i < g->a; ++ i) // Initialize
    {
        dist[i] = INF;
        pred[i] = -1;
        color[i] = 0;
    }
    dist[v] = 0;
    // color[v] = 1;

    for (int i = 0; i < g->a - 1; ++ i) // Find the point with the shortest path that has not been accessed
    {
        min = INF;
        for (int j = 0; j < g->a; ++ j)
        {
            if (!color[j] && dist[j] < min)
            {
                pw = j; // Record the point
                min = dist[j]; // Record the path length
            }
        }

        if (pw == -1)
        {
            printf("Graph unconnected!");
            break;
        }
        color[pw] = 1; // Mark as accessed
        p = g->adjList[pw].first;

        while (p) // Traverse the adjacent nodes of the current point and find the shortest path
        {
            int tmp = dist[pw] + p->weight;
            if (tmp < dist[p->adjVertex]) // Find shorter path, update record
            {
                dist[p->adjVertex] = tmp;
                pred[p->adjVertex] = pw;
                // printf("%d--%d ", g->adjList[pw].data, g->adjList[p->adjVertex].data);
            }
            p = p->next;
        }
    }

    for (int i = 0; i < g->a; ++ i)
    {
        if (i != v)
            printf("%d--%d ", pred[i], i);
    }
}

void inputList(Graph *g)
{
    Node *curr;
    for (int i = 0; i < g->a; ++ i)
    {
        printf("%d->", g->adjList[i].data);
        curr = g->adjList[i].first;

        while (curr != NULL)
        {
            printf("%d->", curr->adjVertex);
            curr = curr->next;
        }
        printf("\n");
    }
}

int main()
{
    char filename[100];
    printf("***************\n");
    printf("Input the file name: \n"); 

    while (scanf("%s", filename) != EOF)
    {
        Graph *g = (Graph *)malloc(sizeof(Graph));
        FILE* fp = fopen(filename, "r+"); // Open the file

        if (fp == NULL) // No such file
        {
            printf("***************\n");
            printf("The file does not exist, please input again!\n");
            printf("***************\n");
            printf("Input the file name: \n");
            continue;
        }
        fscanf(fp, "%d", &(g->a));
        fscanf(fp, "%d", &(g->b));

        if (g->a == 0) // No vertex
        {
            printf("***************\n");
            printf("No vertex is failed!\n");
            printf("***************\n");
            printf("Input the file name: \n");
            free(g);
            g = NULL;
            continue;
        }

        if (g->b == 0) // No edge
        {
            printf("***************\n");
            printf("No edge is failed!\n");
            printf("***************\n");
            printf("Input the file name: \n");
            free(g);
            g = NULL;
            continue;
        }

        g->p = (int *)malloc(sizeof(int) * g->a);
        g->arr = (Edge *)malloc(sizeof(Edge) * (g->b + 1));

        for (int i = 0; i < g->a; ++ i) // Initialize
        {
            g->adjList[i].data = i;
            g->adjList[i].mark = 0;
            g->adjList[i].first = NULL;
            g->p[i] = g->adjList[i].data;
        }

        for (int i = 0; i < g->b; ++ i) // Read the data
        {
            int fv, sv, value;
            fscanf(fp, "%d, ", &fv);
            fscanf(fp, "%d, ", &sv);
            fscanf(fp, "%d\n", &value);
            g->arr[i].v1 = fv; // Record the 1st vertex
            g->arr[i].v2 = sv; // Record the 2nd vertex
            g->arr[i].weight = value; // Record the weight

            // Insert the subsequent vertices into the tail of the linked list
            // Because it is an undirected graph, it should be inserted in both directions
            Node *p, *d = (Node *)malloc(sizeof(Node)); // Record an adjacency vertex
            d->adjVertex = sv;
            d->weight = value;
            d->next = NULL;
            p = g->adjList[fv].first;

            if (p == NULL) // First adjacent vertex
            {
                g->adjList[fv].first = d;
            }
            else // Other adjacent vertice
            {
                while (p->next != NULL)
                {
                    p = p->next;
                }
                p->next = d;
            }

            d = (Node *)malloc(sizeof(Node));
            d->adjVertex = fv;
            d->weight = value;
            d->next = NULL;
            p = g->adjList[sv].first;

            if (p == NULL)
            {
                g->adjList[sv].first = d;
            }
            else
            {
                while (p->next != NULL)
                {
                    p = p->next;
                }
                p->next = d;
            }
        }
        fclose(fp); // Close the file
        fp = NULL;
        sortWeight(g);
        // inputList(g);

        printf("***************\n");
        printf("1. The following are the Nodes in the constructed DFS Tree: \n");
        dfs(g, init);
        printf("\n");

        if (num < g->a) // Unconnected Graph
        {
            printf("***************\n");
            printf("2. The articulation point(s) found in the given graph is/are: \n");
            printf("No result!\n");
            printf("***************\n");
            printf("3. The following are the edges in the constructed MST: \n");
            printf("No result!\n");
            printf("***************\n");
            printf("4. The following are the edges in the constructed SPT: \n");
            printf("No result!\n");
            printf("***************\n");
            printf("Unconnected Graph! Can only do DFS!\n");
            printf("***************\n");
            printf("Input the file name: \n");
            free(g);
            g = NULL;
            num = 0;
            continue;
        }

        printf("***************\n");
        printf("2. The articulation point(s) found in the given graph is/are: \n");
        printf("I do not choose to complete this question.\n");
        printf("***************\n");
        printf("3. The following are the edges in the constructed MST: \n");
        MST *m = (MST *)malloc(sizeof(MST) * g->a - 1);
        int res = kruskal(g, m);

        if (res) // Output in ascending order of vertex index
        {
            for (int i = g->a - 2; i > 0; -- i)
            {
                for (int j = 0; j < i; ++ j)
                {
                    if (m[j].x > m[j + 1].x || (m[j].x == m[j + 1].x && m[j].y > m[j + 1].y))
                    {
                        int v1 = m[j].x, v2 = m[j].y;
                        int v3 = m[j + 1].x, v4 = m[j + 1].y;
                        m[j].x = v3;
                        m[j].y = v4;
                        m[j + 1].x = v1;
                        m[j + 1].y = v2;
                    }
                }
            }

            for (int i = 0; i < g->a - 1; ++ i)
            {
                printf("%d--%d ", m[i].x, m[i].y);
            }
            printf("\n");
        }
        else
        {
            printf("Construct MST failed!");
        }

        printf("***************\n");
        printf("4. The following are the edges in the constructed SPT: \n");
        dijkstra(g, init);
        printf("\n");
        
        free(m);
        m = NULL;
        free(g);
        g = NULL;
        num = 0;
        printf("***************\n");
        printf("Input the file name: \n");
    }
    return 0;
}