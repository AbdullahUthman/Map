#include <iostream>
#include <string>
#include "datastructures.h"

using namespace std;

const int NUM_NODES = 25;
const int INF = 2147483647;

struct Edge {
    int to;
    int weight;  // distance in meters
    Edge* next;

    Edge(int t, int w) : to(t), weight(w), next(nullptr) {}
};

// Adjacency list using linked lists with weights
struct AdjList {
    Edge* head;
    int edgeCount;

    AdjList() : head(nullptr), edgeCount(0) {}

    ~AdjList() {
        Edge* current = head;
        while (current != nullptr) {
            Edge* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void addEdge(int to, int weight) {
        Edge* newEdge = new Edge(to, weight);
        newEdge->next = head;
        head = newEdge;
        edgeCount++;
    }
};

AdjList adj[NUM_NODES];

/* ---------- PLACE NAMES ---------- */
const char* names[NUM_NODES] = {
    "Entrance 3",           // 0
    "Parking Area",         // 1
    "Entrance 2",           // 2
    "Sports Complex",       // 3
    "CAFE 1",               // 4
    "Green Area 1",         // 5 (waypoint only)
    "ATM",                  // 6
    "Business Inc Center",  // 7
    "Library",              // 8
    "Green Area 2",         // 9 (waypoint only)
    "Green Area 3",         // 10 (waypoint only)
    "Auditorium",           // 11
    "Admission Office",     // 12
    "Main Entrance",        // 13
    "Admin Office",         // 14
    "Student Affairs",      // 15
    "Cafe 2",               // 16
    "Gym",                  // 17
    "A-Block",              // 18
    "B-Block",              // 19
    "C-Block",              // 20
    "Mosque",               // 21
    "China Block",          // 22
    "Arena",                // 23
    "Green Area 4"          // 24 (waypoint only)
};

// Green areas that cannot be selected as source/destination
bool isGreenArea(int id) {
    return (id == 5 || id == 9 || id == 10 || id == 24);
}

// Helper to convert string to lowercase
string toLower(const string& str) {
    string result = str;
    for (size_t i = 0; i < result.length(); i++) {
        if (result[i] >= 'A' && result[i] <= 'Z')
            result[i] = result[i] + ('a' - 'A');
    }
    return result;
}

// BFS-based search (excluding green areas for source/dest)
int searchByName(const string& query, bool allowGreenArea = false) {
    string lowerQuery = toLower(query);

    // Use BFS to search through the graph
    Queue<int> q;
    bool* visited = new bool[NUM_NODES];

    for (int i = 0; i < NUM_NODES; i++) {
        visited[i] = false;
    }

    // Start BFS from node 0 (arbitrary starting point)
    q.push(0);
    visited[0] = true;

    // Check starting node first
    if (!isGreenArea(0) || allowGreenArea) {
        string lowerName = toLower(string(names[0]));
        if (lowerName.find(lowerQuery) != string::npos) {
            delete[] visited;
            return 0;
        }
    }

    // BFS traversal
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        // Check all neighbors
        Edge* edge = adj[u].head;
        while (edge != nullptr) {
            int v = edge->to;

            if (!visited[v]) {
                visited[v] = true;

                // Check if this node matches the query
                if (!isGreenArea(v) || allowGreenArea) {
                    string lowerName = toLower(string(names[v]));
                    if (lowerName.find(lowerQuery) != string::npos) {
                        delete[] visited;
                        return v;
                    }
                }

                q.push(v);
            }
            edge = edge->next;
        }
    }

    delete[] visited;
    return -1;
}

/* ---------- GRAPH SETUP ---------- */
void addRoad(int u, int v, int distance) {
    adj[u].addEdge(v, distance);
    adj[v].addEdge(u, distance);
}

void setupUniversity() {
    // Original connections
    addRoad(0, 3, 120);   addRoad(0, 1, 80);
    addRoad(1, 2, 60);    addRoad(1, 3, 100);
    addRoad(2, 4, 90);
    addRoad(3, 4, 150);   addRoad(3, 5, 180);
    addRoad(4, 5, 110);   addRoad(4, 6, 95);
    addRoad(5, 7, 85);    addRoad(5, 11, 130);
    addRoad(6, 7, 70);    addRoad(6, 10, 80);   addRoad(6, 12, 75);
    addRoad(7, 11, 90);   addRoad(7, 12, 65);
    addRoad(8, 9, 50);    addRoad(8, 10, 70);   addRoad(8, 13, 85);
    addRoad(9, 10, 45);   addRoad(9, 13, 60);
    addRoad(10, 12, 90);  addRoad(10, 13, 55);
    addRoad(11, 12, 80);
    addRoad(12, 13, 70);

    // New building connections - creating a complex graph
    addRoad(0, 18, 95);   // Entrance 3 -> A-Block
    addRoad(1, 17, 110);  // Parking -> Gym
    addRoad(2, 16, 85);   // Entrance 2 -> Cafe 2
    addRoad(3, 23, 140);  // Sports Complex -> Arena
    addRoad(4, 16, 75);   // CAFE 1 -> Cafe 2
    addRoad(6, 14, 60);   // ATM -> Admin Office
    addRoad(7, 15, 80);   // Business Inc -> Student Affairs
    addRoad(8, 14, 95);   // Library -> Admin Office
    addRoad(11, 23, 120); // Auditorium -> Arena
    addRoad(13, 14, 65);  // Main Entrance -> Admin Office
    addRoad(13, 15, 70);  // Main Entrance -> Student Affairs

    addRoad(14, 15, 55);  // Admin Office -> Student Affairs
    addRoad(15, 24, 65);  // Student Affairs -> Green Area 4
    addRoad(16, 17, 90);  // Cafe 2 -> Gym
    addRoad(17, 18, 85);  // Gym -> A-Block
    addRoad(18, 19, 70);  // A-Block -> B-Block
    addRoad(19, 20, 75);  // B-Block -> C-Block
    addRoad(20, 21, 60);  // C-Block -> Mosque
    addRoad(21, 22, 95);  // Mosque -> China Block
    addRoad(22, 23, 110); // China Block -> Arena
    addRoad(23, 24, 80);  // Arena -> Green Area 4
    addRoad(24, 12, 70);  // Green Area 4 -> Admission Office

    // Additional cross connections for multiple path options
    addRoad(18, 20, 130); // A-Block -> C-Block (skip B)
    addRoad(16, 24, 100); // Cafe 2 -> Green Area 4
    addRoad(17, 23, 150); // Gym -> Arena
    addRoad(19, 21, 120); // B-Block -> Mosque
    addRoad(14, 24, 85);  // Admin Office -> Green Area 4
}

/* ---------- DIJKSTRA'S ALGORITHM ---------- */
struct NodeDist {
    int node;
    int dist;

    NodeDist(int n, int d) : node(n), dist(d) {}
};

class MinHeap {
private:
    NodeDist** data;
    int size;
    int capacity;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (data[index]->dist < data[parent]->dist) {
                NodeDist* temp = data[index];
                data[index] = data[parent];
                data[parent] = temp;
                index = parent;
            } else break;
        }
    }

    void heapifyDown(int index) {
        while (true) {
            int smallest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < size && data[left]->dist < data[smallest]->dist)
                smallest = left;
            if (right < size && data[right]->dist < data[smallest]->dist)
                smallest = right;

            if (smallest != index) {
                NodeDist* temp = data[index];
                data[index] = data[smallest];
                data[smallest] = temp;
                index = smallest;
            } else break;
        }
    }

public:
    MinHeap() : size(0), capacity(NUM_NODES * 2) {
        data = new NodeDist*[capacity];
    }

    ~MinHeap() {
        for (int i = 0; i < size; i++)
            delete data[i];
        delete[] data;
    }

    void push(int node, int dist) {
        data[size] = new NodeDist(node, dist);
        heapifyUp(size);
        size++;
    }

    NodeDist* pop() {
        if (size == 0) return nullptr;
        NodeDist* result = data[0];
        size--;
        if (size > 0) {
            data[0] = data[size];
            heapifyDown(0);
        }
        return result;
    }

    bool empty() const {
        return size == 0;
    }
};

void dijkstra(int start, int end, int* dist, int* parent) {
    MinHeap pq;
    bool* visited = new bool[NUM_NODES];

    for (int i = 0; i < NUM_NODES; i++) {
        dist[i] = INF;
        parent[i] = -1;
        visited[i] = false;
    }

    dist[start] = 0;
    pq.push(start, 0);

    while (!pq.empty()) {
        NodeDist* current = pq.pop();
        int u = current->node;
        delete current;

        if (visited[u]) continue;
        visited[u] = true;

        if (u == end) break;

        Edge* edge = adj[u].head;
        while (edge != nullptr) {
            int v = edge->to;
            int weight = edge->weight;

            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push(v, dist[v]);
            }
            edge = edge->next;
        }
    }

    delete[] visited;
}

/* ---------- MAIN ---------- */
int main() {
    setupUniversity();

    char mode;
    cin >> mode;

    if (mode == 'F') {
        cin.ignore();
        string query;
        getline(cin, query);

        int result = searchByName(query, false);
        cout << result << endl;
        return 0;
    }

    if (mode == 'S') {
        int startNode, endNode;
        cin >> startNode >> endNode;

        if (isGreenArea(startNode) || isGreenArea(endNode)) {
            cout << "NoPath" << endl;
            return 0;
        }

        int* dist = new int[NUM_NODES];
        int* parent = new int[NUM_NODES];

        dijkstra(startNode, endNode, dist, parent);

        if (dist[endNode] == INF) {
            cout << "NoPath" << endl;
        } else {
            // Output distance first
            cout << "DIST " << dist[endNode] << endl;

            // Then output path
            int pathLen = 0;
            for (int v = endNode; v != -1; v = parent[v])
                pathLen++;

            int* path = new int[pathLen];
            int idx = pathLen - 1;
            for (int v = endNode; v != -1; v = parent[v]) {
                path[idx] = v;
                idx--;
            }

            for (int i = 0; i < pathLen; i++) {
                cout << path[i];
                if (i + 1 < pathLen)
                    cout << " ";
            }
            cout << endl;
            
            delete[] path;
        }
        
        delete[] dist;
        delete[] parent;
        
        return 0;
    }

    return 0;
}
