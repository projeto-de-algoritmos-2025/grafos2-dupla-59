# Exercícios de Grafos 2

**Número da Lista**: 2
**Conteúdo da Disciplina**: FGA0124 - PROJETO DE ALGORITMOS  

## Alunos

<div align = "center">
<table>
  <tr>
    <td align="center"><a href="https://github.com/thalesgvl"><img style="border-radius: 50%;" src="https://github.com/thalesgvl.png" width="190;" alt=""/><br /><sub><b>Thales Germano Vargas Lima</b></sub></a></td>
    <td align="center"><a href="https://github.com/vcpVitor"><img style="border-radius: 50%;" src="https://github.com/vcpVitor.png" width="190;" alt=""/><br /><sub><b>Vitor Carvalho Pereira</b></sub></a></td>
  </tr>
</table>

| Matrícula       | Aluno              |
| --------------- | ------------------ |
| 20/2017147 | Thales Germano Vargas Lima |
| 21/1062615 | Vitor Carvalho Pereira |
</div>

## Sobre 
A atividade foi baseada na resolução de desafios de programação da plataforma LeetCode.  
Foram selecionados **3 exercícios** de grafos que exploram o algoritmo de **Dijkstra** e suas variações, em diferentes níveis de dificuldade (médio e difícil).

## Exercícios 

---

### [743. Network Delay Time](https://leetcode.com/problems/network-delay-time/) – Medium

Dado um grafo dirigido e ponderado, calcule o tempo mínimo para que um sinal enviado a partir de um nó `k` chegue a todos os outros nós. Se algum nó não for alcançável, retorne `-1`.

**Ideia**

- Construir lista de adjacência.
- Rodar **Dijkstra clássico** a partir de `k`.
- A resposta é o maior valor entre as distâncias calculadas.

**Código em C++ (nível médio)**
```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        vector<vector<pair<int,int>>> adj(n + 1);
        for (auto &e : times) {
            int u = e[0], v = e[1], w = e[2];
            adj[u].push_back({v, w});
        }

        vector<int> dist(n + 1, INT_MAX);
        dist[k] = 0;

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

        int ans = 0;
        for (int i = 1; i <= n; i++) {
            if (dist[i] == INT_MAX) return -1;
            ans = max(ans, dist[i]);
        }
        return ans;
    }
};
```

---

### [1631. Path With Minimum Effort](https://leetcode.com/problems/path-with-minimum-effort/) – Medium

Em uma grade de alturas, defina o custo de uma aresta como a diferença absoluta entre duas células adjacentes. O custo de um caminho é o **máximo** desses valores. O objetivo é encontrar o caminho de menor esforço.

**Ideia**

- Adaptar o **Dijkstra** para armazenar o **esforço máximo** até cada célula.
- Atualização: `novo = max(atual, abs(altura[nxt]-altura[cur]))`.

**Código em C++ (nível médio)**
```cpp
#include <vector>
#include <queue>
#include <climits>
#include <cmath>
using namespace std;

class Solution {
public:
    int minimumEffortPath(vector<vector<int>>& heights) {
        int R = heights.size();
        int C = heights[0].size();

        vector<vector<int>> dist(R, vector<int>(C, INT_MAX));
        dist[0][0] = 0;

        priority_queue<
            tuple<int,int,int>,
            vector<tuple<int,int,int>>,
            greater<tuple<int,int,int>>
        > pq;

        pq.push({0, 0, 0});
        int dx[4] = {1, -1, 0, 0};
        int dy[4] = {0, 0, 1, -1};

        while (!pq.empty()) {
            auto [e, x, y] = pq.top();
            pq.pop();

            if (e > dist[x][y]) continue;
            if (x == R - 1 && y == C - 1) return e;

            for (int k = 0; k < 4; k++) {
                int nx = x + dx[k];
                int ny = y + dy[k];
                if (nx < 0 || nx >= R || ny < 0 || ny >= C) continue;

                int step = abs(heights[nx][ny] - heights[x][y]);
                int ne = max(e, step);

                if (ne < dist[nx][ny]) {
                    dist[nx][ny] = ne;
                    pq.push({ne, nx, ny});
                }
            }
        }
        return 0;
    }
};
```

---

### [2045. Second Minimum Time to Reach Destination](https://leetcode.com/problems/second-minimum-time-to-reach-destination/) – Hard

Em um grafo não-direcionado com arestas de tempo fixo `time` e semáforos que alternam entre verde/vermelho a cada `change` minutos, calcule o **segundo menor tempo** para ir de `1` até `n`.

**Ideia**

- Usar **Dijkstra modificado** mantendo até **duas melhores chegadas** em cada nó.
- Considerar a espera de semáforo: se `(t/change) % 2 == 1`, precisa esperar.
- O resultado é o tempo da segunda vez que o nó `n` é processado.

**Código em C++ (nível médio)**
```cpp
#include <vector>
#include <queue>
using namespace std;

class Solution {
public:
    int secondMinimum(int n, vector<vector<int>>& edges, int time, int change) {
        vector<vector<int>> adj(n + 1);
        for (auto &e : edges) {
            int u = e[0], v = e[1];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
        pq.push({0, 1});
        vector<int> poppedCount(n + 1, 0);

        while (!pq.empty()) {
            auto [t, u] = pq.top();
            pq.pop();

            if (poppedCount[u] >= 2) continue;
            poppedCount[u]++;

            if (u == n && poppedCount[u] == 2) return t;

            int wait = 0;
            int k = t / change;
            if (k % 2 == 1) {
                wait = (k + 1) * change - t;
            }

            int depart = t + wait;
            int arrive = depart + time;

            for (int v : adj[u]) {
                if (poppedCount[v] < 2) {
                    pq.push({arrive, v});
                }
            }
        }
        return -1;
    }
};
```

---

## Apresentação 

<div align="center">
<a href="https://youtu.be/jqgfT8PXio4"><img src="./imagens/image.png" width="50%"></a>

<font size="3"><p style="text-align: center">Autores: [Thales Germano Vargas Lima](https://github.com/thalesgvl) & [Vitor Carvalho Pereira](https://github.com/vcpVitor)</p></font>
</div>

