#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int x, y;
    int g, h;
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

int astar_search(const int *grid, int n, int start_x, int start_y, int end_x, int end_y) {
    Node *open_set = malloc(n * n * sizeof(Node));
    int open_set_size = 0;
    int *closed_set = calloc(n * n, sizeof(int));  // Closed set to mark visited nodes
    Node start = {start_x, start_y, 0, heuristic(start_x, start_y, end_x, end_y)};

    open_set[open_set_size++] = start;

    while (open_set_size > 0) {
        qsort(open_set, open_set_size, sizeof(Node), compare_nodes);
        Node current = open_set[0];
        memmove(&open_set[0], &open_set[1], (open_set_size - 1) * sizeof(Node));
        open_set_size--;

        if (current.x == end_x && current.y == end_y) {
            free(open_set);
            free(closed_set);
            return current.g;
        }

        int neighbors[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};
        for (int i = 0; i < 4; i++) {
            int nx = current.x + neighbors[i][0];
            int ny = current.y + neighbors[i][1];
            if (nx >= 0 && ny >= 0 && nx < n && ny < n && grid[nx * n + ny] == 0 && !closed_set[nx * n + ny]) {
                Node neighbor = {nx, ny, current.g + 1, heuristic(nx, ny, end_x, end_y)};
                open_set[open_set_size++] = neighbor;
                closed_set[nx * n + ny] = 1;  // Mark as visited
            }
        }
    }
    free(open_set);
    free(closed_set);
    return -1;  // return -1 if no path is found
}


// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <math.h>

// typedef struct Node {
//     int x, y;
//     int g, h;
//     struct Node *parent; // Pointer to parent node for path tracking
// } Node;

// int compare_nodes(const void *a, const void *b) {
//     Node *node_a = (Node *)a;
//     Node *node_b = (Node *)b;
//     return (node_a->g + node_a->h) - (node_b->g + node_b->h);
// }

// int heuristic(int x1, int y1, int x2, int y2) {
//     // Manhattan distance as heuristic
//     return abs(x1 - x2) + abs(y1 - y2);
// }

// Node *create_node(int x, int y, int g, int h, Node *parent) {
//     Node *new_node = malloc(sizeof(Node));
//     if (new_node) {
//         new_node->x = x;
//         new_node->y = y;
//         new_node->g = g;
//         new_node->h = h;
//         new_node->parent = parent;
//     }
//     return new_node;
// }

// char **generate_directions(Node *path, int path_length) {
//     char **directions = malloc((path_length - 1) * sizeof(char *));
//     for (int i = 0; i < path_length - 1; i++) {
//         int dx = path[i + 1].x - path[i].x;
//         int dy = path[i + 1].y - path[i].y;

//         if (dx == 1 && dy == 0) {
//             directions[i] = strdup("SOUTH");
//         } else if (dx == -1 && dy == 0) {
//             directions[i] = strdup("NORTH");
//         } else if (dy == 1 && dx == 0) {
//             directions[i] = strdup("EAST");
//         } else if (dy == -1 && dx == 0) {
//             directions[i] = strdup("WEST");
//         }
//     }
//     return directions;
// }

// int astar_search(const int *grid, int n, int start_x, int start_y, int end_x, int end_y, Node **path, char ***directions) {
//     Node **open_set = malloc(n * n * sizeof(Node *));
//     int open_set_size = 0;
//     int *closed_set = calloc(n * n, sizeof(int));
//     Node *start = create_node(start_x, start_y, 0, heuristic(start_x, start_y, end_x, end_y), NULL);

//     open_set[open_set_size++] = start;

//     while (open_set_size > 0) {
//         qsort(open_set, open_set_size, sizeof(Node *), compare_nodes);
//         Node *current = open_set[0];
//         memmove(&open_set[0], &open_set[1], (open_set_size - 1) * sizeof(Node *));
//         open_set_size--;

//         if (current->x == end_x && current->y == end_y) {
//             // Build the path by tracing parent pointers
//             Node *step = current;
//             int path_length = 0;
//             while (step) {
//                 path_length++;
//                 step = step->parent;
//             }

//             *path = malloc(path_length * sizeof(Node));
//             step = current;
//             for (int i = path_length - 1; i >= 0; i--) {
//                 (*path)[i] = *step;
//                 step = step->parent;
//             }

//             // Generate directions
//             *directions = generate_directions(*path, path_length);

//             // Cleanup
//             for (int i = 0; i < open_set_size; i++) {
//                 free(open_set[i]);
//             }
//             free(open_set);
//             free(closed_set);
//             return path_length;
//         }

//         int neighbors[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};
//         for (int i = 0; i < 4; i++) {
//             int nx = current->x + neighbors[i][0];
//             int ny = current->y + neighbors[i][1];
//             if (nx >= 0 && ny >= 0 && nx < n && ny < n && grid[nx * n + ny] == 0 && !closed_set[nx * n + ny]) {
//                 Node *neighbor = create_node(nx, ny, current->g + 1, heuristic(nx, ny, end_x, end_y), current);
//                 open_set[open_set_size++] = neighbor;
//                 closed_set[nx * n + ny] = 1;
//             }
//         }
//     }

//     for (int i = 0; i < open_set_size; i++) {
//         free(open_set[i]);
//     }
//     free(open_set);
//     free(closed_set);
//     return -1;
// }

