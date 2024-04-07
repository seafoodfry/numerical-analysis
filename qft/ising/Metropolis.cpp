/* metropolis.cpp
Runs a MCMC simulation for a 2D Ising model.
Writes energy, magnetization, specific heat, and susceptibility.

This is based on https://inspirehep.net/literature/1386200 ,
Lattice Simulations of Nonperturbative Quantum Field Theories
by David Schaich
*/
#include <cmath>             // floor, sqrt.
#include <cstdio>            // printf.
#include <cstdlib>           // atoi.
#include <gsl/gsl_sf_log.h>  // Natural log.
#include "Lattice.h"

int main(unsigned int argc, char** const argv) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s xDim yDim init sampleSize temp\n", argv[0]);
        fflush(stderr);
        exit(1);
    }

    unsigned int xDim = atoi(argv[1]);        // Lattice size in the x dimension.
    unsigned int yDim = atoi(argv[2]);        // Lattice size in the y dimention.
    unsigned int init = atoi(argv[3]);        // Number of equilibrium steps.
    unsigned int sampleSize = atoi(argv[4]);  // Number of iterations for statistics.
    unsigned int RNSeed = atoi(argv[5]);      // 100x temperature (kT but in natural units).

    float temp = (float)RNSeed / 100;

    unsigned int randomSite;
    unsigned int latticeSize = xDim * yDim;

    double avgEnergy = 0.0;
    double avgMagnet = 0.0;
    double AvgMagnetAbs = 0.0;
    double sqrEnergy = 0.0;
    double sqrMagnet = 0.0;
    double specificHeat = 0.0;
    double susceptibility = 0.0;

    double energyData[sampleSize];
    double magnetData[sampleSize];
    double autocorrelation[sampleSize];
    double scaleFactor = 0.0;
    double autocorTime = 0.0;
    double energyStdDev = 0.0;
    double magnetStdDev = 0.0;

    Lattice* lattice = new Lattice(xDim, xDim, RNSeed);

    // Initialize and equilibrate the lattice.
    for (unsigned int i = 0; i < init*latticeSize; i++) {
        randomSite = (int) floor(latticeSize * gsl_rng_uniform(lattice->generator));
        lattice->metropolis(randomSite);
    }
}