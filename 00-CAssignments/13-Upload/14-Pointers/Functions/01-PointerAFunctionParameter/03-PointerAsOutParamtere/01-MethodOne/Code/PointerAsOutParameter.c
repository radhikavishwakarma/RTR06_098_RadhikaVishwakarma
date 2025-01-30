#include<stdio.h>

int main(void)
{
    //function declarations
    void MathematicalOperations(int,int,int*,int*,int*,int*,int*);

    //varibale declaration
    int a;
    int b;
    int answer_sum;
    int answer_difference;
    int answer_product;
    int answer_quotient;
    int answer_remainder;

    //code
    printf("\n\n");
    printf("Enter Value of 'A' : ");
    scanf("%d",&a);

    printf("\n\n");
    printf("Enter Value of 'B' : ");
    scanf("%d",&b);

    /*passing addresses to function...functsion will fill them up with
    values...hence ,they go into the funcction as address parametres and come out of the function filled with avlid values
    thus (&answer_sum,&answer_diffeence,&and_asnwer_product,&answer_quotient,&answer_remainder)are called Out Paramters' or 'Paramaterizes returnvalue...return values of function
    coming via parametres
    //Hence,althou each function has only one return value,using the concept
    of parameterized return value our function "MathemAticalOperations()
    has gievn Us 5 return values"
*/

   MathematicalOperations(a,b,&answer_sum,&answer_difference,&answer_product,&answer_quotient,&answer_remainder);
    //MathematicalOperations(a ,b , &answer_sum , &answer_difference ,&answer_product ,&answer_quotient ,&answer_remainder);
    printf("\n\n");
    printf("****Results****\n\n");
    printf("Sum = %d \nn\n",answer_sum);
    printf("Difference= %d \n\n",answer_difference);
    printf("Produc = %d\n",answer_product);
    printf("quotient = %d \n\n",answer_product);
    printf("Remainder = %d \n\n",answer_remainder);
    return 0;

}

void MathematicalOperations(int x, int y, int *sum,int *difference, int *product,int *quotient,int *remainder)
{

    *sum = x + y;//value at address sum = (x +y)
    *difference = x - y; //value ata address differnect = x - y
    *product = x * y ;//value ata address product = (x * y)
    *quotient = x / y ;// value ata address quotient = (x/y)
    *remainder = x %y; // value ata ddress remainder =  (x)
}