import numpy as np
from cffi import FFI

ffi = FFI()

# ffi.cdef("""
# int astar_search(const int *grid, int n, int start_x, int start_y, int end_x, int end_y, int **path_out, int *path_length);
# """)

ffi.cdef("""
int astar_search(const int *grid, int n, int start_x, int start_y, int end_x, int end_y, int **path_out, int *path_length);
void free(void *ptr);
""")

C = ffi.dlopen("./libastarsearchpath.so")

def astar_path(grid, start, end):
    n = grid.shape[0]
    grid_ptr = ffi.cast("int *", grid.ctypes.data)

    # Prepare pointers for path output and path length
    path_out_ptr = ffi.new("int **")
    path_length_ptr = ffi.new("int *")

    # Call the C function
    result = C.astar_search(grid_ptr, n, start[0], start[1], end[0], end[1], path_out_ptr, path_length_ptr)

    # Extract the path from the C data structure
    if result != -1 and path_length_ptr[0] > 0:
        path_length = path_length_ptr[0]
        path_array = np.frombuffer(ffi.buffer(path_out_ptr[0], path_length * 2 * ffi.sizeof("int")), dtype=np.int32)
        path = path_array.reshape(path_length, 2)
        C.free(path_out_ptr[0])  # Corrected memory free call
        return path.tolist(), result
    else:
        return None, result


if __name__ == "__main__":
    grid = np.array([
        [0, 0, 0, 1, 0],
        [1, 1, 0, 1, 0],
        [0, 0, 0, 1, 0],
        [0, 1, 0, 0, 0],
        [0, 0, 0, 1, 1]
    ], dtype=np.int32)
    start = (0, 0)
    end = (2, 0)
    path = astar_path(grid, start, end)
    if path:
        print("Path found:", path)
