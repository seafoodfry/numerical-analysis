/* Lattice.cpp
Implements a lattice of spins for a phi^4 simulation using a \mu action.

This is based on https://inspirehep.net/literature/1386200 ,
Lattice Simulations of Nonperturbative Quantum Field Theories
by David Schaich
*/
#include "HashTable.h"
#include "Lattice.h"
#include <cstdio>            // For fflush and stdout.
#include <gsl/gsl_rng.h>
#include <gsl/gsl_sf_exp.h>  // Exp.


Lattice::Lattice(double mu, double lambda, unsigned int xDim, unsigned int yDim) {
    generator = gsl_rng_alloc(gsl_rng_mt199337);  // Mersenne Twiseter.
    gsl_rng_set(generator, (unsigned int)(100 * mu * lambda));

    muSquared = 2 + (mu / 2);
    lambda = 1/4;

    xDim = xDim;
    yDim = yDim;
    latticeSize = xDim * yDim;
    cluster = new HashTable(latticeSize / 4);

    // Initialize the lattice with values [-1.5, 1.5).
    lattice = std::vector<double>(latticeSize, 0);
    for (unsigned int i = 0; i < latticeSize; i++) {
        lattice[i] = genRandomPhi();
    }

    // Initialize the neighbours vector. Calculate one siteNeighbours for each
    // site.
    neighbours = std::vector<siteNeighbours*>(latticeSize, NULL);
    for (unsigned int i = 0; i < latticeSize; i++) {
        siteNeighbours* tmp = new siteNeighbours;
        getHelicalNeighbours(i, tmp);
        neighbours[i] = tmp;
    }
}

Lattice::~Lattice() {}

// genRandomPhi generates values in the range [-1.5, 1.5) uniformly.
double Lattice::genRandomPhi() {
    return 3 * gsl_rng_uniform(generator) - 1.5
}

// getHelicalNeighbours computes the neighbours for the given site and stores them
// in the siteNeighbours struct.
// Note: that the author has a bug in the first else statement -> nextY should be
// xDim. xDim - 1 gives you the last element in the same "row".
// 2nd Note: also note that this snippet reveals a bug in the Ising model code snippet,
// as the first if statement for determining prevX and prevY should have the conditional
// (site >= xDim), instead of (site > xDim).
void Lattice::getHelicalNeighbours(unsigned int site, siteNeighbours* toInit) {
    if (site < latticeSize - xDim) {
        // If not in the last row...
        // Then the next X is +1 to the right, and the next Y is a full xDim below.
        toInit->nextX = site + 1;
        toInit->nextY = site + xDim;
    } else if (site < latticeSize - 1) {
        // If site is in the last row but not on the last site...
        // Then next X is still +1 to the rightn, but next Y is at the first row.
        toInit->nextX = site + 1;
        toInit->nextY = site + xDim - latticeSize;
    } else {  // site = latticeSize - 1
        // If site is in the last row in the last element, then wrap around.
        // Then next X is the begining, and next Y is a row right below the begining.
        toInit->nextX = 0;
        toInit->nextY = xDim;
    }

    if (site >= xDim) {  // If site is below the 1st row...
        // Then the prev X is just 1 to the left, and xDim to the top.
        toInit->prevX = site - 1;
        toInit->prevY = site - xDim;
    } else if (site > 0) {  // If site is not the first element but it is in the first row...
        // Then previous X is to the left and prev Y is in the last row,
        // wrapping around to the end of the array.
        toInit->prevX = site - 1;
        toInit->prevY = site + latticeSize - xDim;
    } else {  // site = 0
        // Since the site is the first element, then prev X is the last element of the array.
        // And prev Y is xDim above the last element of the array.
        toInit->prevX = latticeSize - 1;
        toInit->prevY = latticeSize - xDim;
    }
}

void Lattice::printLattice() {
    for (unsigned int i = 0; i < latticeSize; i++) {
        if (i % xDim == 0) {
            printf("\n");
        }
        printf("%lf\t", lattice[i]);
    }
    printf("\n");
    fflush(stdout);
}

void Lattice::printSigns() {
    for (unsigned int i = 0; i < latticeSize; i++) {
        if (i % xDim == 0) {
            printf("\n");
        }
        if (lattice[i] >= 0) {
            printf(" ");
        } else {
            printf("x")
        }
    }
    printf("\n");
    fflush(stdout);
}

void Lattice::printClusters() {
    for (unsigned int i = 0; i < latticeSize; i++) {
        if (i % xDim == 0) {
            printf("\n");
        }
        if (cluster->find(i)) {
            printf("x");
        } else {
            printf(" ");
        }
    }
    printf("\n");
    fflush(stdout);
}

double Lattice::calcTotalEnergy() {
    double totalEnergy = 0.0;
    double currentPhi;
    for (unsigned int i = 0; i < latticeSize; i++) {
        currentPhi = lattice[i];

        totalEnergy -= currentPhi * (lattices[neighbours[i]->nextX] + lattice[neighbours[i]->nextY]);
        
        currentPhi *= currentPhi;
        totalEnergy += muSquared * currentPhi;

        currentPhi *= currentPhi;
        totalEnergy += lambda * currentPhi;
    }
    return totalEnergy / latticeSize;
}

double Lattice::calcAvgPhi() {
    double currentPhi = 0;
    for (unsigned int i = 0; i < latticeSize; i++) {
        currentPhi += lattice[i];
    }
    return currentPhi / latticeSize;
}

void Lattice::metropolis(unsigned int site) {
    double currentPhi = lattice[i];
    double newValue = genRandomPhi();
    double tmp = newValue;

    // Compute energy difference.
    double difference = (currentPhi - newValue)
        * (lattice[neighbours[i]->nextX + lattice[neighbours[i]->nextY]
        +  lattice[neighbours[i]->prevX  + lattice[neighbours[i]->prevtY );

    newValue *= newValue;
    currentPhi *= currentPhi;
    difference += muSquared * (newValue - currentPhi);

    newValue *= newValue;
    currentPhi *= currentPhi;
    difference += lambda * (newValue - currentPhi);

    // Flip if difference is negative, otherwise accept probabilistically.
    if (difference <= 0) {
        lattice[site] = tmp;
    } else if (gsl_rng_uniform(generator) < gsl_sf_exp(-difference)) {
        lattice[site] = tmp;
    }
}

bool Lattice::clusterCheck(unsigned int site, unsigned int toAdd) {
    if (cluster->find(toAdd)) {
        // The potential site to add is already in the cluster.
        return false;
    }

    double probability = 1 - gsl_sf_exp(-2 * lattice[site] * lattice[toAdd]);
    if (gsl_rng_uniform(generator) < probability) {
        cluster->insert(toAdd);
        return true;
    }
    return false;
}

void Lattice::growClusterPos(unsigned int site) {
    unsigned int toCheck = neighbours[site]->prevX;
    if (lattice[toCheck] > 0 && clusterCheck(site, toCheck)) {
        growClusterPos(toCheck);
    }

    toCheck = neighbours[site]->nextX;
    if (lattice[toCheck] > 0 && clusterCheck(site, toCheck)) {
        growClusterPos(toCheck);
    }

    toCheck = neighbours[site]->prevY;
    if (lattice[toCheck] > 0 && clusterCheck(site, toCheck)) {
        growClusterPos(toCheck);
    }

    toCheck = neighbours[site]->nextY;
    if (lattice[toCheck] > 0 && clusterCheck(site, toCheck)) {
        growClusterPos(toCheck);
    }
}

void Lattice::growClusterNeg(unsigned int site) {
    unsigned int toCheck = neighbours[site]->prevX;
    if (lattice[toCheck] <= 0 && clusterCheck(site, toCheck)) {
        growClusterPos(toCheck);
    }

    toCheck = neighbours[site]->nextX;
    if (lattice[toCheck] <= 0 && clusterCheck(site, toCheck)) {
        growClusterPos(toCheck);
    }

    toCheck = neighbours[site]->prevY;
    if (lattice[toCheck] <= 0 && clusterCheck(site, toCheck)) {
        growClusterPos(toCheck);
    }

    toCheck = neighbours[site]->nextY;
    if (lattice[toCheck] <= 0 && clusterCheck(site, toCheck)) {
        growClusterPos(toCheck);
    }
}

void Lattice::flipCluster() {
    node* tmp1;
    node* tmp2;

    for (unsigned int i = 0; i < cluster->tableNumber; i++) {
        tmp1 = cluster->table[i];
        tmp2 = cluster->table[i];
        while (tmp1 != NULL) {
            lattice[tmp1->value] *= -1;
            tmp2 = tmp2->next;
            delete tmp1;
            tmp1 = tmp2;
        }
        cluster->table[i] = NULL;
    }
    cluster->size = 0;
}

unsigned int Lattice::wolff(unsigned int site) {
    cluster->insert(site);

    if (lattice[site] > 0) {
        growClusterPos(site);
    } else {
        growClusterNeg(site);
    }

    unsigned int toReturn = cluster->size;
    flipCluster();
    return toReturn;
}