#include<stdio.h>

int main(void)
{
    //variablde declarations
    int iArray[10];
    int i;

    for (i = 0; i < 10; i++)
        iArray[i] = (i + 1) * 3;
    printf("\n\n");
    printf("Elements of the Intger Array  :\n\n");
    for (i = 0; i < 10; i++)
        printf("iArray[%d] : = %d\n", i, iArray[i]);

    printf("\n\n");
    printf("ELements of the Intger Array  : \n\n");
    for (i = 0; i < 10; i++)
        printf("'iArray[%d] = %d\t\t Address = %p\n'", i, iArray[i], &iArray[i]);


    return (0);

}
