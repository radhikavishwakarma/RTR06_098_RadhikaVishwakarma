#include <stdio.h>

struct Employee
{
    char name[100];
    int age;
    float salary;
    char sex;
    char marital_status;
};

int main(void)
{
    // Code
    printf("\n\n");
    printf("Sizes of Data Types and Pointers to Those Respective Data Types:\n\n");

    printf("Size of (int): %zu \t\t size of Pointer to int (int*): %zu\n\n", sizeof(int), sizeof(int*));
    printf("Size of (float): %zu \t\t size of Pointer to float (float*): %zu\n\n", sizeof(float), sizeof(float*));
    printf("Size of (double): %zu \t size of Pointer to double (double*): %zu\n\n", sizeof(double), sizeof(double*));
    printf("Size of (char): %zu \t\t size of Pointer to char (char*): %zu\n\n", sizeof(char), sizeof(char*));
    printf("Size of (struct Employee): %zu \t size of Pointer to struct Employee (struct Employee*): %zu\n\n", sizeof(struct Employee), sizeof(struct Employee*));

    return 0;
}
