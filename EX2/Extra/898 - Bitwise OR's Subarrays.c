#include <stdlib.h>

int compare(const void* a, const void* b) 
{
    return (*(int*)a - *(int*)b);
}

int subarrayBitwiseORs(int* arr, int arrSize) 
{
    int* allResults = (int*)malloc(arrSize * 32 * sizeof(int));
    int resultCount = 0;

    int prev[32];
    int prevSize = 0;

    for (int i = 0; i < arrSize; i++) 
    {
        int num = arr[i];
        int curr[32];
        int currSize = 0;

        curr[currSize++] = num;
        allResults[resultCount++] = num;

        for (int j = 0; j < prevSize; j++) 
        {
            int orValue = prev[j] | num;
            int isDuplicate = 0;
            
            for (int k = 0; k < currSize; k++) 
            {
                if (curr[k] == orValue) 
                {
                    isDuplicate = 1;
                    break;
                }
            }
            
            if (!isDuplicate) 
            {
                curr[currSize++] = orValue;
                allResults[resultCount++] = orValue;
            }
        }

        prevSize = currSize;
        for (int j = 0; j < currSize; j++) 
        {
            prev[j] = curr[j];
        }
    }

    if (resultCount == 0) 
    {
        free(allResults);
        return 0;
    }

    qsort(allResults, resultCount, sizeof(int), compare);

    int uniqueCount = 1;
    for (int i = 1; i < resultCount; i++) 
    {
        if (allResults[i] != allResults[i - 1]) 
        {
            uniqueCount++;
        }
    }

    free(allResults);
    return uniqueCount;
}
