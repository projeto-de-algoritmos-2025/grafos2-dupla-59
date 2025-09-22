#include <vector>
#include <queue>
#include <climits>
#include <cmath>    // std::abs
using namespace std;

class Solution {
public:
    int minimumEffortPath(vector<vector<int>>& heights) {
        int R = heights.size();
        int C = heights[0].size();

        // dist[x][y] = menor esforço máximo para chegar em (x,y)
        vector<vector<int>> dist(R, vector<int>(C, INT_MAX));
        dist[0][0] = 0;

        // Min-heap: (esforco, x, y)
        priority_queue<
            tuple<int,int,int>,
            vector<tuple<int,int,int>>,
            greater<tuple<int,int,int>>
        > pq;

        pq.push({0, 0, 0});

        // 4 vizinhos
        int dx[4] = {1, -1, 0, 0};
        int dy[4] = {0, 0, 1, -1};

        while (!pq.empty()) {
            auto [e, x, y] = pq.top();
            pq.pop();

            // Entrada obsoleta
            if (e > dist[x][y]) continue;

            // Chegou no destino
            if (x == R - 1 && y == C - 1) return e;

            for (int k = 0; k < 4; k++) {
                int nx = x + dx[k];
                int ny = y + dy[k];
                if (nx < 0 || nx >= R || ny < 0 || ny >= C) continue;

                int step = abs(heights[nx][ny] - heights[x][y]);
                int ne = max(e, step);   // esforço do caminho até o vizinho

                if (ne < dist[nx][ny]) {
                    dist[nx][ny] = ne;
                    pq.push({ne, nx, ny});
                }
            }
        }
        return 0; // grid não vazio garante retorno antes
    }
};
