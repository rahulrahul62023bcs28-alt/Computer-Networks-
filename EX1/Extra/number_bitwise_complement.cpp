#include <iostream>

class Solution {
public:
    int findComplement(int num) {
        unsigned int mask = 1;
        while (mask < static_cast<unsigned int>(num)) {
            mask = (mask << 1) | 1;
        }
        return mask ^ num;
    }
};
