#include <stdio.h>
#include <stdbool.h>
int main()
{
    int num = 20;

    bool isPrime(int);

    for (int i = 2; i <= num; i++)
    {
        if (isPrime(i))
        {
            printf("%d ", i);
        }
        
    }
    return 0;
}

bool isPrime(int n)
{
    for (int i = 2; i < n; i++)
    {
        if (n % i == 0)
        {
            return false;
        }
        
    }
    return true;
    
}