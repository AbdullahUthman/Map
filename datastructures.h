#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <iostream>

/* ========== CUSTOM QUEUE ========== */
template<typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* frontNode;
    Node* rearNode;
    int size;

public:
    Queue();
    ~Queue();

    void push(const T& value);
    void pop();
    T& front();
    const T& front() const;

    bool empty() const;
    int getSize() const;
    void clear();
};

// Template implementations
#include "datastructures.cpp"

#endif
