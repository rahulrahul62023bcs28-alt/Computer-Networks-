#include <stdio.h>
#include <string.h>

bool hasMatch(char* s, char* p) {
    int s_len = strlen(s);
    int p_len = strlen(p);
    
    // Find the index of '*' in p
    int star_idx = -1;
    for (int i = 0; i < p_len; i++) {
        if (p[i] == '*') {
            star_idx = i;
            break;
        }
    }
    
    int L1 = star_idx;                   // Length of prefix
    int L2 = p_len - 1 - star_idx;      // Length of suffix
    
    // Step 1: Find the earliest occurrence of prefix in s
    int i_first = -1;
    if (L1 == 0) {
        i_first = 0; // Empty prefix can start at index 0
    } else {
        for (int i = 0; i <= s_len - L1; i++) {
            if (strncmp(s + i, p, L1) == 0) {
                i_first = i;
                break;
            }
        }
    }
    
    // If prefix is not found, return false
    if (i_first == -1) {
        return false;
    }
    
    // Step 2: Find the latest occurrence of suffix in s
    int j_last = -1;
    if (L2 == 0) {
        j_last = s_len; // Empty suffix can end at index s_len
    } else {
        for (int j = s_len - L2; j >= 0; j--) {
            if (strncmp(s + j, p + star_idx + 1, L2) == 0) {
                j_last = j;
                break;
            }
        }
    }
    
    // If suffix is not found, return false
    if (j_last == -1) {
        return false;
    }
    
    // Step 3: Ensure suffix starts after or right at the end of prefix
    return j_last >= i_first + L1;
}
