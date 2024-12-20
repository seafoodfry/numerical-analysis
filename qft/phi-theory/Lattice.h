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
#include <memory>
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
        explicit Lattice(double mu, double lambda, unsigned int x, unsigned int y);
        ~Lattice() = default; // Destructor is no longer required to delete cluster.

        void printLattice();
        void printSigns();
        void printClusters();

        double calcTotalEnergy();
        double calcAvgPhi();

        void metropolis(unsigned int site);
        
        bool clusterCheck(unsigned int site, unsigned int toAdd);
        void growClusterPos(unsigned int site);
        void growClusterNeg(unsigned int site);
        void flipCluster();
        unsigned int wolff(unsigned int site);  // Returns cluster size.

        unsigned int getRandomSite();

    private:
        double muSquared;
        double lambda;
        unsigned int xDim;
        unsigned int yDim;
        unsigned int latticeSize;
        std::vector<double> lattice;

        // Simple version: gsl_rng* generator;
        std::unique_ptr<gsl_rng, decltype(&gsl_rng_free)> generator;
        // Simple version is: std::vector<siteNeighbours*> neighbours;
        std::vector<std::unique_ptr<siteNeighbours>> neighbours;
        // Simple version: HashTable* cluster;
        std::unique_ptr<HashTable> cluster;

        double genU();
        double genRandomPhiValue();
        void getHelicalNeighbours(unsigned int site, siteNeighbours* toInit);
};


#endif // _LATTICE_H