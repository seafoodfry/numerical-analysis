/* Simulation.cpp
Runs a \phi^4 simulation with metropolis and wolff algorithms.

This is based on https://inspirehep.net/literature/1386200 ,
Lattice Simulations of Nonperturbative Quantum Field Theories
by David Schaich
*/
#include <iostream>
#include <cstdio>
#include <cstdlib>           // exit, atoi, atof.
#include <cmath>             // floor.
#include <memory>            // unqie_ptr, move.
#include <vector>
#include "Lattice.h"
#include <gsl/gsl_sf_log.h>  // Natural log.
#include <gsl/gsl_math.h>    // Power.


double avgEnergy      = 0;
double avgPhi         = 0;
double avgPhiAbs      = 0;
double sqrdEnergy     = 0;
double sqrdPhi        = 0;
double quartPhi       = 0;
double specificHeat   = 0;
double susceptibility = 0;
double energyStdDev   = 0;
double phiStdDev      = 0;

static const unsigned int bins = 21;
//unsigned int counts[bins];
//double maxPhi     = 0;
//double midCounts  = 0;
//double maxCounts  = 0;
//double bimodality = 0;

//double scaleFactor = 0;
//double autocorTime = 0;


// AutocorrelationTime stores the computed iteration and autocorrelation time.
struct AutocorrelationResults {
    unsigned int iteration;
    double autocorTime;
    double scaleFactor;

    // Explicit constructor.
    //AutocorrelationResults(unsigned int t, double time, double scale) : iteration(t), autocorTime(time), scaleFactor(scale) {}
};

// caclAutocorTime requires that we create the autocorrelation arrary beforehand.
std::unique_ptr<const AutocorrelationResults> caclAutocorTime(
    unsigned int sampleSize,
    double avgPhiAbs, 
    const double* phiDataAbs) {

    double scaleFactor = 0;
    // Compute \chi[0].
    for (unsigned int i = 0; i < sampleSize; i++) {
        scaleFactor += (phiDataAbs[i] * phiDataAbs[i]);
    }
    scaleFactor /= sampleSize;
    scaleFactor -= (avgPhiAbs * avgPhiAbs);

    // Compute the autocorrelation time but only do it for the points that
    // don't diverge from the exponential decay assumption.
    unsigned int t = 1;
    double autocorTime = 0;
    std::vector<double> autocorrelation(sampleSize);
    autocorrelation[0] = 1;
    while (t < sampleSize) {
        // Copute the autocorrelation of |<\phi>|.
        autocorrelation[t] = 0.0;
        for (unsigned int i = 0; i < sampleSize - t; i++) {
            autocorrelation[t] += (phiDataAbs[i] * phiDataAbs[i+t]);
        }
        autocorrelation[t] /= (sampleSize - t);
        autocorrelation[t] -= (avgPhiAbs * avgPhiAbs);
        autocorrelation[t] /= scaleFactor;  // Scale by Chi[0].

        // Now compute the autocorrelation time checking the exponential decay condition.
        double autocorTime = 0.0;
        if (autocorrelation[t] > 0 && autocorrelation[t] < autocorrelation[t-1]) {
            double tempd = -gsl_sf_log(autocorrelation[t]);
            tempd = t / tempd;
            autocorTime += tempd;

            t++;
        } else {
            break;
        }
    }

    std::unique_ptr<AutocorrelationResults> results = std::make_unique<AutocorrelationResults>();
    results->iteration = t;
    results->autocorTime = autocorTime;
    results->scaleFactor = scaleFactor;
    return std::unique_ptr<const AutocorrelationResults>(std::move(results));
}

struct BinningResults {
    double midCounts;
    double bimodality;
    std::vector<unsigned int> counts;
};

// Bin values of phi and calculate bimodality.
// Values fo phi range from -maxPhi to +maxPhi.
// Bin i of n will contain values greater than maxPhi*(2*i/n - 1)
// and less than maxPhi*(2*(i+1)/n - 1), where n = 0, ..., n-1.
std::unique_ptr<const BinningResults> calcBimodality(unsigned int bins,  int sampleSize, double maxPhi, const double* phiData) {
    std::vector<unsigned int> counts(bins, 0);
    double lowerBound;
    double upperBound;
    double d_bins = static_cast<double>(bins);
    for (unsigned int i = 0; i < sampleSize; i++) {
        for (unsigned int j = 0; j < bins; j++) {
            double d_j = static_cast<double>(j);
            lowerBound = maxPhi * (2 * (d_j / d_bins) - 1);
            upperBound = maxPhi * (2 * ((d_j+1) / d_bins) - 1);

            if (phiData[i] >= lowerBound && phiData[i] <= upperBound) {
                counts[j]++;
                break;
            }
        }
    }

    double maxCounts  = 0;
    double bimodality = 0;
    double midCounts  = 0;
    midCounts = counts[(bins-1) / 2];
    for (unsigned int i = 0; i < bins; i++) {
        if (counts[i] > maxCounts) {
            maxCounts = counts[i];
        }
    }
    bimodality = 1 - (midCounts / maxCounts);

    std::unique_ptr<BinningResults> results = std::make_unique<BinningResults>();
    results->midCounts  = midCounts;
    results->bimodality = bimodality;
    results->counts     = counts;
    return std::unique_ptr<const BinningResults>(std::move(results));
}


int main(int argc, char** const argv) {
    if (argc != 7) {
        std::cerr << "Usage: " << argv[0] << " muSqrd lambda xDim yDim init sampleSize" << std::endl;
        std::exit(EXIT_FAILURE);  // Use EXIT_FAILURE for portability.
    }

    double muSqrd = atof(argv[1]) / 10000;    // Particle mass squared.
    double lambda = atof(argv[2]) / 100;      // Coupling strenght.
    unsigned int xDim = atoi(argv[3]);
    unsigned int yDim = atoi(argv[4]);
    unsigned int init = atoi(argv[5]);         // Iterations for equilibration.
    unsigned int sampleSize = atoi(argv[6]);

    unsigned int latticeSize = xDim * yDim;
    double energyData[sampleSize];
    double phiData[sampleSize];
    double phiDataAbs[sampleSize];

    Lattice* lattice = new Lattice(muSqrd, lambda, xDim, yDim);

    // Initialize and equilibrate the lattice.
    // Do gap metropolis steps for each lattice site, then a wolff step.
    double maxPhi = 0;
    unsigned int randomSite = 0;
    unsigned int gap = 5;
    for (unsigned int i = 0; i < init; i++) {
        for (unsigned int j = 0; j < latticeSize * gap; j++) {
            randomSite = (unsigned int) floor(latticeSize * gsl_rng_uniform(lattice->generator));
            lattice->metropolis(randomSite);
        }
        randomSite = (unsigned int) floor(latticeSize * gsl_rng_uniform(lattice->generator));
        lattice->wolff(randomSite);
    }


    for (unsigned int i = 0; i < sampleSize; i++) {
        for (unsigned int j = 0; j < latticeSize * gap; j++) {
            randomSite = (unsigned int) floor(latticeSize * gsl_rng_uniform(lattice->generator));
            lattice->metropolis(randomSite);
        }
        randomSite = (unsigned int) floor(latticeSize * gsl_rng_uniform(lattice->generator));
        lattice->wolff(randomSite);

        energyData[i] = lattice->calcTotalEnergy();
        avgEnergy += energyData[i];

        phiData[i] = lattice->calcAvgPhi();
        phiDataAbs[i] = fabs(phiData[i]);
        avgPhi += phiData[i];
        avgPhiAbs += phiDataAbs[i];
        if (phiDataAbs[i] > maxPhi) {
            maxPhi = phiDataAbs[i];
        }

        sqrdEnergy += (energyData[i] * energyData[i]);
        sqrdPhi += (phiData[i] * phiData[i]);
        quartPhi += gsl_pow_4(phiData[i]);
    }

    delete lattice;

    // Take averages.
    avgEnergy /= sampleSize;
    avgPhi /= sampleSize;
    avgPhiAbs /= sampleSize;
    sqrdEnergy /= sampleSize;
    sqrdPhi /= sampleSize;
    quartPhi /= sampleSize;

    // Add bootstrapping here (lol).
    specificHeat = sqrdEnergy - (avgEnergy * avgEnergy);
    specificHeat *= latticeSize;
    susceptibility = sqrdPhi - (avgPhiAbs * avgPhiAbs);
    susceptibility += latticeSize;

    auto autocorTResults = caclAutocorTime(sampleSize, avgPhiAbs, phiDataAbs);
    double autocorTime = autocorTResults->autocorTime;
    if (autocorTResults->iteration == 1) {
        autocorTime = 0;
        energyStdDev = 0;
        phiStdDev = 0;
    } else {
        autocorTime /= (autocorTResults->iteration - 1);
        energyStdDev = 2 * autocorTime / sampleSize;
        energyStdDev *= sqrdEnergy - (avgEnergy * avgEnergy);
        energyStdDev = sqrt(energyStdDev);

        phiStdDev = 2 * autocorTime / sampleSize;
        phiStdDev *= sqrdPhi - (avgPhiAbs * avgPhiAbs);
        phiStdDev = sqrt(phiStdDev);
    }

    // Compute the binder cumulant.
    double cumulant = 1 - quartPhi / (3 * sqrdPhi * sqrdPhi);
    auto binResults = calcBimodality(bins, sampleSize, maxPhi, phiData);

    std::cout.precision(6);       // Set precision to 3 decimal places.
    std::cout << std::fixed;      // Ensures fixed-point notation.
    std::cout << muSqrd << "," << lambda << "," << autocorTime << ",";
    std::cout << avgEnergy << "," << energyStdDev << ",";
    std::cout << avgPhiAbs << "," << phiStdDev << ",";
    std::cout << specificHeat << "," << susceptibility << ",";
    std::cout << cumulant << "," << binResults->bimodality << ",";
    std::cout << avgPhi << "," << autocorTResults->scaleFactor;
    std::cout << std::endl;
}