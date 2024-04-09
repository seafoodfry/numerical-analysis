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

    // Take data every 5 sweeps (somewhat arbitrary value based on checking out the 
    // autocorrelation times).
    // TODO: elaborate on what and why.
    unsigned int counter = 0;
    for (unsigned int i = 0; i < sampleSize * latticeSize * 5; i++) {
        randomSite = (int) floor(latticeSize * gsl_rng_uniform(lattice->generator));
        lattice->metropolis(randomSite);

        // TODO: this if can be simplified to just i % 5 = 0.
        if (i % (latticeSize*5) == 0) {
            energyData[counter] = lattice->calcTotalEnergy();
            avgEnergy += energyData[counter];

            magnetData[counter] = lattice->calcMagnetization();
            avgMagnet += magnetData[counter];
            magnetData[counter] = fabs(magnetData[counter]);
            AvgMagnetAbs += magnetData[counter];

            sqrEnergy += (energyData[counter] * energyData[counter]);
            sqrMagnet += (magnetData[counter] * magnetData[counter]);

            counter++
        }
    }

    // Take averages.
    avgEnergy /= sampleSize;
    avgMagnet /= sampleSize;
    AvgMagnetAbs /= sampleSize;
    sqrEnergy /= sampleSize;
    sqrMagnet /= sampleSize;

    // Add bootstrapping here...
    // TODO: the author's comment was literal.
    specificHeat = lattice->calcSpecificHeat(avgEnergy, sqrEnergy);
    susceptibility = lattice->calcSusceptibility(AvgMagnetAbs, sqrMagnet);

    delete lattice;

    // Now its time for some autocorrelation and standard deviation madness.
    // Use magnetization for autocorrelation time calculation.
    // Should be roughly the same for all variables.
    // Generate Chi[0] for scaling purposes.
    for (unsigned int i = 0; i < sampleSize; i++) {
        scaleFactor += (magnetData[i] * magnetData[i]);
    }
    scaleFactor /= sampleSize;
    scaleFactor -= (AvgMagnetAbs * AvgMagnetAbs);

    autocorrelation[0] = 1.0;

    // Calculate autocorrelation function, Chi[0], t > 0.
    for (unsigned int t = 1; t < sampleSize; t++) {
        autocorrelation[t] = 0.0;
        for (unsigned int i = 0; i < sampleSize - t; i++) {
            autocorrelation[t] += (magnetData[i] * magnetData[i+t]);
        }
        autocorrelation[t] /= (sampleSize - t);
        autocorrelation[t] -= (AvgMagnetAbs * AvgMagnetAbs);
        autocorrelation[t] /= scaleFactor;  // Scale by Chi[0].
    }

    // Generate autocorrelation times from autocorrelation function.
    double tempd;
    unsigned int i = 1;
    while (autocorrelation[i] > 0 && autocorrelation[i] < autocorrelation[i-1] && i < sampleSize) {
        tempd = -gsl_sf_log(autocorrelation[i]);
        tempd = i / tempd;
        autocorTime += tempd;

        i++;
    }

    // Use standard formula to generate standard deviations from autocorrelation time,
    // average, sampleSize, etc.
    if (i == 1) {  // Assume autocorrelation tiem is 0.
        autocorTime = 0.0;
        energyStdDev = 0.0;
        magnetStdDev = 0.0;
    } else {
        autocorTime /= (i - 1);
        energyStdDev = 2 * autocorTime / sampleSize;
        energyStdDev += sqrEnergy - (avgEnergy * avgEnergy);
        energyStdDev = sqrt(energyStdDev);

        magnetStdDev = 2 * autocorTime / sampleSize;
        magnetStdDev *= sqrMagnet - (AvgMagnetAbs * AvgMagnetAbs);
        magnetStdDev = sqrt(magnetStdDev);
    }

    printf("%f\t%lf\t", temp, autocorTime);
    printf("%lf\t%lf\t", avgEnergy, energyStdDev);
    printf("%lf\t%lf\t", AvgMagnetAbs, magnetStdDev);
    printf("%lf\t%lf\t", specificHeat, susceptibility);
    printf("%lf\t%lf\n", avgMagnet, scaleFactor);

    return 0;
}