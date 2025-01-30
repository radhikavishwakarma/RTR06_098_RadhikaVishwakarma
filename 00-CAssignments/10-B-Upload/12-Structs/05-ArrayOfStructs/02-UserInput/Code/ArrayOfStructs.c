#include<stdio.h>
#include<ctype.h>

#define NUM_EMPLOYEE 5 // Simply change this constant vaue to have as many number of employee Records As you Plase..
#define NAME_LENGTH 100
#define MARITAL_STATUS 10

struct Employee
{
    char ganii_name[NAME_LENGTH];
    int ganii_age;
    char ganii_sex;
    float ganii_salary;
    char ganii_marital_status;
};

int main(void)
{
    //function prottype
    void MyGetString(char[],int);

    // variablde declarations
    struct Employee EmployeeRecord[NUM_EMPLOYEE];// An array of Nume_Employee structs -Each Beigng type struct Employee
    int ganii_i ;

    // code
    //**** USER INPUT INITIALIZATION OF ARRAY OF  struct EMploye **** */
    for(ganii_i = 0; ganii_i < NUM_EMPLOYEE; ganii_i++)
    {
        printf("\n\n\n\n");
        printf("***********DATA ENTRY FOR EMPLOYEE NUMBER %d *******\n",(ganii_i+1));
        printf("\n\n");
        printf("ENter Employee Name : ");
        MyGetString(EmployeeRecord[ganii_i].ganii_name,NAME_LENGTH);

        printf("\n\n\n");
        printf("Enter Employee Age (in years) :");
        scanf("%d",&EmployeeRecord[ganii_i].ganii_age);

        printf("\n\n");
        scanf("Enter Employees Sex (M/m For Male,F/f For Female) :");
        EmployeeRecord[ganii_i].ganii_sex = getch();
        printf("%c",EmployeeRecord[ganii_i].ganii_sex);
        EmployeeRecord[ganii_i].ganii_sex = toupper(EmployeeRecord[ganii_i].ganii_sex);

        printf("\n\n\n");
        printf("ENter Employees Salary (in Indian Rupees) :");
        scanf("%f",&EmployeeRecord[ganii_i].ganii_salary);

        printf("\n\n");
        printf("Is The EMployee Married ? (Y/y For Yes,N/n For No) :");
        EmployeeRecord[ganii_i].ganii_marital_status = getch();
        printf("%c",EmployeeRecord[ganii_i].ganii_marital_status);
        EmployeeRecord[ganii_i].ganii_marital_status =toupper(EmployeeRecord[ganii_i].ganii_marital_status);

    }

    // ****DISPLAY*****
    printf("\n\n");
    printf("***********DISPLAYING EMPLOYEE RECORDS **************\n\n");
    for(ganii_i = 0; ganii_i < NUM_EMPLOYEE; ganii_i++)
    {
        printf("**********EMPLOYEE NUMBER %d **************\n\n",(ganii_i +1));
        printf("Name                : %s\n",EmployeeRecord[ganii_i].ganii_name);
        printf("Age                 : %d years \n",EmployeeRecord[ganii_i].ganii_age);

        if(EmployeeRecord[ganii_i].ganii_sex == 'M')
            printf("Sex         :Male\n");
        else if(EmployeeRecord[ganii_i].ganii_sex =='F')
            printf("Sex          : Female\n");
        else    
            printf("Sex     :Invalid Data Entered \n");
        
        printf("Salary  : Rs. %f\n",EmployeeRecord[ganii_i].ganii_salary);

        if(EmployeeRecord[ganii_i].ganii_marital_status == 'Y')
            printf("Marital Status :  Married\n");
        else if(EmployeeRecord[ganii_i].ganii_marital_status == 'N')
            printf("Maritall Status : Unmarried \n");
        else
            printf("Marital Status : Invalid Data Entred\n");
        printf("\n\n");

    }
    return 0;
}

// SIMPLY REDUMENTARY IMPLEMEMTATION OF gets_s()**
// IMPLEMENTED DUE TO THE DIFFERENT BEHAVIOUR OF gets_s()/fgets()/sdcnaf() ON DIFFERENT PLAFORMS**
// **BACKSPACE /CHARACTER DELETION AND ARROW KEY CURSOR MOVEMENT NOT IMPLEMENTD **

void MyGetString(char str[],int str_size)
{
    //variable declarations
    int ganii_i;
    char ganii_ch ='\0';

    // code
    ganii_i = 0;
    do
    {
        /* code */
        ganii_ch = getch();
        str[ganii_i] = ganii_ch;
        printf("%c",str[ganii_i]);
        ganii_i++;
    } while (((ganii_ch != '\r') && (ganii_i <str_size)));

    if(ganii_i == str_size)
        str[ganii_i -1] = '\0';
    else
        str[ganii_i] = '\0';
    
}
