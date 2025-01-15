#include <stdio.h>
#include <string.h>

#define NAME_LENGTH 100
#define MARITAL_STATUS 10

struct Employee
{
    char name[NAME_LENGTH];
    int age;
    float salary;
    char sex;
    char marital_status[MARITAL_STATUS];
};

int main(void)
{
    // Variable declarations
    struct Employee Employeerecord[5]; // Array of 5 structs of type Employee
    int i;

    // Hardcoded Initializations of Employee Records
    // Employee 1
    strcpy(Employeerecord[0].name, "Raghav");
    Employeerecord[0].age = 30;
    Employeerecord[0].sex = 'M';
    Employeerecord[0].salary = 500000.f;
    strcpy(Employeerecord[0].marital_status, "Unmarried");

    // Employee 2
    strcpy(Employeerecord[1].name, "Prathamesh");
    Employeerecord[1].age = 32;
    Employeerecord[1].sex = 'M';
    Employeerecord[1].salary = 600000.f;
    strcpy(Employeerecord[1].marital_status, "Married");

    // Employee 3
    strcpy(Employeerecord[2].name, "Vipul");
    Employeerecord[2].age = 29;
    Employeerecord[2].sex = 'M';
    Employeerecord[2].salary = 620000.f;
    strcpy(Employeerecord[2].marital_status, "Unmarried");

    // Employee 4
    strcpy(Employeerecord[3].name, "Sandeep");
    Employeerecord[3].age = 33;
    Employeerecord[3].sex = 'M';
    Employeerecord[3].salary = 50000.f;
    strcpy(Employeerecord[3].marital_status, "Married");

    // Employee 5
    strcpy(Employeerecord[4].name, "Sanket");
    Employeerecord[4].age = 35;
    Employeerecord[4].sex = 'M';
    Employeerecord[4].salary = 55000.f;
    strcpy(Employeerecord[4].marital_status, "Married");

    // Displaying Employee Records
    printf("\n\n************ DISPLAYING EMPLOYEE RECORDS **********\n\n");
    for (i = 0; i < 5; i++)
    {
        printf("***** EMPLOYEE NUMBER %d *****\n", (i + 1));
        printf("Name              : %s\n", Employeerecord[i].name);
        printf("Age               : %d years\n", Employeerecord[i].age);

        // Corrected string comparison for sex
        if (Employeerecord[i].sex == 'M' || Employeerecord[i].sex == 'm')
            printf("Sex               : Male\n");
        else
            printf("Sex               : Female\n");

        printf("Salary            : Rs. %.2f\n", Employeerecord[i].salary);
        printf("Marital Status    : %s\n", Employeerecord[i].marital_status);
        printf("\n");
    }

    return (0);
}
