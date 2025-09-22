#include <vector>
#include <queue>
#include <climits>
using namespace std;

class Solution {
public:
    int secondMinimum(int n, vector<vector<int>>& edges, int time, int change) {
        // Grafo não-direcionado
        vector<vector<int>> adj(n + 1);
        for (auto &e : edges) {
            int u = e[0], v = e[1];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // Min-heap por tempo: (tempoAtual, nó)
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
        pq.push({0, 1});

        // Contagem de "pop" (chegadas confirmadas) por nó.
        // Precisamos da 2ª chegada ao nó n.
        vector<int> poppedCount(n + 1, 0);

        while (!pq.empty()) {
            auto [t, u] = pq.top();
            pq.pop();

            // Se já confirmamos 2 chegadas neste nó, ignore
            if (poppedCount[u] >= 2) continue;

            poppedCount[u]++;

            // Se esta é a 2ª chegada no destino, retornamos
            if (u == n && poppedCount[u] == 2) return t;

            // Calcular espera de semáforo antes de sair de u
            int wait = 0;
            int k = t / change;
            // ciclos: 0..(change-1) verde, change..(2*change-1) vermelho, e assim por diante
            if (k % 2 == 1) {
                // estamos em janela vermelha; esperar até próximo múltiplo de 'change'
                wait = (k + 1) * change - t;
            }

            int depart = t + wait;
            int arriveBase = depart + time;

            // Relaxar vizinhos (mas só enfileirar se v ainda não teve 2 chegadas confirmadas)
            for (int v : adj[u]) {
                if (poppedCount[v] < 2) {
                    pq.push({arriveBase, v});
                }
            }
        }

        // Problema garante existência; retorno simbólico
        return -1;
    }
};
