#include <stdio.h>
#include <stdlib.h>

// Structure for Trie Node
typedef struct TrieNode {
    struct TrieNode* child[2];
} TrieNode;

// Helper function to create a new Trie Node
TrieNode* createNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->child[0] = NULL;
    node->child[1] = NULL;
    return node;
}

// Function to find the maximum XOR of two numbers in an array
int findMaximumXOR(int* nums, int numsSize) {
    TrieNode* root = createNode();

    // Insert all numbers into the Trie
    for (int k = 0; k < numsSize; k++) {
        int num = nums[k];
        TrieNode* curr = root;
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;
            if (curr->child[bit] == NULL) {
                curr->child[bit] = createNode();
            }
            curr = curr->child[bit];
        }
    }

    int maxXor = 0;

    // Search for the maximum XOR for each number
    for (int k = 0; k < numsSize; k++) {
        int num = nums[k];
        TrieNode* curr = root;
        int xorVal = 0;

        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;
            int opposite = 1 - bit;

            if (curr->child[opposite] != NULL) {
                xorVal |= (1U << i); // Using unsigned bit shift to avoid overflow
                curr = curr->child[opposite];
            } else {
                curr = curr->child[bit];
            }
        }

        if (xorVal > maxXor) {
            maxXor = xorVal;
        }
    }

    return maxXor;
}
