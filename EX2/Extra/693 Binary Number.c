#include <stdbool.h>

bool hasAlternatingBits(int n) 
{
    int x = n ^ (n >> 1);
    return (x & ((long)x + 1)) == 0;
}
