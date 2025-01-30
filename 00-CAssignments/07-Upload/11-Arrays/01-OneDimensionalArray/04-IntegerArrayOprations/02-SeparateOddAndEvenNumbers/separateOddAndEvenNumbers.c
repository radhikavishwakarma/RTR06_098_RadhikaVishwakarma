#include<stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
    //varible declarations
    int iArray[NUM_ELEMENTS];
    int i , num,sum = 0;

    //code
    printf("\n\n");

    // *** ARRAY ELEMENTS INPUT  *****
    printf("Enter Intger Elements For Array : \n\n");
    for(i=0; i <NUM_ELEMENTS; i++)
    {
        scanf("%d",&num);
        iArray[i] = num;
    }

    // **** SEPARATING OUT EVENN NUMBERS FROM ARRAY ELEMENTS ****
    printf("\n\n");
    printf("Even Numbers Amognst The Array Elements Are : \n\n");
    for(i =0; i <NUM_ELEMENTS;i++)
    {
        if((iArray[i] % 2 ) == 0)
            printf("%d\n",iArray[i]);
    }

    // *** SEPARATING OUT ODD NUMBERS FROM ARRAY ELEMENTS ****
    printf("\n\n");
    printf("Odd Numbers Amongst The Array Elements Are : \n\n");
    for(i =0; i <NUM_ELEMENTS; i++)
    {
        if((iArray[i] %2) != 0)
            printf("%d\n",iArray[i]);
    }

    return 0;
}