#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

class Solution {
public:
    int findCheapestPrice(int n, const std::vector<std::vector<int>>& flights, int src, int dst, int k) {
        std::vector<int> cost(n, INT_MAX);
        cost[src] = 0;

        for (int i = 0; i <= k; ++i) {
            std::vector<int> temp = cost;
            for (const auto& flight : flights) {
                int u = flight[0];
                int v = flight[1];
                int price = flight[2];

                if (cost[u] != INT_MAX && cost[u] + price < temp[v]) {
                    temp[v] = cost[u] + price;
                }
            }
            cost = std::move(temp);
        }

        return cost[dst] == INT_MAX ? -1 : cost[dst];
    }
};
