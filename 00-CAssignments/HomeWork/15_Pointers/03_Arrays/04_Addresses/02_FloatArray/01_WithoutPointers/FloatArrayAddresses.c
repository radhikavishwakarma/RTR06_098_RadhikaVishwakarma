#include<stdio.h>

int main(void)
{
    //variablde declarations
    float fArray[10];
    int i;

    //code
    for (i = 0; i < 10; i++)
        fArray[i] = (float)(i + 1) * 1.5f;

    printf("\n\n");
    printf("Elements of the float Array  : \n\n");
    for (i = 0; i < 10; i++)
        printf("fArray[%d] = %f\n", i, fArray[i]);

    printf("\n\n");
    printf("Elements of the float Array  : \n\n");
    for (i = 0; i < 10; i++)
        printf("fArray[%d] = %f \t \t Adddress= %p\n", i, fArray[i], &fArray[i]);
    printf("\n\n");

    return (0);
}
