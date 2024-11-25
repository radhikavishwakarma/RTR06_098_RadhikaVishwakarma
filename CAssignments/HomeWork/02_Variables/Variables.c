#include <stdio.h>

int main(void)
{
    // variable declaration
    int i = 10;
    float f = 91.5f;
    double d = 99.654125;
    char c = 'L';
    
    // code
    printf("\n\n");
    printf("-----------------------\n");

    printf("i = %d\n", i);
    printf("f = %f\n", f);
    printf("d = %lf\n", d);
    printf("c = %c\n", c);

    printf("\n\n");
    printf("-----------------------\n");

    i = 28;
    f = 99.15f;
    d = 82.5684789;
    c = 'R';

    printf("i = %d\n", i);
    printf("f = %f\n", f);
    printf("d = %lf\n", d);
    printf("c = %c\n", c);

    printf("\n\n");
    printf("-----------------------\n");

    return(0);    
}