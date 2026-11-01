#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

class Solution {
public:
    int numBusesToDestination(const std::vector<std::vector<int>>& routes, int source, int target) {
        if (source == target) return 0;

        std::unordered_map<int, std::vector<int>> stopToRoutes;
        for (int i = 0; i < static_cast<int>(routes.size()); ++i) {
            for (int stop : routes[i]) {
                stopToRoutes[stop].push_back(i);
            }
        }

        if (stopToRoutes.find(source) == stopToRoutes.end() || 
            stopToRoutes.find(target) == stopToRoutes.end()) {
            return -1;
        }

        std::queue<int> q;
        std::unordered_set<int> visitedStops;
        std::vector<bool> visitedBuses(routes.size(), false);

        q.push(source);
        visitedStops.insert(source);
        int busesTaken = 0;

        while (!q.empty()) {
            int size = q.size();
            busesTaken++;

            for (int i = 0; i < size; ++i) {
                int currStop = q.front();
                q.pop();

                for (int busId : stopToRoutes[currStop]) {
                    if (visitedBuses[busId]) continue;
                    visitedBuses[busId] = true;

                    for (int nextStop : routes[busId]) {
                        if (nextStop == target) return busesTaken;
                        if (visitedStops.find(nextStop) == visitedStops.end()) {
                            visitedStops.insert(nextStop);
                            q.push(nextStop);
                        }
                    }
                }
            }
        }
        return -1;
    }
};
