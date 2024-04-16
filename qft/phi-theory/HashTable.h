// HashTable.h
#pragma once

#include <memory>
#include <vector>

struct Node {
    unsigned int value;
    std::unique_ptr<Node> next;

    Node(unsigned int val);
};

class HashTable {
public:
    explicit HashTable(unsigned int tableNumber = 4093);
    ~HashTable() = default;

    void insert(unsigned int site);
    bool find(unsigned int site);
    void clear();

    const std::vector<std::unique_ptr<Node>>& getTable() const;  // Expose table for external manipulation.
    unsigned int getTableSize() const;  // Get the size of the table.
    unsigned int getNumberOfNodes() const;  // Get the total number of nodes.

private:
    unsigned int size;
    unsigned int tableNumber;
    std::vector<std::unique_ptr<Node>> table;
};
