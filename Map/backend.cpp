#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <string>

using namespace std;

const int NUM_NODES = 20;
const int INF = INT_MAX;

struct Edge {
    int to;
};

vector<vector<Edge>> adj(NUM_NODES);

/* ---------- PLACE NAMES ---------- */
string names[NUM_NODES] = {
    "Entrance", "Fire Stn", "Gaming",
    "Res Block 1", "Police Stn", "Commercial", "Food Court",
    "Res Block 2", "Public Park", "Mall", "Cinema",
    "Sports Cmplx", "Hospital", "School", "Library",
    "Parking", "Pharmacy", "Clinic", "Playground", "Exit"
};

int searchByName(const string& q) {
    for (int i = 0; i < NUM_NODES; i++) {
        if (names[i].find(q) != string::npos)
            return i;
    }
    return -1;
}


/* ---------- GRAPH SETUP ---------- */
void addRoad(int u, int v) {
    adj[u].push_back({v});
    adj[v].push_back({u});
}

void setupCity() {
    int roads[][2] = {
        {0,1},{1,2},{3,4},{4,5},{5,6},{7,8},{8,9},{9,10},
        {11,12},{12,13},{13,14},{15,16},{16,17},{17,18},{18,19},
        {0,4},{4,8},{8,12},{12,16},
        {1,5},{5,9},{9,13},{13,17},
        {3,7},{7,11},{11,15},
        {2,6},{6,10},{10,14},{14,18},
        {8,0},{8,1},{8,2},{8,3},{8,6},{8,11},{8,14},{8,15},{8,19},{8,5},{8,12},
        {5,0},{5,2},{5,7},{5,10},{5,12},{5,14},{5,17}
    };

    for (auto &r : roads)
        addRoad(r[0], r[1]);
}

/* ---------- BFS SEARCH ---------- */
int bfsSearchByName(const string& target) {
    vector<bool> visited(NUM_NODES, false);
    queue<int> q;

    q.push(0);
    visited[0] = true;

    while (!q.empty()) {
        int u = q.front(); q.pop();

        if (names[u].find(target) != string::npos)
            return u;

        for (auto &e : adj[u]) {
            if (!visited[e.to]) {
                visited[e.to] = true;
                q.push(e.to);
            }
        }
    }
    return -1;
}

/* ---------- MAIN ---------- */

    int main() {
        setupCity();

        char mode;
        cin >> mode;

        if (mode == 'F') {
            string query;
            getline(cin >> ws, query);
            cout << searchByName(query) << endl;
            return 0;
        }

        if (mode == 'S') {
            int startNode, endNode;
            cin >> startNode >> endNode;


    /* ---- SHORTEST PATH ---- */

        queue<int> q;
        vector<int> dist(NUM_NODES, INF);
        vector<int> parent(NUM_NODES, -1);

        dist[startNode] = 0;
        q.push(startNode);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (u == endNode) break;

            for (auto &e : adj[u]) {
                if (dist[e.to] == INF) {
                    dist[e.to] = dist[u] + 1;
                    parent[e.to] = u;
                    q.push(e.to);
                }
            }
        }

        if (dist[endNode] == INF) {
            cout << "NoPath\n";
        } else {
            vector<int> path;
            for (int v = endNode; v != -1; v = parent[v])
                path.push_back(v);

            reverse(path.begin(), path.end());

            for (int i = 0; i < path.size(); i++)
                cout << path[i] << (i + 1 < path.size() ? " " : "");
            cout << endl;
        }
    }

    /* ---- SEARCH MODE ---- */
    else if (mode == 'F') {
        cin.ignore();
        string query;
        getline(cin, query);

        int res = bfsSearchByName(query);
        if (res == -1) cout << "NOT_FOUND\n";
        else cout << res << endl;
    }

    return 0;
}
