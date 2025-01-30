#include<stdio.h>

int main(void)
{
    //function declarations
    void SwapNumbers(int*,int*);

    //variable declarations
    int a;
    int b;

    //code
    printf("\n\n");
    printf("Enter Value For 'A' :");
    scanf("%d",&a);

    printf("\n\n");
    printf("Enter Value For 'B' : ");
    scanf("%d",&b);

    printf("\n\n");
    printf("*****Before Swapping *****\n\n");
    printf("Value fo 'A' = %d \n",a);
    printf("Value for 'B' = %d \n",b);

    SwapNumbers(&a,&b); ///**Arguments passed by reference /addresss///
     
    printf("\n\n");
    printf("******After Swapping ****\n\n");
    printf("Value of 'A' = %d \n\n",a);
    printf("Value of 'B' = %d \n\n",b);

    return 0;
}

// Address of 'a' is copied into 'x' and address of 'b' is copied into 'y'...so
//'&a' and 'x' are pointing to One and &b and y are pointing to one and the same address..
// Swapping takes place between value at address of (value at &a and i.e. 'a')
// and value at address y (value at &b i.i. 'b)....
//Hence Swapping in this case takes place between *x and *y As well as
//between a  and b ..

void SwapNumbers(int *x,int *y)
{
    //variable declarations
    int temp;

    //code
    printf("\n\n");
    printf("*****BEfore Swapping ****\n\n");
    printf("Value of 'X' = %d \n\n",*x);
    printf("Value of 'Y = %d\n\n",*y);

    temp = *x ;
    *x = *y;
    *y = temp;

    printf("\n\n");
    printf("***After Swapping ***\n\n");
    printf("Value of 'x = %d \n\n",*x);
    printf("Value of 'Y = %d \n\n",*y);

}