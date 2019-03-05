from cython.parallel import parallel, prange
from libc.stdlib cimport abort, malloc, free

cdef Py_ssize_t idx, i, n = 100
cdef str * local_buf
cdef size_t size = 10
cdef str meg = "algo"

with nogil, parallel():
    local_buf = <int *> malloc(sizeof(int) * size)
    if local_buf is NULL:
        abort()

    # populate our local buffer in a sequential loop
    for i in xrange(size):
        local_buf[i] = meg 

    # share the work using the thread-local buffer(s)
    for i in prange(n, schedule='guided'):
        i += 1

    free(local_buf)