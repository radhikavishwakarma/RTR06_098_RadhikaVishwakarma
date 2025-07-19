#include <stdio.h>
int main(void)
{
    int a = 10;
    int b = 21;
    int c = 48;

    if (a > b)
    {
        if (a > c)
        {
            printf("%d", a);
        }
        else
        {
            printf("%d", c);
        }
    }
    else
    {
        if (b  > c)
        {
            printf("%d", b);
        }
        else
        {
            printf("%d", c);
        }
        
    }
    
    return 0;
}