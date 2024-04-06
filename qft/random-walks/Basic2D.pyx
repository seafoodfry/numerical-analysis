# cython: language_level=3

# distutils: libraries = gsl gslcblas
# distutils: include_dirs = /usr/local/include
# distutils: library_dirs = /usr/local/lib

from libc.stdio cimport FILE, fopen, fprintf, fclose
from libc.stdlib cimport malloc, free

cdef extern from "gsl/gsl_rng.h":
    ctypedef struct gsl_rng_type:
        pass
    gsl_rng_type* gsl_rng_mt19937
    ctypedef struct gsl_rng:
        pass
    gsl_rng* gsl_rng_alloc(gsl_rng_type* T)
    void gsl_rng_free(gsl_rng* r)
    unsigned long int gsl_rng_get(gsl_rng* r)

cpdef run_random_walks():
    cdef int sampleSize = 100000
    cdef int minLength = 1
    cdef int maxLength = 50
    cdef int x = 0, y = 0
    cdef int stepLength = 1
    cdef int direction = 4
    cdef double dist = 0, distTotal = 0, distAvg = 0
    cdef int totalSteps, test, step  # Declare loop variables here
    
    cdef gsl_rng* generator = gsl_rng_alloc(gsl_rng_mt19937)
    
    # Open file
    cdef FILE* output = fopen(b"Basic2D-cytonic.csv", "w")
    if not output:
        raise ValueError("Could not open file for writing.")
    
    # Main logic
    # Note: Cython loop syntax follows Python syntax closely.
    for totalSteps in range(minLength, maxLength + 1):
        distTotal = 0
        for test in range(sampleSize):
            x = y = 0
            for step in range(totalSteps):
                direction = gsl_rng_get(generator) % 4
                if direction == 0: x += stepLength
                elif direction == 1: y += stepLength
                elif direction == 2: x -= stepLength
                else: y -= stepLength
            dist = x * x + y * y
            distTotal += dist
        
        distAvg = distTotal / sampleSize
        fprintf(output, b"%i,\t%.3lf\n", totalSteps, distAvg)
    
    gsl_rng_free(generator)
    fclose(output)
