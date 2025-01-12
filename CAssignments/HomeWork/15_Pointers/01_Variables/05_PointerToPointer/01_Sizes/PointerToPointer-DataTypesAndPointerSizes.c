#include<stdio.h>

struct Employee
{
    char name[100];
    int age;
    char sex;
    char mariatal_status;
};

int main(void)
{
    //code
    printf("\n\n");
    printf("Sizes of data types and pointes to those respective dat types are :\\n");
    printf("Size pf (int)   : %zd \t \t \t  szie of pointer to int (int*)  : %zd \t \t \t Size of pointer to pointer to int (int**)   : %zd\n\n", sizeof(int), sizeof(int*), sizeof(int**));

    printf("Size of (float) : %zd \t \t \t  Size of pointer to float (float*)  : %zd \t \t \t Size of pointer to pointer to float (float**)     : %zd \n\n", sizeof(float), sizeof(float*), sizeof(float**));

    printf("Size of (double) : %zd \t \t \t  Size of pointer to double   (doule*)    :%zd \t \t \t Size of pointer to pointer double (double**)   :%zd \n\n", sizeof(double), sizeof(double*), sizeof(double**));

    printf("Size of (char) : %zd \t \t \t Size of pointer to char (char*)    :%zd \t \t\t Size of pointer to pointer to char (char**) : %zd\n\n", sizeof(char), sizeof(char*), sizeof(char**));

    printf("Size of (struct Employee) : %zd \t \t  Size of pointer to struct Employee (struct Employee*) : %zd \t \t \t Size of pointer to pointer to struct Employee (struct Employe**)", sizeof(struct Employee), sizeof(struct Employe*), sizeof(struct Employe**));


    return (0);
}
