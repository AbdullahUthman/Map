#include <iostream>
#include <climits>
using namespace std;

#define MAX 100

// ---------- Queue Implementation ----------
struct QNode {
    int data;
    QNode* next;
};

class Queue {
private:
    QNode* front;
    QNode* rear;

public:
    Queue() {
        front = rear = NULL;
    }

    bool isEmpty() {
        return front == NULL;
    }

    void push(int x) {
        QNode* temp = new QNode;
        temp->data = x;
        temp->next = NULL;

        if (rear == NULL) {
            front = rear = temp;
            return;
        }
        rear->next = temp;
        rear = temp;
    }

    int pop() {
        if (isEmpty()) return -1;

        QNode* temp = front;
        int value = temp->data;
        front = front->next;

        if (front == NULL)
            rear = NULL;

        delete temp;
        return value;
    }

    ~Queue() {
        while (!isEmpty())
            pop();
    }
};

// ---------- Graph Node ----------
struct Node {
    int vertex;
    int weight;
    Node* next;
};

// ---------- Graph Class ----------
class Graph {
private:
    int V;
    int adjMatrix[MAX][MAX];
    Node* adjList[MAX];

    void dfsHelper(int node, bool visited[]) {
        visited[node] = true;
        cout << node << " ";

        Node* temp = adjList[node];
        while (temp != NULL) {
            if (!visited[temp->vertex])
                dfsHelper(temp->vertex, visited);
            temp = temp->next;
        }
    }

    bool cycleDFS(int node, int parent, bool visited[]) {
        visited[node] = true;

        Node* temp = adjList[node];
        while (temp != NULL) {
            int v = temp->vertex;

            if (!visited[v]) {
                if (cycleDFS(v, node, visited))
                    return true;
            } else if (v != parent) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

public:
    Graph(int vertices) {
        V = vertices;

        for (int i = 0; i < V; i++) {
            adjList[i] = NULL;
            for (int j = 0; j < V; j++)
                adjMatrix[i][j] = 0;
        }
    }

    ~Graph() {
        for (int i = 0; i < V; i++) {
            Node* curr = adjList[i];
            while (curr) {
                Node* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
    }

    void addEdge(int u, int v, int w) {
        if (u < 0 || v < 0 || u >= V || v >= V)
            return;

        adjMatrix[u][v] = w;
        adjMatrix[v][u] = w;

        Node* n1 = new Node{v, w, adjList[u]};
        adjList[u] = n1;

        Node* n2 = new Node{u, w, adjList[v]};
        adjList[v] = n2;
    }

    void printAdjMatrix() {
        cout << "\nAdjacency Matrix:\n";
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++)
                cout << adjMatrix[i][j] << " ";
            cout << endl;
        }
    }

    void printAdjList() {
        cout << "\nAdjacency List:\n";
        for (int i = 0; i < V; i++) {
            cout << i << " -> ";
            Node* temp = adjList[i];
            while (temp) {
                cout << "(" << temp->vertex << ", w=" << temp->weight << ") ";
                temp = temp->next;
            }
            cout << endl;
        }
    }

    void BFS(int start) {
        bool visited[MAX] = {false};
        Queue q;

        visited[start] = true;
        q.push(start);

        cout << "\nBFS: ";
        while (!q.isEmpty()) {
            int node = q.pop();
            cout << node << " ";

            Node* temp = adjList[node];
            while (temp) {
                if (!visited[temp->vertex]) {
                    visited[temp->vertex] = true;
                    q.push(temp->vertex);
                }
                temp = temp->next;
            }
        }
        cout << endl;
    }

    void DFS(int start) {
        bool visited[MAX] = {false};
        cout << "\nDFS: ";
        dfsHelper(start, visited);
        cout << endl;
    }

    void checkCycle() {
        bool visited[MAX] = {false};

        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                if (cycleDFS(i, -1, visited)) {
                    cout << "\nCycle Detected!\n";
                    return;
                }
            }
        }
        cout << "\nNo Cycle Detected.\n";
    }

    void dijkstra(int start) {
        int dist[MAX];
        bool visited[MAX];

        for (int i = 0; i < V; i++) {
            dist[i] = INT_MAX;
            visited[i] = false;
        }

        dist[start] = 0;

        for (int count = 0; count < V - 1; count++) {
            int u = -1;
            int minDist = INT_MAX;

            for (int i = 0; i < V; i++) {
                if (!visited[i] && dist[i] < minDist) {
                    minDist = dist[i];
                    u = i;
                }
            }

            if (u == -1) break; // disconnected graph
            visited[u] = true;

            Node* temp = adjList[u];
            while (temp) {
                int v = temp->vertex;
                int w = temp->weight;

                if (!visited[v] && dist[u] + w < dist[v])
                    dist[v] = dist[u] + w;

                temp = temp->next;
            }
        }

        cout << "\nDijkstra from " << start << ":\n";
        for (int i = 0; i < V; i++)
            cout << "Node " << i << " : " << dist[i] << endl;
    }
};
