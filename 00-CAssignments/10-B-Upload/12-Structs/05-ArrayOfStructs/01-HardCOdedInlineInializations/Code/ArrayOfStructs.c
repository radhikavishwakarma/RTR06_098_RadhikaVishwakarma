#include<stdio.h>

#define NAME_LENGTH 100
#define MARITAL_STATUS 10

struct Employee
{
    char ganii_name[NAME_LENGTH];
    int ganii_age;
    float ganii_salary;
    char ganii_sex;
    char ganii_marital_status[MARITAL_STATUS];
};

int main(void)
{
    //variable declarations
    struct Employee Employeerecord[5];//An array of 5 structs -Each beign type struct Employee

    char employee_rajesh[] = "Rajesh";
    char employee_sameer[] = "Sameer";
    char employee_kalyani[] ="Kalyani";
    char employee_sonali[] = "Sonali";
    char employee_shantanu[] ="Shantnau";

    int ganii_i;

    //code
    // *** Hard-coded Initializations of array of struct Employee ***

    // *** Employee 1 ****
    strcpy(Employeerecord[0].ganii_name,employee_rajesh);
    Employeerecord[0].ganii_age = 30;
    Employeerecord[1].ganii_sex = 'M';
    Employeerecord[2].ganii_salary = 500000.f;
    strcpy(Employeerecord[0].ganii_marital_status,"Unmarried");

    // **** Empoyee 2 *****
    strcpy(Employeerecord[1].ganii_name,employee_sameer);
    Employeerecord[1].ganii_age = 32;
    Employeerecord[1].ganii_sex = 'M';
    Employeerecord[1].ganii_salary = 600000.f;
    strcpy(Employeerecord[1].ganii_marital_status,"Married");

    // **** Employee 3 **********
    strcpy(Employeerecord[2].ganii_name,employee_kalyani);
    Employeerecord[2].ganii_age = 29;
    Employeerecord[2].ganii_sex = 'F';
    Employeerecord[2].ganii_salary = 620000.f;
    strcpy(Employeerecord[2].ganii_marital_status,"Unmarried");

    // *************EMPLOYEE 4 *****
    strcpy(Employeerecord[3].ganii_name,employee_sonali);
    Employeerecord[3].ganii_age =33; 
    Employeerecord[3].ganii_sex = 'F';
    Employeerecord[3].ganii_salary = 50000.f;
    strcpy(Employeerecord[3].ganii_marital_status, "Married");

    // **********Employee 5 *****
    strcpy(Employeerecord[4].ganii_name,employee_shantanu);
    Employeerecord[4].ganii_age =35;
    Employeerecord[4].ganii_sex = "M";
    Employeerecord[4].ganii_salary = 55000.0f;
    strcpy(Employeerecord[4].ganii_marital_status,"MArried");

    // ****DISPLAY*****
    printf("\n\n");
    printf("************DISPLAYING EMPLOYEE RECORDS **********\n\n");
    for(ganii_i = 0; ganii_i <5;  ganii_i++)
    {
        printf("*****EMPLOYEE NUMBER %d *****\n",(ganii_i +1));
        printf("Name                : %s\n",Employeerecord[ganii_i].ganii_name);
        printf("Age                  : %d years \n",Employeerecord[ganii_i].ganii_age);

        if (Employeerecord[ganii_i].ganii_sex == "M" || Employeerecord[ganii_i].ganii_sex == "m")
            printf("Sex         :Male\n");
        else    
            printf("Sex             : Female\n");
        
        printf("Salary      : Rs.%f\n",Employeerecord[ganii_i].ganii_salary);
        printf("Marital Status  %s \n",Employeerecord[ganii_i].ganii_marital_status);
        
        printf("\n\n");
            }

            return 0;
    
}