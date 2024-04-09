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
#include "HashTable.h"
#include <vector>
#include <cstddef>  // NULL.

HashTable::HashTable(unsigned int tableNumber) {
    size = 0;
    std::vector<node*>* temp = new std::vector<node*>(tableNumber, NULL);
    table = *temp;
    mod = tableNumber - 1;
}

HashTable::HashTable() {
    HashTable(4093);
}

HashTable::~HashTable() {}

void HashTable::insert(unsigned int site) {
    size++;

    node* toAdd = new node;
    toAdd->value = site;

    unsigned int index = (17 * site - 97) & mod;
    toAdd->next = table[index];
    table[index] = toAdd;
}

bool HashTable::find(unsigned int site) {
    unsigned int index = (17 * site - 97) & mod;
    node* temp = table[index];

    while (temp != NULL) {
        if (site == temp->value) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

void HashTable::clear() {
    // Step 1: Delete all nodes in each list.
    for (unsigned int i = 0; i < table.size(); i++) {
        node* current = table[i];
        while (current != nullptr) {
            node* toDelete = current;
            current = current->next;
            delete toDelete; // Delete the node to prevent memory leaks.
        }
        table[i] = nullptr; // Reset the head of the list to indicate it's empty.
    }

    // Step 2: Reset the hash table's state.
    size = 0;
}
