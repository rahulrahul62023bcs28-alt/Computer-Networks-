#include <string.h>

int countPalindromicSubsequence(char* s) {
    int first[26], last[26];
    
    // Initialize first and last arrays with -1
    for (int i = 0; i < 26; i++) {
        first[i] = -1;
        last[i] = -1;
    }

    int n = strlen(s);

    // Record the first and last occurrence of each character
    for (int i = 0; i < n; i++) {
        int char_code = s[i] - 'a';
        if (first[char_code] == -1) {
            first[char_code] = i;
        }
        last[char_code] = i;
    }

    int total_palindromes = 0;

    // Check middle characters for each character from 'a' to 'z'
    for (int c = 0; c < 26; c++) {
        if (first[c] != -1 && last[c] > first[c] + 1) {
            int seen[26] = {0}; // Track unique characters in between
            
            for (int i = first[c] + 1; i < last[c]; i++) {
                seen[s[i] - 'a'] = 1;
            }

            // Count unique middle characters
            for (int i = 0; i < 26; i++) {
                if (seen[i]) {
                    total_palindromes++;
                }
            }
        }
    }

    return total_palindromes;
}
