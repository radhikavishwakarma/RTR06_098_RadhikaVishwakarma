#include<stdio.h>

int main(void)
{
    // variable declaration
    float f;
    float f_num = 1.7f;  // simply change this value only to get different outputs...

    // code
    printf("\n\n");
    printf("Printing numbers %f o %f :\n\n", f, (f_num * 10.f));

    f = f_num;
    while (f <= (f_num * 10.f))
    {
        printf("\t%f\n", f);
        f = f + f_num;
    }
 
    printf("\n\n");

    return 0;
}