#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

class Solution {
public:
    int networkDelayTime(const std::vector<std::vector<int>>& times, int n, int k) {
        std::vector<std::vector<std::pair<int, int>>> graph(n + 1);
        for (const auto& t : times) {
            graph[t[0]].emplace_back(t[1], t[2]);
        }

        std::priority_queue<std::pair<int, int>, 
                            std::vector<std::pair<int, int>>, 
                            std::greater<std::pair<int, int>>> pq;
        std::vector<int> dist(n + 1, INT_MAX);

        dist[k] = 0;
        pq.emplace(0, k);

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d > dist[u]) continue;

            for (const auto& [v, weight] : graph[u]) {
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.emplace(dist[v], v);
                }
            }
        }

        int maxDelay = 0;
        for (int i = 1; i <= n; ++i) {
            if (dist[i] == INT_MAX) return -1;
            maxDelay = std::max(maxDelay, dist[i]);
        }
        return maxDelay;
    }
};
