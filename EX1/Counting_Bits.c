#include <stdlib.h>

int* countBits(int n, int* returnSize)
{
    *returnSize = n + 1;

    int *ans = (int *)malloc((n + 1) * sizeof(int));

    for (int i = 0; i <= n; i++)
    {
        int temp = i;
        int count = 0;

        while (temp != 0)
        {
            if (temp % 2 == 1)
                count++;

            temp = temp / 2;
        }

        ans[i] = count;
    }

    return ans;
}
