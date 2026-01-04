#ifndef DATASTRUCTURES_CPP
#define DATASTRUCTURES_CPP

/* ========== QUEUE IMPLEMENTATION ========== */
/*
 * Custom Queue implementation using linked list
 * Used for BFS traversal in the graph
 * No standard library queue used - built from scratch
 */

template<typename T>
Queue<T>::Queue() : frontNode(nullptr), rearNode(nullptr), size(0) {}

template<typename T>
Queue<T>::~Queue() {
    clear();
}

template<typename T>
void Queue<T>::push(const T& value) {
    Node* newNode = new Node(value);

    if (rearNode == nullptr) {
        // Queue is empty
        frontNode = rearNode = newNode;
    } else {
        // Add to rear
        rearNode->next = newNode;
        rearNode = newNode;
    }
    size++;
}

template<typename T>
void Queue<T>::pop() {
    if (frontNode == nullptr) return;

    Node* temp = frontNode;
    frontNode = frontNode->next;

    if (frontNode == nullptr) {
        // Queue is now empty
        rearNode = nullptr;
    }

    delete temp;
    size--;
}

template<typename T>
T& Queue<T>::front() {
    return frontNode->data;
}

template<typename T>
const T& Queue<T>::front() const {
    return frontNode->data;
}

template<typename T>
bool Queue<T>::empty() const {
    return frontNode == nullptr;
}

template<typename T>
int Queue<T>::getSize() const {
    return size;
}

template<typename T>
void Queue<T>::clear() {
    while (!empty()) {
        pop();
    }
}

#endif
