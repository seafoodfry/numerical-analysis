/* Basic2D.cpp simulates a 2D random walk

This is based on https://inspirehep.net/literature/1386200 ,
Lattice Simulations of Nonperturbative Quantum Field Theories
by David Schaich

Basic random walk in 2D.
Creates a file with positions over time.
The file is called Basic2D.csv.
*/
#include <iostream>
#include <gsl/gsl_rng.h>

using namespace std;


int main(int argc, char* argv[]) {
    FILE* output;
    output = fopen("Basic2D.csv", "w");

    int sampleSize = 100000; // 100_000.
    int minLength  = 1;
    int maxLength  = 50;

    int x = 0;
    int y = 0;

    int stepLength = 1;
    int direction  = 4; // To be computed randomly.

    double dist      = 0;
    double distTotal = 0;
    double distAvg   = 0;

    // Mersenne Twister generator.
    gsl_rng* generator = gsl_rng_alloc(gsl_rng_mt19937);

    // We will perform maxLength number of experiments.
    // During each experiment, we will perform a sampleSize of random walks.
    // After each sampleSize of random walks for a give number ot totalSteps,
    // we will computer the avegraged squared distance traveled.
    for (int totalSteps = minLength; totalSteps <= maxLength; totalSteps++) {
        distTotal = 0;

        // Perform sampleSize experiments of the entire random walk.
        // Each experiment allows the walker to take totalSteps.
        // The number of totalSteps is progressively increased from
        // minLength to maxLength.
        // After each random walk, compute the squared distance and add it to
        // distTotal.
        for (int test = 0; test < sampleSize; test++) {
            x = y = 0;

            // Chose 1 of the possible 4 directions in a 2D plane.
            // Take a step in that direction.
            // Do that for a total of totalSteps.
            // Remember that the range of totalSteps is from minLength to maxLength.
            for (int step = 0; step < totalSteps; step++) {
                direction = gsl_rng_get(generator) % 4;
                if      (direction == 0) x += stepLength;
                else if (direction == 1) y += stepLength;
                else if (direction == 2) x -= stepLength;
                else                     y -= stepLength;
            }

            // Compute the distance squared for each test.
            dist = x*x + y*y;
            distTotal += dist;
        }

        // Compute the average distance and write it.
        distAvg = distTotal / sampleSize;
        cout << totalSteps << " " << distAvg << endl;
        fprintf(output, "%i,\t%.3lf\n", totalSteps, distAvg);
    }

    gsl_rng_free(generator);
    fclose(output);
    return 0;
}