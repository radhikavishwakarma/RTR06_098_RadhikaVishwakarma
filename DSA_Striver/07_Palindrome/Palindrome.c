#include <stdio.h>
int main(void)
{
    int n = 5;
    int Array1[5] = {2, 4, 7, 9, 8};
    int Array2[n];

    printf("Printing number: \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", Array1[i]);
    }

    // Reversing array logic
    for (int i = n; i >= 0; i--)
    {
        Array2[n - i - 1] = Array1[i];
    }

    printf("\nPrinting reversed number: \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", Array2[i]);
    }
    // compare arrary1 and array2
    for (int i = 0; i < n; i++)
    {
        if (Array1[i] == Array2[i])
        {
            continue;
        }
        else
        {
            printf("not palindrome");
            break;
        }
    }
    printf("Is palindrome");
    

    return 0;
    
    
}