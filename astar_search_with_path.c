#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Node {
    int x, y;
    int g, h;
    struct Node* parent;  // Parent pointer to trace the path
} Node;

int compare_nodes(const void *a, const void *b) {
    Node *node_a = (Node *)a;
    Node *node_b = (Node *)b;
    return (node_a->g + node_a->h) - (node_b->g + node_b->h);
}

int heuristic(int x1, int y1, int x2, int y2) {
    // Manhattan distance as heuristic
    return abs(x1 - x2) + abs(y1 - y2);
}

Node* create_node(int x, int y, int g, int h, Node* parent) {
    Node *new_node = malloc(sizeof(Node));
    new_node->x = x;
    new_node->y = y;
    new_node->g = g;
    new_node->h = h;
    new_node->parent = parent;
    return new_node;
}

int astar_search_path(const int *grid, int n, int start_x, int start_y, int end_x, int end_y, int **path_out, int *path_length) {
    Node **open_set = malloc(n * n * sizeof(Node *));
    int open_set_size = 0;
    int *closed_set = calloc(n * n, sizeof(int));  // Closed set to mark visited nodes
    Node *start = create_node(start_x, start_y, 0, heuristic(start_x, start_y, end_x, end_y), NULL);

    open_set[open_set_size++] = start;

    while (open_set_size > 0) {
        qsort(open_set, open_set_size, sizeof(Node *), compare_nodes);
        Node *current = open_set[0];
        memmove(&open_set[0], &open_set[1], (open_set_size - 1) * sizeof(Node *));
        open_set_size--;

        if (current->x == end_x && current->y == end_y) {
            // Trace path back to start
            Node *path_node = current;
            *path_length = 0;
            while (path_node != NULL) {
                (*path_length)++;
                path_node = path_node->parent;
            }

            *path_out = malloc(*path_length * 2 * sizeof(int));
            path_node = current;
            for (int i = *path_length - 1; i >= 0; i--) {
                (*path_out)[2 * i] = path_node->x;
                (*path_out)[2 * i + 1] = path_node->y;
                path_node = path_node->parent;
            }

            for (int i = 0; i < open_set_size; i++) {
                free(open_set[i]);
            }
            free(open_set);
            free(closed_set);
            return current->g;  // Return path length as cost
        }

        int neighbors[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        for (int i = 0; i < 4; i++) {
            int nx = current->x + neighbors[i][0];
            int ny = current->y + neighbors[i][1];
            if (nx >= 0 && ny >= 0 && nx < n && ny < n && grid[nx * n + ny] == 0 && !closed_set[nx * n + ny]) {
                Node *neighbor = create_node(nx, ny, current->g + 1, heuristic(nx, ny, end_x, end_y), current);
                open_set[open_set_size++] = neighbor;
                closed_set[nx * n + ny] = 1;
            }
        }
    }

    for (int i = 0; i < open_set_size; i++) {
        free(open_set[i]);
    }
    free(open_set);
    free(closed_set);
    *path_length = 0;
    return -1;  // return -1 if no path is found
}
