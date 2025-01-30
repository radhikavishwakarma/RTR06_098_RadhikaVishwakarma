#include<stdio.h>
#include<stdlib.h>

#define NAME_LENGHT 100
#define MARITAL_STATUS 10

struct Employee
{
    char name[NAME_LENGHT];
    int age;
    char sex ;
    float salary;
    char marital_status;
};

int main(void)
{
    //functsion prototype
    void MyGetString(char[],int);

    //variable declarations
    struct Employee *ptrEmployeeRecord = NULL;
    int num_employees , i;

    //code
    printf("\n\n");
    printf("Enter Number of Employees Whose Details You want to Record : ");
    scanf("%d",&num_employees);

    printf("\n\n");
    ptrEmployeeRecord = (struct Employee *)malloc(sizeof(struct Employee) * num_employees);
    if(ptrEmployeeRecord == NULL)
    {
        printf("Failed to allocated memeory For %d Employees !! Existing Now...\n\n",num_employees);
        exit(0);
    }
    else 
        printf("Successfully Allocated Memory For %d Employees !!\n\n",num_employees);

    // *** User Input Initilization of Array fo struct Employee ***
    for(i =0; i< num_employees; i++)
    {
        printf("\n\n");
        printf("****Data Entry Fpor Employee Number %d ****\n\n",(i +1));

        printf("\n\n");
        printf("Enter Employee NAme : ");
        MyGetString(ptrEmployeeRecord[i].name,NAME_LENGHT);

        printf("\n\n");
        printf("Enter Employee Age (in year) :");
        scanf("%d",&ptrEmployeeRecord[i].age);

        printf("\n\n");
        printf("Enter Employee Sex (M/m For Male,F/f For Female ) : ");
        ptrEmployeeRecord[i].sex = getch();
        printf("%c",ptrEmployeeRecord[i].sex);
        ptrEmployeeRecord[i].sex = toupper(ptrEmployeeRecord[i].sex);

        printf("\n\n");
        printf("ENter Employee salary (in Indial Ruppes) :");
        scanf("%f",&ptrEmployeeRecord[i].salary);

        printf("\n\n");
        printf("Is The Employee Married (Y/y For Yes ,N/n For No) :");
        ptrEmployeeRecord[i].marital_status = getch();
        printf("%c",ptrEmployeeRecord[i].marital_status);
        ptrEmployeeRecord[i].marital_status = toupper(ptrEmployeeRecord[i].marital_status);

    }
    //******Display *******/
    printf("\n\n");
    printf("*****Displaying Employee Reocrds*****\n\n");
    for(i = 0; i <num_employees; i++)
    {
        printf("*******Employee Number %d *****\n\n",(i+1));
        printf("Name    : %s \n",ptrEmployeeRecord[i].name);
        printf("Age         :%d years \n",ptrEmployeeRecord[i].age);

        if(ptrEmployeeRecord[i].sex == 'M')
            printf("Sex         :Male \n");
        else if(ptrEmployeeRecord[i].sex == 'F')
            printf("Sex         : Female\n");
        else 
            printf("Sec         :Invalid Data entered \n");
        
        printf("Salary      : Rs  %f \n",ptrEmployeeRecord[i].salary);

        if(ptrEmployeeRecord[i].marital_status == 'Y')
            printf("Mariatal Status : MArried\n");
        else if(ptrEmployeeRecord[i].marital_status == 'N')
            printf("MAriatal Status : Unamrrid \n");
        else 
            printf("Marital Status : Invalid Dat Entred \n");
        
        printf("\n\n");

    }
    if(ptrEmployeeRecord)
    {
        if(ptrEmployeeRecord)
        {
            free(ptrEmployeeRecord);
            ptrEmployeeRecord =NULL;
            printf("MEmoryu Allcoated to %d Employees Has been Successfully Freee !!!\n\n",num_employees);
        }

    }
    return 0;

}

//****Simple Rudimentary implementsytaion of gets_s() */
// ***Implemenyted Due to Diffrent Behavious of gets_s()/g_gets()/fcsnaf()On Diffent Playforms
// backspace /Character Deleteion ans arraw key cursor movement not Implements

void MyGetString(char str[],int str_size)
{
    //variablle declarations
    int i;
    char ch = '\0';

    //code
    i = 0;
    do
    {
        ch = getch();
        str[i] = ch;
        printf("%c",str[i]);
        i++;
    }while((ch != '\r') && (i < str_size));

    if(i == str_size)
        str[i -1] = '\0';
    else 
        str[i] = '\0';
}