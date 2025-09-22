#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        // Lista de adjacência: (vizinho, peso)
        vector<vector<pair<int,int>>> adj(n + 1);
        for (auto &e : times) {
            int u = e[0], v = e[1], w = e[2];
            adj[u].push_back({v, w});
        }

        // Distâncias (inicialmente infinito)
        vector<int> dist(n + 1, INT_MAX);
        dist[k] = 0;

        // Min-heap: (dist, nó)
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
        pq.push({0, k});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d > dist[u]) continue;

            for (auto [v, w] : adj[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }

        // Pegar o maior tempo
        int ans = 0;
        for (int i = 1; i <= n; i++) {
            if (dist[i] == INT_MAX) return -1;
            ans = max(ans, dist[i]);
        }
        return ans;
    }
};
