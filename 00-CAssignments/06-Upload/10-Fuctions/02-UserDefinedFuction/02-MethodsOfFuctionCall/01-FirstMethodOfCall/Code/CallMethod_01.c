#include<stdio.h> //'stdio.h' contains declaration of printf()

// ************8USER DEFINED FUCTIONS : METHOD OF CALLING FUCTION 1 
//*********CALLING ALL FUCTION IN main() DIRECTLY   */

//Entr-point fuction=>main()=>Valid Return Type (int) and 3 Paramaterers(int argc,char *argv[],char *envp[])

int main(int agrc, char *argv[],char *envp[])
{
    //fuction prototypes or declarations
    void MyAddition(void);
    int MySubtraction(void);
    void MyMultiplication(int,int);
    int MyDivision(int,int);

    // variable declarations 
    int result_subtraction;
    int a_mutiplication,b_multiplication;
    int a_division,b_division,result_division;

    //code

    //***ADDITION */  
    MyAddition();// fuction call

    // ***SUBTRACTION ***//////////
    result_subtraction = MySubtraction();// fuction  call
    printf("\n\n");
    printf("Subtraction Yiels Result = %d\n",result_subtraction);

    //***MULTIPLCIATION */
    printf("\n\n");
    printf("Enter Ineger Value For 'A' For Multiplication : ");
    scanf("%d",&a_mutiplication);

    printf("\n\n");
    printf("Enter Intger Value For 'B' For Division");
    scanf("%d",&b_multiplication);

    MyMultiplication(a_mutiplication,b_division);// fuction call

    
    //**Division*** */
    printf("\n\n");
    printf("Enter Valur For 'A' For Division : ");
    scanf("%d",&a_division);

    printf("\n\n");
    printf("Enter Integer Valur For 'B' For Division : ");
    scanf("%d",&b_division);

    result_division = MyDivision(a_division,b_division);
    printf("\n\n");

    printf("Division of %d and %d Gives = %d (Quotient\n)",a_division,b_division,result_division);

    printf("\n'n");
    return 0;
}

// ***Fuction Definition of MySubtraction() ***********
void MyAddition(void) // fuction definition
{
    //varible declarations : local variable  to MySubtraction()
    int a, b,sum;

    //code 
    printf("\n\n");
    printf("Enter Intger Value For 'A' For Addition : ");
    scanf("%d",&a);

    printf("\n\n");
    printf("Enter Integer Valur For ' B' Addition : ");
    scanf("%d",&b);

    sum = a + b;
    printf("\n\n");
    printf("Sum Of %d And %d = %d \n\n",a,b,sum);
}

// ***Fuction Definition of MySubtraction() ***********
int MySubtraction(void) // fuction definition
{
    //varible declarations : local variable  to MySubtraction()
    int a, b,subtraction;

    //code 
    printf("\n\n");
    printf("Enter Intger Value For 'A' For Subtraction : ");
    scanf("%d",&a);

    printf("\n\n");
    printf("Enter Integer Valur For ' B' Subtracction : ");
    scanf("%d",&b);

    subtraction = a - b;
    return subtraction;
}

// *** Fuction Definition f MyMultiplication() *****
void MyMultiplication(int a,int b) // fuction definition
{
    //variable declarations  : local variable to MyMultiplication()

    int multiplication ;

    //code 
    multiplication = a * b;

    printf("\n\n");
    printf("Multiplcation of %d And %d = %d \n\n",a,b,multiplication);
}

// *** fuction Definition of MyDivision ( **********)
int MyDivision(int a, int b)//fuction definition
{
    //variable declarations : local variables to MyDivision()
    int division_quotient;

    //code 
    if(a> b)
        division_quotient = a /b;
    else 
        division_quotient = b /a ;

    return division_quotient;
}
