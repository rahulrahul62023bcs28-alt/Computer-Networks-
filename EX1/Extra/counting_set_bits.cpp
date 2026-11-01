#include <iostream>
#include <vector>

class Solution {
public:
    std::vector<int> countBits(int n) {
        std::vector<int> a(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            int c = 0, j = i;
            while (j > 0) {
                j = j & (j - 1);
                c++;
            }
            a[i] = c;
        }
        return a;
    }
};
