/* Lattice.h
Implements a lattice of spins for an Ising model simulation.

This is based on https://inspirehep.net/literature/1386200 ,
Lattice Simulations of Nonperturbative Quantum Field Theories
by David Schaich
*/
#ifndef _LATTICE_H
#define _LATTICE_H

#include "HashTable.h"
#include <vector>
#include <filesystem>        // filesystem::path.
#include <gsl/gsl_rng.h>     // Random number generators.
#include <gsl/gsl_sf_exp.h>  // Exponential functions.


class Lattice {
    public:
    // Data.
    std::vector<int> lattice;  // Values must be 1, -1.
    unsigned int xDim;         // x dimension of the lattice.
    unsigned int yDim;         // y dimension of the lattice.
    unsigned int latticeSize;  // Number of sites in the lattice

    float temp;          // kT in energy units (k=1).
    float beta;          // 1/kT in energy units (k=1).
    double randomU;      // Random number in range [0,1).
    double probability;  // Probability of flipping or adding to a cluster.
                         // SHOULD NOT change for wolff.
    
    double exponentials[2];
    double totalEnergy;

    // Neighboring lattice sites.
    unsigned int nextX;
    unsigned int prevX;
    unsigned int nextY;
    unsigned int prevY;

    HashTable* cluster;
    gsl_rng* generator;

    
    // Methods.
    Lattice(unsigned int x, unsigned int y, unsigned int RNSeed);
    Lattice();
    ~Lattice();

    void printLattice();
    void saveLatticeToFile(const std::filesystem::path& dirPath, const std::string& filename);
    void printCluster();

    // Periodicboundary conditions.
    void getHalfNeighbours(unsigned int site);
    void getNeighbours(unsigned int site);

    int calcHalfenergy(unsigned int site);
    int calcEnergy(unsigned int site);
    double calcTotalEnergy();
    double calcMagnetization();
    double calcSpecificHeat(double avgEnergy, double squared);
    double calcSusceptibility(double avgMagnet, double sqrdMagnet);

    bool metropolis(unsigned int site);  // Returns whether or not the site flipped.
    void growCluster(unsigned int site, int spin);
    void flipCluster();
    void flipComplement();
    unsigned int wolff(unsigned int site);

    private:
    // Making this to avoid bugs where we confuse a '*' for a '+' or any other sort of operator.
    unsigned int flipped(unsigned int site);
};

#endif // _LATTICE_H