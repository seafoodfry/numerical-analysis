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
#include <fstream>
#include <iostream>          // cerr.
#include <filesystem>        // filesystem::path.
#include <gsl/gsl_sf_log.h>  // Natural log.
#include "Lattice.h"


void writeArrayToTextFile(const double* array, size_t size, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);;
        return;
    }

    for (size_t i = 0; i < size; ++i) {
        file << array[i] << "\n";
    }

    file.close();
}


int main(int argc, char** const argv) {
    if (argc != 10) {
        fprintf(stderr, "Usage: %s xDim yDim init sampleSize temp autocorrelation.txt dir-lattice-snaptshots snapshot-prefix 10\n", argv[0]);
        fflush(stderr);
        exit(1);
    }

    unsigned int xDim = atoi(argv[1]);           // Lattice size in the x dimension.
    unsigned int yDim = atoi(argv[2]);           // Lattice size in the y dimention.
    unsigned int init = atoi(argv[3]);           // Number of equilibrium steps.
    unsigned int sampleSize = atoi(argv[4]);     // Number of iterations for statistics.
    unsigned int RNSeed = atoi(argv[5]);         // 100x temperature (kT but in natural units).
    std::string autocorFile = argv[6];           // Name of text file in which to store the autocorrelation values.
    std::filesystem::path dirPath = argv[7];     // Path where to store snaptshots.
    std::string snapshotPrefix = argv[8];        // Prefix for the snaptshots.
    unsigned int snapFrequency = atoi(argv[9]);  // Frequency with which to store snapshots.

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
    

    Lattice* lattice = new Lattice(xDim, xDim, RNSeed);

    // Initialize and equilibrate the lattice.
    for (unsigned int i = 0; i < init*latticeSize; i++) {
        randomSite = (int) floor(latticeSize * gsl_rng_uniform(lattice->generator));
        lattice->metropolis(randomSite);

        if (snapFrequency > 0 && i%snapFrequency == 0) {
            std::string filename = snapshotPrefix + "-equil-" + std::to_string(i/latticeSize) + ".txt";
            lattice->saveLatticeToFile(dirPath, filename);
        }
    }

    // Take data every 5 sweeps (somewhat arbitrary value based on checking out the 
    // autocorrelation times).
    // TODO: elaborate on what and why. Evaluate how the critical slowing down is affected by this
    // parameter.
    unsigned int counter = 0;
    for (unsigned int i = 0; i < sampleSize * latticeSize * 5; i++) {
        randomSite = (int) floor(latticeSize * gsl_rng_uniform(lattice->generator));
        lattice->metropolis(randomSite);

        if (i % (latticeSize*5) == 0) {
            energyData[counter] = lattice->calcTotalEnergy();
            avgEnergy += energyData[counter];

            magnetData[counter] = lattice->calcMagnetization();
            avgMagnet += magnetData[counter];
            magnetData[counter] = fabs(magnetData[counter]);
            AvgMagnetAbs += magnetData[counter];

            sqrEnergy += (energyData[counter] * energyData[counter]);
            sqrMagnet += (magnetData[counter] * magnetData[counter]);

            counter++;
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
    // Generate Chi[0] (the scale factor) for scaling purposes.
    double scaleFactor = 0.0;
    for (unsigned int i = 0; i < sampleSize; i++) {
        scaleFactor += (magnetData[i] * magnetData[i]);
    }
    scaleFactor /= sampleSize;
    scaleFactor -= (AvgMagnetAbs * AvgMagnetAbs);


    // Calculate autocorrelation function, Chi[0], t > 0.
    double autocorrelation[sampleSize];
    autocorrelation[0] = 1.0;
    for (unsigned int t = 1; t < sampleSize; t++) {
        autocorrelation[t] = 0.0;
        for (unsigned int i = 0; i < sampleSize - t; i++) {
            autocorrelation[t] += (magnetData[i] * magnetData[i+t]);
        }
        autocorrelation[t] /= (sampleSize - t);
        autocorrelation[t] -= (AvgMagnetAbs * AvgMagnetAbs);
        autocorrelation[t] /= scaleFactor;  // Scale by Chi[0].
    }

    // Save the autocorrelation time series.
    writeArrayToTextFile(autocorrelation, sampleSize, autocorFile);


    // Generate autocorrelation times from autocorrelation function.
    double autocorTime = 0.0;
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
    double energyStdDev = 0.0;
    double magnetStdDev = 0.0;
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

    printf("%d,%d,%d,%d,%d,", xDim, yDim, init, sampleSize, RNSeed);
    printf("%f,%lf,", temp, autocorTime);
    printf("%lf,%lf,", avgEnergy, energyStdDev);
    printf("%lf,%lf,", AvgMagnetAbs, magnetStdDev);
    printf("%lf,%lf,", specificHeat, susceptibility);
    printf("%lf,%lf\n", avgMagnet, scaleFactor);

    return 0;
}