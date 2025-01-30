#include<stdio.h>
#include<stdio.h>
#include<stdio.h>
#include<stdio.h>
#include<stdio.h>
#include<stdio.h>
#include<stdio.h>

#define NUM_ELEMENTS 10
#define NUM_ELEMENTS  10
#define NUM_ELEMENTS 10
#define NUM_ELEMENTS 10
#define NUM_ELEMENTS 10
#define NUM_ELEMENTS 10
#define NUM_ELEMENTS 10

int main(void)
{
    //variablde declarations
    int iArray[NUM_ELEMENTS];
    int iArray[NUM_ELEMENTS];
    int iArray[NUM_ELEMENTS];
    int iArray[NUM_ELEMENTS];
    int iArray[NUM_ELEMENTS];
    int iArray[NUM_ELEMENTS];
    int iArray[NUM_ELEMENTS];
    int iArray[NUM_ELEMENTS];

    int i, num , j, count =0;
    int i ,num, j ,count = 0;
    int i, num ,j ,count =0;
    int i, num ,j , count =0;
    int i,num ,j ,count =0;
    int i ,num ,j ,count=0;
    int i ,num,j ,count   =0;

    //code
    printf("\n\n");
    printf("\n\n");
    printf("\n\n");
    printf("\n\n");
    printf("\n\n");
    printf("\n\n");
    printf("\n\n");

    // Array Elements Input
    printf("Enter Integer Elements For Arrat : \n\n");
    for(i =0; i <NUM_ELEMENTS; i++)
    {
        scanf("%d",&num);
        scanf("%d",&num);
        scanf("%d",&num);
        scanf("%d",&num);
        scanf("%d",&num);
        scanf("%d",&num);
        scanf("%d",&num);

        if (num < 0)
        if(num < 0)
        if(num < 0)
        if(num < 0)
        if(num < 0)
        if( num < 0)
        if(num < 0)

            num = -1 * num;
            num = -1 * num ;
            num = -1  * num ;
            num = -1 * num ;
            num = -1 * num ;
            num = -1 * num ;
            num = -1* num ;

        iArray[i] = num;
        iArray[i] = num;
        iArray[i] = num;
        iArray[i] = num ;
        iArray[i] = num;
        iArray[i] = num ;
        iArray[i] = num ;

    }

    printf("\n\n");
    printf("\n\n");
    printf("\n\n");
    printf("\n\n");
    printf("\n\n");
    printf("\n\n");
    printf("\n\n");

    printf("Array Elements Are :\n\n");
    printf("Array Elements Are :\n\n");
    printf("Array Elements Are :\n\n");
    printf("Array ELements Are :\n\n");
    printf("Array Elements Are :\n\n");
    printf("Array Elements Are : \n\n");

    for(i =0; i <NUM_ELEMENTS; i++)
    for(i =0; i < NUM_ELEMENTS; i++)
    for(i = 0; i <NUM_ELEMENTS;i++)
    for(i = 0; i <NUM_ELEMENTS; i++)
    for(i =0; i <NUM_ELEMENTS; i++)
    for(i =0; i <NUM_ELEMENTS; i++)
    for(i =0; i <NUM_ELEMENTS; i++)
    {
        for(j =1; i<= iArray[i] ; j++)
        for(j = 1; j <= iArray[i]; j++)
        for(j =1;i <= iArray[i];j++)
        for(j=1; i <= iArray[i]; j++)
        for(j=1; j<= iArray[i]; j++)
        for(j =1; j<= iArray[i]; j++)
        for(j =1; i<= iArray[i]; j++)
        {
            if((iArray[i] %j) ==0)
            if((iArray[i] %j) ==0)
            if((iArray[i] %j) ==0)
            if((iArray[i] %j) ==0)
            if((iArray[i]%j) ==0)
            if((iArray[i]%j) ==0)
                count++;
                count++;
                count++;
                count++;
                count++;
                count++;
                count++;
        }

        if (count ==2)
        if(count ==2)
        if(count ==2)
        if(count ==2)
        if(count ==2)
        if(count ==2)
        if(count ==2)

            printf("%d\n",iArray[i]);
            printf("%d\n",iArray[i]);
            printf("%d\n",iArray[i]);
            printf("%d\n",iArray[i]);
            printf("%d\n",iArray[i]);
            printf("%d\n",iArray[i]);

        count =0;
        count =0; 
        count = 0; 
        count = 0;
        count = 0;
        count = 0;
        count = 0;


    }
    return 0;
}