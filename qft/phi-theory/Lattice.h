/* Lattice.h
Implements a lattice of spins for a phi^4 simulation using a \mu action.

This is based on https://inspirehep.net/literature/1386200 ,
Lattice Simulations of Nonperturbative Quantum Field Theories
by David Schaich
*/
#ifndef _LATTICE_H
#define _LATTICE_H

#include "HashTable.h"
#include <vector>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_sf_exp.h>


struct siteNeighbours {
    unsigned int prevX;
    unsigned int nextX;
    unsigned int prevY;
    unsigned int nextY;
};


class Lattice {
    public:
        std::vector<double> lattice;
        unsigned int xDim;
        unsigned int yDim;
        unsigned int latticeSize;

        double muSquared;  // Mass of particles.
        double lambda;     // Coupling strenght.

        std::vector<siteNeighbours*> neighbours;
        HashTable* cluster;

        gsl_rng* generator;

        Lattice(double mu, double lambda, unsigned int x, unsigned int y);
        ~Lattice();

        void printLattice();
        void printSigns();
        void printClusters();

        void getNeighbours(unsigned int site, siteNeighbours* toInit);

        double calcTotalEnergy();
        double calcAvgPhi();

        void metropolis(unsigned int site);
        bool clusterCheck(unsigned int site, unsigned int toAdd);

        void growClusterPos(unsigned int site);
        void growClusterNeg(unsigned int site);
        void flipCluster();
        unsigned int wolff(unsigned int site);  // Returns cluster size.
    private:
};


#endif // _LATTICE_H