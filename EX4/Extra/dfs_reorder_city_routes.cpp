#include <iostream>
#include <vector>

class Solution {
public:
    int minReorder(int n, const std::vector<std::vector<int>>& connections) {
        std::vector<std::vector<std::pair<int, int>>> adj(n);
        for (const auto& conn : connections) {
            int u = conn[0];
            int v = conn[1];
            adj[u].emplace_back(v, 1);
            adj[v].emplace_back(u, 0);
        }

        std::vector<bool> visited(n, false);
        return dfs(0, adj, visited);
    }

private:
    int dfs(int node, const std::vector<std::vector<std::pair<int, int>>>& adj, std::vector<bool>& visited) {
        visited[node] = true;
        int changes = 0;

        for (const auto& [next, direction] : adj[node]) {
            if (!visited[next]) {
                changes += direction + dfs(next, adj, visited);
            }
        }
        return changes;
    }
};
