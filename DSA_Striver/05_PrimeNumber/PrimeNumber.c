#include <stdio.h>
#include <stdbool.h>
int main(void)
{
    // variable declaration
    int num = 50;

    // function declaration
    bool isPrime(int);
    
    for (int i = 2; i <= num; i++)
    {
        // check if current number is prime
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
        // if n is divisival by i then n is not prime number
        if (n % i == 0)
        {
            return false;
        }
        
    }   
        // if n is not divisable by i then n is prime number
            return true;
}