/* HasTable.cpp
Implements a hash table designed for lists (vectors) of nodes.

This is based on https://inspirehep.net/literature/1386200 ,
Lattice Simulations of Nonperturbative Quantum Field Theories
by David Schaich.

A hash table was chosen for these simulations because a considerable ammount of time
was being spend keeping the lattice-representing data strctures (creating, updating, and deleting).

This is an actual hash table implementation where a node is added to linked list
at a certain index in the table vector pseudorandomly.
As such, the author mentions that the number 97 was chosen "because having a prime number
of lists in the hash table increases the randomness of the hash function."

In his thesis the author mentions the following (page 126, section B3 Data Structures):

    In addition, it is a good diea to adjust the number of lists in the hash table so that
the average list is only a few elements long. When we increased the number of lists in the
hash table from 97 to one quarter of the size of the lattice in \phi^4 simulations, the program's
running time decreased by around 20%.
*/
// HashTable.cpp
#include "HashTable.h"

Node::Node(unsigned int val) : value(val), next(nullptr) {}

HashTable::HashTable(unsigned int tableNum)
    : size(0), tableNumber(tableNum), table(tableNum) {}

void HashTable::insert(unsigned int site) {
    auto index = site % tableNumber;
    auto newNode = std::make_unique<Node>(site);
    // We wouldn't need the std::move if we had shared_ptrs.
    newNode->next = std::move(table[index]);  // Like ewNode->next = table[index];
    table[index] = std::move(newNode);  // Like table[index] = newNode;
    size++;
}

bool HashTable::find(unsigned int site) {
    auto index = site % tableNumber;
    auto current = table[index].get();  // The .get() is not necessary for s shared_ptr.
    while (current) {
        if (current->value == site) {
            return true;
        }
        current = current->next.get();  // The .get() is not necessary for s shared_ptr.
    }
    return false;
}

void HashTable::clear() {
    for (auto& head : table) {
        // head = nullptr;  // Clears the list due to smart pointers
        head.reset();
    }
    size = 0;
}

const std::vector<std::unique_ptr<Node>>& HashTable::getTable() const {
    return table;
}

unsigned int HashTable::getTableSize() const {
    return tableNumber;
}

unsigned int HashTable::getNumberOfNodes() const {
    return size;
}