#include<stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
    // variable declarations
    int iArray[NUM_ELEMENTS];
    int i, num,j, count =0 ;

    //code
    printf("\n\n");

    // ** Array ELEMENTS INPUT ****
    printf("Enter Integer Elements For Array : \n\n");
    for(i = 0; i <NUM_ELEMENTS; i++)
    {
        scanf("%d",&num);

        // If 'num' is negative ( < 0),then convert it to positive (multiply by -1) 
        if( num < 0)
            num = -1 * num;
        
        iArray[i] =  num;

    }

    // *** PRINTTIMG ENTIRE ARRAY  ****
    printf("\n\n");
    printf("Array Elements Are : \n\n");
    for(i = 0; i <NUM_ELEMENTS; i ++)
        printf("%d\n",iArray[i]);
    
    // ** SEPARATING OUT EVEN NUMBERS ARRAY ELEMENTS ****
    printf("\n\n");
    printf("Primen Number Amongst The Array Elements Are : \n\n");
    for(i =0; i <NUM_ELEMENTS; i++)
    {
        for(j =1;j <= iArray[i]; j++)
        {
            if((iArray[i] %j) ==0)
                count++;
                
        }

        // NUMBERS 1 IS NEITHER A PRIME NUMBER NOT A CONSONANT
        // IF A NUMBER IS PRIME ,IT IS ONLY DIVISIBLY BY 1, AND ITSELF.
        // HENCE,IF A NUMBER IS PRIME, THE VALUE OF 'count' WILL BE EXACTLY 2.
        //IF THE VALUE OF 'count' IS GREATER THAN 2,THE NUMBER IS DIVISIBLE BY
        //NUMBER OTHER THAN 1 AND ITSELF AND HENCE ,IT IS NOT PRIME
        //THE VALUE OF 'count' WILL BE 1 ONLY IF iArray[i] IS 1.
        if(count ==2)
            printf("%d\n",iArray[i]);
        
        count = 0;// RESET 'count' TO 0 FOR CHECKING THE NEXT NUMBER....


    }
    return 0;
}