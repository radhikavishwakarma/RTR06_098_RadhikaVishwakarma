#include<stdio.h>

int main(void)
{
    //fucntsion declarations
    void SwapNumbers(int,int);

    //variable declaration
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
    printf("****Before Swapping ***\n\n");
    printf("Valsue of 'A' = %d\n\n",a);
    printf("Value of 'B' = %d \n\n",b);

    SwapNumbers(a,b);//****Arguemnets Passed By Value.. */
    printf("\n\n");
    printf("****After Swapping ***\n\n");
    printf("Value of 'A' = %d \n\n",a);
    printf("Value of  'B' = %d \n\n",b);

    return 0;

}

void SwapNumbers(int x,int y)//Value of 'a' is copied into 'x' and value of 'b' is copied into 'y'...swapping takes place between x and y not between a and b...
{
    //variable declarations
    int temp;

    //code
    printf("\n\n");
    printf("***Before Swapping ***\n\n");
    printf("Value of 'x' = %d\n\n",x);
    printf("Value of 'y' = %d\n\n",y);

    temp =x ;
    x = y;
    y = temp ;

    printf("\n\n");
    printf("****After swapping***\n\n");
    printf("value of x = %d \n\n",x);
    printf("Value of 'y' = %d\n\n",y);
}