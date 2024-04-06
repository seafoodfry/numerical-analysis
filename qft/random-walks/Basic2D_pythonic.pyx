# cython: language_level=3

import numpy as np

cpdef run_random_walks_pythonic():
    cdef int sampleSize = 100
    cdef int minLength = 1
    cdef int maxLength = 50
    cdef int x, y
    cdef int stepLength = 1
    cdef int totalSteps, test, step
    cdef double dist, distTotal, distAvg

    for totalSteps in range(minLength, maxLength + 1):
        distTotal = 0
        for test in range(sampleSize):
            x = y = 0        
            directions = np.random.randint(0, 4, size=totalSteps)

            # Do the entire random walk.
            for step in range(totalSteps):
                # Directly use directions with Pythonic indexing
                direction = directions[step]
                if direction == 0: x += stepLength
                elif direction == 1: y += stepLength
                elif direction == 2: x -= stepLength
                else: y -= stepLength

            # Now that the walk is over, let's see how far we got.
            dist = x * x + y * y
            distTotal += dist

        distAvg = distTotal / sampleSize
        with open("Basic2D_pythonic.csv", "a") as output:
            output.write(f"{totalSteps},\t{distAvg:.3f}\n")