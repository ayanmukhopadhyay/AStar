from queue import PriorityQueue
import time
import random
from heapq import heappop, heappush
import numpy as np
from cffi import FFI

ffi = FFI()

ffi.cdef("""
int astar_search(const int *grid, int n, int start_x, int start_y, int end_x, int end_y);
""")

C = ffi.dlopen("./libastarsearch.so")


def astar_path_C(grid, start, end):
    n = grid.shape[0]
    grid_ptr = ffi.cast("int *", grid.ctypes.data)
    return C.astar_search(grid_ptr, n, *start, *end)


def a_star_search_old(grid, start, goal):
    def get_neighbors(pos):
        neighbors = []
        directions = [(0, -1), (0, 1), (-1, 0), (1, 0)]  # Left, Right, Up, Down
        for d in directions:
            neighbor = (pos[0] + d[0], pos[1] + d[1])
            if 0 <= neighbor[0] < len(grid) and 0 <= neighbor[1] < len(grid[0]) and not grid[neighbor[0]][neighbor[1]]:
                neighbors.append(neighbor)
        return neighbors

    def heuristic(a, b):
        return abs(a[0] - b[0]) + abs(a[1] - b[1])

    start = tuple(start)
    goal = tuple(goal)
    frontier = PriorityQueue()
    frontier.put((0, start))
    came_from = {}
    cost_so_far = {}
    came_from[start] = None
    cost_so_far[start] = 0

    while not frontier.empty():
        current = frontier.get()[1]

        if current == goal:
            break

        for next in get_neighbors(current):
            new_cost = cost_so_far[current] + 1
            if next not in cost_so_far or new_cost < cost_so_far[next]:
                cost_so_far[next] = new_cost
                priority = new_cost + heuristic(goal, next)
                frontier.put((priority, next))
                came_from[next] = current

    # Reconstruct path
    current = goal
    path = []
    while current != start:
        path.append(current)
        current = came_from[current]
    path.append(start)
    path.reverse()

    # Determine actions based on path
    actions = []
    for i in range(1, len(path)):
        prev, curr = path[i - 1], path[i]
        if curr[0] - prev[0] == 1: actions.append(3)
        elif curr[0] - prev[0] == -1: actions.append(1)
        elif curr[1] - prev[1] == 1: actions.append(2)
        elif curr[1] - prev[1] == -1: actions.append(0)

    return path, actions


def get_neighbors(grid):
    neighbors = {}
    directions = [(0, -1), (0, 1), (-1, 0), (1, 0)]  # Left, Right, Up, Down
    for i in range(grid_size):
        for j in range(grid_size):
            pos = tuple([i, j])
            neighbors[pos] = []
            for d in directions:
                neighbor_temp = (pos[0] + d[0], pos[1] + d[1])
                if 0 <= neighbor_temp[0] < len(grid) and 0 <= neighbor_temp[1] < len(grid[0]) and not grid[neighbor_temp[0]][neighbor_temp[1]]:
                    neighbors[pos].append(neighbor_temp)
    return neighbors


def a_star_search_new(grid, start, goal, neighbors):

    def heuristic(a, b):
        return abs(a[0] - b[0]) + abs(a[1] - b[1])

    start = tuple(start)
    goal = tuple(goal)
    frontier = PriorityQueue()
    frontier.put((0, start))
    came_from = {}
    cost_so_far = {}
    came_from[start] = None
    cost_so_far[start] = 0

    while not frontier.empty():
        current = frontier.get()[1]

        if current == goal:
            return cost_so_far[current]

        for next in neighbors[current]:
            new_cost = cost_so_far[current] + 1
            if next not in cost_so_far or new_cost < cost_so_far[next]:
                cost_so_far[next] = new_cost
                priority = new_cost + heuristic(goal, next)
                frontier.put((priority, next))
                came_from[next] = current

    print("Failure")
    return None


def a_star_search_heap(grid, start, goal, neighbors):

    def heuristic(a, b):
        return abs(a[0] - b[0]) + abs(a[1] - b[1])

    start = tuple(start)
    goal = tuple(goal)
    frontier = [[0, start]]
    came_from = {}
    cost_so_far = {}
    came_from[start] = None
    cost_so_far[start] = 0

    while frontier:
        value, key = heappop(frontier)
        current = key
        if current == goal:
            return cost_so_far[current]

        for next in neighbors[current]:
            new_cost = cost_so_far[current] + 1
            if next not in cost_so_far or new_cost < cost_so_far[next]:
                cost_so_far[next] = new_cost
                priority = new_cost + heuristic(goal, next)
                heappush(frontier, (-priority, next))
                came_from[next] = current

    print("Failure")
    return None


# Example usage:
num_obstacles = 300
grid_size = 400
# randomly choose 2000 obstacles
obstacles = [[random.randint(0, 99), random.randint(0, 99)] for i in range(0, num_obstacles)]

# Initialize grid and set obstacles
grid = [[0] * grid_size for _ in range(grid_size)]
for obs in obstacles:
    grid[obs[0]][obs[1]] = 1  # 1 represents an obstacle

# store to see that all results are the same
results_python = []
results_c = []



num_experiments = 50000
# create experimental config for all experiments:
starts = []
ends = []
for i in range(0, num_experiments):
    legal = False
    while not legal:
        start = (random.randint(0, 99), random.randint(0, 99))
        if grid[start[0]][start[1]] == 1:
            continue
        else:
            starts.append(start)
            break

    legal = False
    while not legal:
        end = (random.randint(0, 99), random.randint(0, 99))
        if grid[end[0]][end[1]] == 1:
            continue
        else:
            ends.append(end)
            break

print("Generated uniform start and end configurations")

"""
Try with Old A Star
"""
start_time = time.time()
for i in range(0, num_experiments):
    try:
        # start = (random.randint(0, 99), random.randint(0, 99))
        # goal = (random.randint(0, 99), random.randint(0, 99))
        start = starts[i]
        goal = ends[i]
        a_star_search_old(grid, start, goal)
    except KeyError:
        print("start:{}".format(start))
        print("goal:{}".format(goal))
        raise KeyError
end_time = time.time()
print(end_time-start_time)

"""
Try with A Star without backtracking and creating the graph once
"""

start_time = time.time()
neighbors = get_neighbors(grid)
for i in range(0, num_experiments):
    try:
        # start = (random.randint(0, 99), random.randint(0, 99))
        # goal = (random.randint(0, 99), random.randint(0, 99))
        start = starts[i]
        goal = ends[i]
        # results_python.append(a_star_search_new(grid, start, goal, neighbors))
    except KeyError:
        print("start:{}".format(start))
        print("goal:{}".format(goal))
        raise KeyError
end_time = time.time()
print(end_time-start_time)


"""
Try with A Star with heap
"""
start_time = time.time()
neighbors = get_neighbors(grid)
for i in range(0, num_experiments):
    try:
        # start = (random.randint(0, 99), random.randint(0, 99))
        # goal = (random.randint(0, 99), random.randint(0, 99))
        start = starts[i]
        goal = ends[i]
        a_star_search_new(grid, start, goal, neighbors)
    except KeyError:
        print("start:{}".format(start))
        print("goal:{}".format(goal))
        raise KeyError
end_time = time.time()
print(end_time-start_time)

"""
Okay, let us pull out the big gun!!
"""

start_time = time.time()
grid = np.array(grid, dtype=np.int32)
for i in range(0, num_experiments):
    try:
        # start = (random.randint(0, 99), random.randint(0, 99))
        # goal = (random.randint(0, 99), random.randint(0, 99))
        start = starts[i]
        goal = ends[i]
        # results_c.append(astar_path_C(grid, start, goal))
    except KeyError:
        print("start:{}".format(start))
        print("goal:{}".format(goal))
        raise KeyError
end_time = time.time()
print(end_time-start_time)

# if results_c == results_python:
#     print("Hurray!!")



