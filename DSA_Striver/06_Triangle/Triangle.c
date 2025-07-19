#include <stdio.h>
int main(void)
{
    int a = 5;
    int b = 6;
    int c = 10;

    if (((a + b) > c) && ((b + c) > a) && ((c + a) > b))
    {
        printf("Is a triangle");
    }

    else
    {
        printf("not a triangle");
    }
    
        
   return 0; 
}