#include <stdio.h>
int main(void)
{
    int a = 10;
    int b = 21;
    int c = 48;

    if ((a > b) && (a > c))
    {
        printf("Largest number is %d\n", a);
    }
    else if ((b > a) && (b > c))
    {
        printf("Largest number is %d\n", b);
    }
    else
    {
        printf("Largest number is %d\n", c);
    }
    
    
    
    return 0;
}