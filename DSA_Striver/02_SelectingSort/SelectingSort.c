#include <stdio.h>

int main(void)
{
    int minIndex;

    int A[] = {21, 14, 33, 5, 11};

    int intSize = sizeof(int);
    int arraySize = sizeof(A);
    int numberOfElements = arraySize/intSize;

    printf("integer size: %d\n", intSize);
    printf("array size: %d\n", arraySize);
    printf("number of elements: %d\n", arraySize/intSize);

    for (int i = 0; i < numberOfElements-1; i++)
    {
        minIndex = i;
        for (int j = 0; j < numberOfElements; j++)
        {
            if (A[j] < A[minIndex])
            {
                minIndex = j;
            }
            
        }
        
    }
    printf("MiniIndex = %d", minIndex);

    
    return 0;
}