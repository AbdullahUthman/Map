#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

const int NUM_NODES = 20;
const int INF = INT_MAX;

struct Edge { int to; };
vector<vector<Edge>> adj(NUM_NODES);

void addRoad(int u, int v) {
    adj[u].push_back({v});
    adj[v].push_back({u});
}

void setupCity() {
    // 1. GRID CONNECTIONS
    addRoad(0,1); addRoad(1,2);
    addRoad(3,4); addRoad(4,5); addRoad(5,6);
    addRoad(7,8); addRoad(8,9); addRoad(9,10);
    addRoad(11,12); addRoad(12,13); addRoad(13,14);
    addRoad(15,16); addRoad(16,17); addRoad(17,18); addRoad(18,19);

    // Vertical
    addRoad(0,4); addRoad(4,8); addRoad(8,12); addRoad(12,16);
    addRoad(1,5); addRoad(5,9); addRoad(9,13); addRoad(13,17);
    addRoad(3,7); addRoad(7,11); addRoad(11,15);
    addRoad(2,6); addRoad(6,10); addRoad(10,14); addRoad(14,18);

    // 2. DIAGONALS / EXPRESS (The Mesh)
    // Park (8) Hub
    addRoad(8,0); addRoad(8,1); addRoad(8,2);
    addRoad(8,3); addRoad(8,6);
    addRoad(8,11); addRoad(8,14);
    addRoad(8,15); addRoad(8,19);
    addRoad(8,5); addRoad(8,12);

    // Comm (5) Hub
    addRoad(5,0); addRoad(5,2);
    addRoad(5,7); addRoad(5,10);
    addRoad(5,12); addRoad(5,14);
    addRoad(5,17);
}

int main() {
    setupCity();

    int startNode, endNode;
    if (cin >> startNode >> endNode) {
        // Dijkstra
        queue<int> q;
        q.push(startNode);
        vector<int> dist(NUM_NODES, INF);
        vector<int> parent(NUM_NODES, -1);
        dist[startNode] = 0;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (u == endNode) break;

            for (auto& edge : adj[u]) {
                if (dist[edge.to] == INF) {
                    dist[edge.to] = dist[u] + 1;
                    parent[edge.to] = u;
                    q.push(edge.to);
                }
            }
        }

        if (dist[endNode] == INF) {
            cout << "NoPath" << endl;
        } else {
            vector<int> path;
            for (int v = endNode; v != -1; v = parent[v]) path.push_back(v);
            reverse(path.begin(), path.end());

            // Print clean space-separated string
            for (int i = 0; i < path.size(); i++) {
                cout << path[i] << (i == path.size() - 1 ? "" : " ");
            }
            cout << endl;
        }
    }
    return 0;
}