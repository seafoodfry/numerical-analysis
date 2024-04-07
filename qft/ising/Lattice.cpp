/* lattice.cpp
Implements a lattice of spins for an Ising model simulation.

This is based on https://inspirehep.net/literature/1386200 ,
Lattice Simulations of Nonperturbative Quantum Field Theories
by David Schaich
*/
#include "Lattice.h"
#include <cstdio>    // For fflush and stdout.


Lattice::Lattice(unsigned int x, unsigned int y, unsigned int RNSeed) {
    cluster = new HashTable();
    generator = gsl_rng_alloc(gsl_rng_mt19937);  // Mersenne twister.
    gsl_rng_set(generator, RNSeed);

    temp = (float)RNSeed / 100;
    beta = 1.0 / temp;
    xDim = x;
    yDim = y;
    latticeSize = x * y;

    // Note: the instantiation of the lattice vector was missing from the code in the thesis.
    // We added it.
    lattice = std::vector<int>(latticeSize, 1);

    // Give everyone a random initial state.
    // Note: since we modified the way the lattice vector is instantiated,
    // we only need to flip spins from the default value of 1.
    // This is also different from the code in the thesis.
    for (unsigned int i = 0; i < latticeSize; i++) {
        if (gsl_rng_uniform(generator) < 0.5) {
            lattice.push_back(-1);
        }
    }

    // Calculate all the exponentials now.
    /* For the Ising model in 2D, there are only 5 possible values for \Delta E:

    Remember that the Hamiltonina is H = - \sum_{ij} J_{ij} s_{ij} = -J \sum_{ij} s_i s_j
    when the J coupling is constant and the external magnetic field is 0.

    Start with an energy of -4J and all spins lined up:
                  u                       u
    E = -4J     u u u       -> E = 4J   u d u
                  u                       u
    \Delta E = 4J - -4J = 8J
    -\beta \Delta E = -beta 8J

                  u                       u
    E = -2J     d u u       -> E = 2J   d d u
                  u                       u
    \Delta E = 2J - -2J = 4J
    -\beta \Delta E = -beta 4J

                  u                       u
    E =  0J     d u u       -> E = 0J   d d u
                  d                       d
    \Delta E = 0J - 0J = 0J
    -\beta \Delta E = -beta 0J = 0

                  d                       d
    E = 2J      d u u      -> E = -2J   d d u
                  d                       d
    \Delta E = -2J - 2J = -4J
    -\beta \Delta E = beta 4J

                  d                       d
    E = 4J      d u d      -> E = -4J   d d d
                  d                       d
    \Delta E = -4J - 4J = -8J
    -\beta \Delta E = beta 8J

    So the change in energy is either 4J or 8J.
    */
    exponentials[0] = gsl_sf_exp(-beta * 4);
    exponentials[1] = gsl_sf_exp(-beta * 8);

    randomU = gsl_rng_uniform(generator);
    probability = 1 - gsl_sf_exp(-2 * beta);

    totalEnergy = calcTotalEnergy();
}

Lattice::Lattice() {
    Lattice(32, 32, 227);
}

Lattice::~Lattice() {}

void Lattice::printLattice() {
    for (unsigned int i = 0; i < latticeSize; i++) {
        if (i % xDim == 0)
            printf("\n");

        if (lattice[i] == -1)
            printf("o")
        else if (lattice[i] == 1)
            printf("x");
        else:
            printf("ERR");
    }
    printf("\n");
    fflush(stdout);
}

void Lattice::printCluster() {
    for (unsigned int i = 0; i < latticeSize; i++) {
        if (i % xDim == 0)
            printf("\n");

        if (cluster->find(i))
            printf("x")
        else:
            printf(" ");
    }
    printf("\n");
    fflush(stdout);
}

// getHalfNeighbours sets up helical boundary conditions.
// Remeber that the 2D lattice has been linearilized in order to be represented by a 1D array-like
// structure.
// We also try to avoid the use of the '%' (modulo) operation.
// Newman and Barkema (Chapter 13) recommend helical boundary conditions to
// reduce the number of multiplications and encourage the use of lattices of
// size L \propto 2^n, since in that case the mod function can be replaced by
// a much quicker bitwise AND operator.
/*
    Periodic boundary conditions:

        123 123
        456 456
        789 789

        123 123
        456 456
        789 789
    
    Helical boundary conditions:

        123 456   789
        456 789   123
        789 123   456

        123 456   789
        456 789   123
        789 123   456

    Here left and right neighbours of site n are n \pm 1 modulo the size of the lattice.
    In the examples above x = 3, y = 3, ans latticeSize = 3*3 = 9.

Remember that the 2D lattice is linearized and is thus represented as a 1D lattice.
So the above is just {1, 2, 3, 4, 5, 6, 7, 8, 9}.
*/
void Lattice::getHalfNeighbours(unsigned int site) {
    if (site < latticeSize - xDim) {
        // If not in the last row...
        // Then the next X is +1 to the right, and the next Y is a full xDim below.
        nextX = site + 1;
        nextY = site + xDim;
    } else if (site < latticeSize - 1) {
        // If site is in the last row but not on the last site...
        // Then next X is still +1 to the rightn, but next Y is at the first row.
        nextX = site + 1;
        nextY = site + xDim - latticeSize;
    } else {  // site = latticeSize - 1
        // If site is in the last row in the last element, then wrap around.
        // Then next X is the begining, and next Y is a row right below the begining.
        nextX = 0;
        nextY = xDim;
    }
}

// getNeighbours relies on getHalfneighbours to figure out the next X and Y positions
// using helical boundary conditions. This function then just focuses on the previous
// X and Y positions.
void Lattice::getNeighbours(unsigned int site) {
    getHalfNeighbours(site);

    if (site > xDim) {  // If site is below the 1st row...
        // Then the prev X is just 1 to the left, and xDim to the top.
        prevX = site - 1;
        prevY = site - xDim;
    } else if (site > 0) {  // If site is not the first element but it is in the first row...
        // Then previous X is to the left and prev Y is in the last row,
        // wrapping around to the end of the array.
        prevX = site - 1;
        prevY = site + latticeSize - xDim;
    } else {  // site = 0
        // Since the site is the first element, then prev X is the last element of the array.
        // And prev Y is xDim above the last element of the array.
        prevX = latticeSize - 1;
        prevY = latticeSize - xDim;
    }
}

int Lattice::calcHalfenergy(unsigned int site) {
    getHalfNeighbours(site);

    return -lattice[site] * (lattice[nextX] + lattice[nextY]);
}

int Lattice::calcEnergy(unsigned int site) {
    getNeighbours(site);

    return -lattice[site] * (lattice[nextX] + lattice[nextY] + lattice[prevX] + lattice[prevY]);
}

double Lattice::calcTotalEnergy() {
    totalEnergy = 0;
    for (unsigned int i = 0; i < latticeSize; i++)
        totalEnergy += calcHalfenergy(i);
    return totalEnergy / latticeSize;
}

double Lattice::calcMagnetization() {
    int magnet = 0;
    for (unsigned int i = 0; i < latticeSize; i++)
        magnet += lattice[i];
    return (double)magnet / latticeSize;
}

// calcSpecificHeat takes as input the average and the squared energies per spin.
double Lattice::calcSpecificHeat(double avgEnergy, double sqrdEnergy) {
    double diff = sqrdEnergy - (avgEnergy * avgEnergy);
    return beta * beta * diff * latticeSize;
}

// calcSusceptibility takes as input the average and the squared susceptibilities per spin.
double Lattice::calcSusceptibility(double avgMagnet, double sqrdMagnet) {
    double diff = sqrdMagnet - (avgMagnet * avgMagnet);
    return beta * diff * latticeSize;
}

bool Lattice::metropolis(unsigned int site) {
    // Only need to consider the change in energy at the given site that we may be flipping.
    // This is only half the actual energy change.
    int difference = -calcEnergy(site);

    if (difference <= 0) {  // This is a lower energy state so we flip.
        lattice[site] *= -1;
        return true;
    }

    // Since it isn't a lower energy state, let's accept the flip based on the boltzman factor.
    randomU = gsl_rng_uniform(generator);
    int exp_index = (int)(difference/2) - 1;
    probability = exponentials[exp_index];

    if (randomU < probability) {
        lattice[site] += -1;
        return true;
    }
    return false;
}

// growCluster is a method used by Wolff.
void Lattice::growCluster(unsigned int site, int spin) {
    getNeighbours(site);

    unsigned int currNextX = nextX;
    unsigned int currNextY = nextY;
    unsigned int currPrevX = prevX;
    unsigned int currPrevY = prevY;

    if (lattice[currPrevX] == spin && !cluster->find(currPrevX)) {
        randomU = gsl_rng_uniform(generator);
        if (randomU < probability)
    }
}