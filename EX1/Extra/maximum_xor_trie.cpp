#include <iostream>
#include <vector>
#include <algorithm>

class Solution {
    struct TrieNode {
        TrieNode* child[2];
        TrieNode() {
            child[0] = nullptr;
            child[1] = nullptr;
        }
    };

public:
    int findMaximumXOR(const std::vector<int>& nums) {
        TrieNode* root = new TrieNode();
        for (int num : nums) {
            TrieNode* curr = root;
            for (int i = 31; i >= 0; --i) {
                int bit = (num >> i) & 1;
                if (!curr->child[bit]) {
                    curr->child[bit] = new TrieNode();
                }
                curr = curr->child[bit];
            }
        }

        int maxXor = 0;
        for (int num : nums) {
            TrieNode* curr = root;
            int xorVal = 0;
            for (int i = 31; i >= 0; --i) {
                int bit = (num >> i) & 1;
                int opposite = 1 - bit;
                if (curr->child[opposite]) {
                    xorVal |= (1 << i);
                    curr = curr->child[opposite];
                } else {
                    curr = curr->child[bit];
                }
            }
            maxXor = std::max(maxXor, xorVal);
        }
        return maxXor;
    }
};
