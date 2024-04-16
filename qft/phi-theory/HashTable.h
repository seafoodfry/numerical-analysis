/* HasTable.h
Implements a basic hash table with chaining.

This is based on https://inspirehep.net/literature/1386200 ,
Lattice Simulations of Nonperturbative Quantum Field Theories
by David Schaich
*/
// HashTable.h
#pragma once

#include <memory>
#include <vector>

struct Node {
    unsigned int value;
    std::shared_ptr<Node> next;

    Node(unsigned int val);
};

class HashTable {
public:
    explicit HashTable(unsigned int tableNumber = 4093);
    ~HashTable() = default;

    void insert(unsigned int site);
    bool find(unsigned int site);
    void clear();

    unsigned int size;
    unsigned int tableNumber;
    std::vector<std::shared_ptr<Node>> table;
private:
    unsigned int mod;
};
