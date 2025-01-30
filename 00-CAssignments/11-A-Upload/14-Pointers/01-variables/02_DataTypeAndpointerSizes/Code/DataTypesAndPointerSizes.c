#include<stdio.h>

struct Employee
{
    char ganii_name[100];
    int ganii_age;
    float ganii_salary;
    char ganii_sex;
    char ganiii_marital_status;
};

int main(void)
{
    //code
    printf("\n\n");
    printf("SIZES OF DATA TYPES AND POINTERS TO THOSE RESPECTIVE DATA TYPES ARE :\n\n");
    printf("Size of (int)   :%zd\t \t\t Size of pointer to inte (int*)   : %zd \n\n",sizeof(int),sizeof(int*));
    printf("Size of (float)     :%zd\t\t\t  Size of of pointer to float (float*) : %zd\n\n",sizeof(float),sizeof(float*));
    printf("Size of (double     :%zd\t\t\t Size of pointer to double (double*) : %zd\n\n",sizeof(double),sizeof(double*));;
    printf("Size of (char)      :%zd \t\t\t Size of pointer to char (char*)  :%zd\n\n",sizeof(char),sizeof(char*));
    printf("Size of(struct Employee) :%zd \t\t   Size of pointer to struct Employee(struct Employee*) : %zd\n\n",sizeof(struct Employee),sizeof(struct Employee*));

    return 0;

}