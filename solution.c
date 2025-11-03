#include <stdio.h>
#include <stdlib.h>

typedef struct Edge {
    int to;
    int next;
} Edge;

static int lca_query(int a, int b, int **up, int *depth, int LOG);

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) {
        return 0;
    }
    if (n <= 0) {
        return 0;
    }

    Edge *edges = NULL;
    if (n > 1) {
        edges = (Edge *)malloc((n - 1) * sizeof(Edge));
        if (!edges) {
            return 0;
        }
    }

    int *head = (int *)malloc((n + 1) * sizeof(int));
    if (!head) {
        free(edges);
        return 0;
    }
    for (int i = 1; i <= n; ++i) {
        head[i] = -1;
    }

    int edge_cnt = 0;
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            free(edges);
            free(head);
            return 0;
        }
        edges[edge_cnt].to = v;
        edges[edge_cnt].next = head[u];
        head[u] = edge_cnt++;
    }

    int x, y;
    if (scanf("%d %d", &x, &y) != 2) {
        free(edges);
        free(head);
        return 0;
    }

    int LOG = 1;
    while ((1 << LOG) <= n) {
        ++LOG;
    }

    int **up = (int **)malloc(LOG * sizeof(int *));
    if (!up) {
        free(edges);
        free(head);
        return 0;
    }
    for (int i = 0; i < LOG; ++i) {
        up[i] = (int *)calloc(n + 1, sizeof(int));
        if (!up[i]) {
            for (int j = 0; j < i; ++j) {
                free(up[j]);
            }
            free(up);
            free(edges);
            free(head);
            return 0;
        }
    }

    int *depth = (int *)calloc(n + 1, sizeof(int));
    int *level_count = (int *)calloc(n + 2, sizeof(int));
    int *stack = (int *)malloc(n * sizeof(int));
    if (!depth || !level_count || !stack) {
        for (int i = 0; i < LOG; ++i) {
            free(up[i]);
        }
        free(up);
        free(edges);
        free(head);
        free(depth);
        free(level_count);
        free(stack);
        return 0;
    }

    int top = 0;
    stack[top++] = 1;
    depth[1] = 1;
    int max_depth = 1;
    int max_team = 0;

    while (top > 0) {
        int node = stack[--top];
        level_count[depth[node]]++;
        if (level_count[depth[node]] > max_team) {
            max_team = level_count[depth[node]];
        }
        if (depth[node] > max_depth) {
            max_depth = depth[node];
        }
        for (int i = head[node]; i != -1; i = edges[i].next) {
            int child = edges[i].to;
            depth[child] = depth[node] + 1;
            up[0][child] = node;
            stack[top++] = child;
        }
    }

    for (int k = 1; k < LOG; ++k) {
        for (int v = 1; v <= n; ++v) {
            int mid = up[k - 1][v];
            up[k][v] = mid ? up[k - 1][mid] : 0;
        }
    }

    int ancestor = lca_query(x, y, up, depth, LOG);
    int latency = 2 * (depth[x] - depth[ancestor]) + (depth[y] - depth[ancestor]);

    printf("%d\n%d\n%d\n", max_depth, max_team, latency);

    if (edges) {
        free(edges);
    }
    free(head);
    for (int i = 0; i < LOG; ++i) {
        free(up[i]);
    }
    free(up);
    free(depth);
    free(level_count);
    free(stack);

    return 0;
}

static int lca_query(int a, int b, int **up, int *depth, int LOG) {
    if (depth[a] < depth[b]) {
        int tmp = a;
        a = b;
        b = tmp;
    }
    int diff = depth[a] - depth[b];
    for (int k = LOG - 1; k >= 0; --k) {
        if ((diff >> k) & 1) {
            a = up[k][a];
        }
    }
    if (a == b) {
        return a;
    }
    for (int k = LOG - 1; k >= 0; --k) {
        if (up[k][a] && up[k][a] != up[k][b]) {
            a = up[k][a];
            b = up[k][b];
        }
    }
    return up[0][a];
}
