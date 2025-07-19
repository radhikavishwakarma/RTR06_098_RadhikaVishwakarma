#include <stdio.h>
int main()
{
    int n = 5;
    int Array1[] = {2, 4, 7, 9, 8};
    int Array2[n];
    
    printf("printing array 1: \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", Array1[i]);
    }

    // Reversing array logic
    for (int i = n; i >= 0; i--)
    {
        Array2[n - i - 1] = Array1[i];
    }
    
    printf("\nprinting reversed array 1: \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", Array2[i]);
    }

    return 0;
}