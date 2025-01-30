#include<stdio.h>
int main(void)
{
    //vaiable declarations
    int num_month;

    //code
    printf("\n\n");

    printf("Enter Number of Month (1to 12) :");
    scanf("%d",&num_month);

    printf("\n\n");

    //IF-ELSE IF LADDER BEGINS FROM HERE....
    if (num_month ==1) //like case 1
        printf("Month Number %d Is January !!!\n\n",num_month);
    else if(num_month == 2) //like case 2
        printf("Month Number d Is Febrary !!!\n\n",num_month);
    else if(num_month == 3)//like cass 3
        printf("Month Number %d Is March !!!|n\n",num_month);
    else if(num_month == 4 )// like case 4
        printf("Month Number %d Is APRIL !!!|n\n",num_month);
    else if(num_month == 5)// like case 5
        printf("Month Number %d Is May !!\n\n",num_month);
    else if(num_month == 6)// like case 6
        printf("Month Number %d Is June !!\n\n",num_month);
    else if(num_month == 7)// like case 7 
        printf("Month Number %d Is July !!!\n\n",num_month);
    else if(num_month == 8) // like case 8
        printf("Month Number %d Is August !!\n\n",num_month);
    else if(num_month == 9) // like case 9 
        printf("Month Number %d Is September !!|n\n",num_month);
    else if(num_month == 10) //like case 10
        printf("Month Number %d is October !!|n\n",num_month);
    else if(num_month == 11)// like case 11 
        printf("Month NUmber %d ius November !!\n\n",num_month);
    else if(num_month== 12)// like case 12 
        printf("Month Number %d Is Deciember !!\n\n",num_month);
    else // like 'defualt ..justr like default is optional is switch -case is 'else' is the if-else ladder..
        printf("Invalid Month NUmber %d Entered !! Please try again..\n\n",num_month);

    printf("If-Else If-Else Ladder Complete!!|n\n");

    return ;

}