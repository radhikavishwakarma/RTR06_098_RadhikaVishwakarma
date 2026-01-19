#include <stdio.h>
int main()
{
    int n = 5;
    int result = 1;

    for (int i = 1; i <= n; i++)
    {
        result = result * i;
        
    }

    printf("Factorial is : %d", result);
    
    return 0;
}