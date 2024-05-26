/* lattice.cpp
Implements a lattice of spins for an Ising model simulation.

This is based on https://inspirehep.net/literature/1386200 ,
Lattice Simulations of Nonperturbative Quantum Field Theories
by David Schaich
*/
#include "Lattice.h"
#include <cstdio>    // For fflush and stdout.
#include <stdexcept> // For std::runtime_error
#include <string>    // For std::to_string()
#include <fstream>
#include <iostream>
#include <filesystem>


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
            lattice[i] = -1;
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
            printf("o");
        else if (lattice[i] == 1)
            printf("x");
        else
            throw std::runtime_error("Lattice integrity violation at index " + std::to_string(i) + ", the spin was: " + std::to_string(lattice[i]));
    }
    printf("\n");
    fflush(stdout);
}

void Lattice::saveLatticeToFile(const std::filesystem::path& dirPath, const std::string& filename) {
    std::filesystem::path fullPath = dirPath / filename;
    std::ofstream file(fullPath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + fullPath.string());
    }

    for (unsigned int i = 0; i < latticeSize; i++) {
        if (i % xDim == 0)
            file << "\n";

        if (lattice[i] == -1)
            file << "o";
        else if (lattice[i] == 1)
            file << "x";
        else
            throw std::runtime_error("Lattice integrity violation at index " + std::to_string(i) + ", the spin was: " + std::to_string(lattice[i]));
    }
    file << "\n";
    file.close();
}

void Lattice::printCluster() {
    for (unsigned int i = 0; i < latticeSize; i++) {
        if (i % xDim == 0)
            printf("\n");

        if (cluster->find(i))
            printf("x");
        else
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
// Note: also note that this snippet reveals a bug in the Ising model code snippet,
// as the first if statement for determining prevX and prevY should have the conditional
// (site >= xDim), instead of (site > xDim).
void Lattice::getNeighbours(unsigned int site) {
    getHalfNeighbours(site);

    if (site >= xDim) {  // If site is below the 1st row...
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

// calcHalfenergy only counts half the interactions in the model because its used by calcTotalenergy
// so we do half to avoid double counting interactions.
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

unsigned int Lattice::flipped(unsigned int site) {
    unsigned int spin = lattice[site];
    // A spin should never be anything other than +1 or -1.
    if (spin != 1 && spin != -1) {
        throw std::runtime_error("Lattice integrity violation at index " + std::to_string(site) + ", the spin was: " + std::to_string(spin));
    }
    return -spin;
}

/* metropolis performs a metropolis step

Note that there are 5 possible initial states (see the accompanying notebook for a
more detailed exaplanation), and in those 5 possible initial states, 3 represent a spin
being flipped and an end state with a lower energy which will be accepted.
So we just need to identify the initial states that correspond in a metropolis state that will absolutely be
accepted, and only compute the boltzmann facotr for the probabilistic acceptance of the 2 other
states which take the system to a higher energy state.

The 2 states that result in a the system's energy increasing are:
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

Note that the initial energy is negative for these 2 states - the energies for the states that
are always accepted is 0J -> 0J, 2J -> -2J, and 4J -> -4J.
*/
bool Lattice::metropolis(unsigned int site) {
    // Performance obten makes us skip steps, but something that can prevent a million erroneous
    // runs is a great time-saving investment.
    if (lattice[site] != 1 && lattice[site] != -1) {
        throw std::runtime_error("Lattice integrity violation at index " + std::to_string(site) + ", the spin was: " + std::to_string(lattice[site]));
    }


    // This result of calcEnergy is only the initial energy. Reemember that we can tell from the
    // initial/current energy of the state whether the transition will be accepted or whether it will
    // be probabilistically accepted: only the initial states with energy -4J and -2J require us to compute
    // their Boltzmann factors for a probabilistic acceptance.
    // Hence, we are converting the computed current energy to obtain the "final" energy.
    // That way we can use a slightly easier to read logic...
    int finalE = -calcEnergy(site);

    // The three possible transition where we go from a state of higher energy to a lower one
    // correspond to states where the final energy is 0J, -2J, or -4J.
    // So if the computed finalE matches any of these values we accept the energy "step down".
    if (finalE <= 0) {  // This is a lower energy state so we flip.
        lattice[site] = flipped(site);
        return true;
    }

    // Since it isn't a lower energy state, let's accept the flip based on the Boltzman factor.
    randomU = gsl_rng_uniform(generator);
    // The value at 0 is the Boltzmann factor -4beta, the next one is -8beta.
    int exp_index = (int)(finalE/2) - 1;
    probability = exponentials[exp_index];
    if (randomU < probability) {
        lattice[site] = flipped(site);
        return true;
    }
    return false;
}

// growCluster is a method used by Wolff.
void Lattice::growCluster(unsigned int site, int spin) {
    getNeighbours(site);

    unsigned int curNextX = nextX;
    unsigned int curNextY = nextY;
    unsigned int curPrevX = prevX;
    unsigned int curPrevY = prevY;

    // TODO: the way the probability variable is iffy and should be fixed.
    if (lattice[curPrevX] == spin && !cluster->find(curPrevX)) {
        randomU = gsl_rng_uniform(generator);
        if (randomU < probability) {
            cluster->insert(curPrevX);
            growCluster(curPrevX, spin);
        }
    }
    if (lattice[curNextX] == spin && !cluster->find(curNextX)) {
        randomU = gsl_rng_uniform(generator);
        if (randomU < probability) {
            cluster->insert(curNextX);
            growCluster(curNextX, spin);
        }
    }
    if (lattice[curPrevY] == spin && !cluster->find(curPrevY)) {
        randomU = gsl_rng_uniform(generator);
        if (randomU < probability) {
            cluster->insert(curPrevY);
            growCluster(curPrevY, spin);
        }
    }
    if (lattice[curNextY] == spin && !cluster->find(curNextY)) {
        randomU = gsl_rng_uniform(generator);
        if (randomU < probability) {
            cluster->insert(curNextY);
            growCluster(curNextY, spin);
        }
    }
}

void Lattice::flipCluster() {
    // Note: we rewrote this code. The original listing had a non-existent type Node.
    // And it also mentioned bits like `cluster->table[i]->head`, where head was never implemented
    // in the code sample for the HashTable.
    for (unsigned int i = 0; i < 97; i++) {
        if (cluster->table.size() != 0) {
            node* temp = cluster->table[i];
            for (unsigned int j = 0; j < cluster->table.size(); j++) {
                lattice[temp->value] *= -1;
                temp = temp->next;
            }
        }
    }
}

void Lattice::flipComplement() {
    for (unsigned int i = 0; i < latticeSize; i++) {
        if (!cluster->find(i))
            lattice[i] *= -1;
    }
}

// wolff returns the size of the cluster.
unsigned int Lattice::wolff(unsigned int site) {
    cluster->insert(site);
    growCluster(site, lattice[site]);

    if (cluster->size >= latticeSize/2)
        flipComplement();
    else
        flipCluster();

    unsigned int clusterSize = cluster->size;
    cluster->clear();
    return clusterSize;
}