import numpy as np
from cffi import FFI

ffi = FFI()

ffi.cdef("""
int astar_search(const int *grid, int n, int start_x, int start_y, int end_x, int end_y);
""")

C = ffi.dlopen("./libastarsearch.so")


def astar_path_length(grid, start, end):
    n = grid.shape[0]
    grid_ptr = ffi.cast("int *", grid.ctypes.data)
    return C.astar_search(grid_ptr, n, *start, *end)


if __name__ == "__main__":
    grid = np.array([
        [1, 1, 1, 0, 1],
        [0, 0, 1, 0, 1],
        [1, 1, 1, 0, 1],
        [1, 0, 1, 1, 1],
        [1, 1, 1, 0, 1]
    ], dtype=np.int32)
    start = (0, 0)
    end = (4, 4)
    result = astar_path_length(grid, start, end)
    print(f"Length of shortest path: {result}")
