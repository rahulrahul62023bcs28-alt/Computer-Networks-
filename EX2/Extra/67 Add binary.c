#include <string.h>
#include <stdlib.h>

char* addBinary(char* a, char* b) 
{
    int lenA = strlen(a);
    int lenB = strlen(b);
    int i = lenA - 1;
    int j = lenB - 1;
    int c = 0;
    
    int maxLen = (lenA > lenB) ? lenA : lenB;
    
    // Allocate space for maxLen + 1 digits + 1 null terminator
    char* result = (char*)malloc((maxLen + 2) * sizeof(char));
    int p = maxLen + 1;
    result[p] = '\0'; // Set the null terminator at the end
    p--;

    while (i >= 0 || j >= 0) 
    {
        int x = (i >= 0) ? a[i--] - '0' : 0;
        int y = (j >= 0) ? b[j--] - '0' : 0;
        int z = x + y + c;
        result[p--] = (z % 2) + '0'; 
        c = z / 2;
    }
    
    if (c == 1) 
    {
        result[p] = '1';
        // Return string starting from the carry position
        return &result[p];
    }
    
    // Return string skipping the unused leading space
    return &result[p + 1];
}
