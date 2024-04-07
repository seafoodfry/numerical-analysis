/* HasTable.h
Implements a basic hash table with chaining.

This is based on https://inspirehep.net/literature/1386200 ,
Lattice Simulations of Nonperturbative Quantum Field Theories
by David Schaich
*/
#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include <vector>

// The original aim of this struct was to avoid the overhead of a class.
struct node {
    unsigned int value;
    node* next;
};

class HashTable {
    public:
        HashTable(unsigned int tableNumber);
        HashTable();
        ~HashTable();

        // Member functions.
        void insert(unsigned int site);
        bool find(unsigned int site);

        // Member data.
        unsigned int size;
        unsigned int tableNumber;
        unsigned int mod;
        std::vector<node*> table;
    private:
};


#endif _HASHTABLE_H