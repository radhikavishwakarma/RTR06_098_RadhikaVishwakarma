#include<stdio.h>
int main(void)
{
    //variable declarations
    int num_month;

    //code
    printf("\n\n");

    printf("Enter Number Of Month (1 to 12) :");
    scanf("%d",&num_month);

    printf("\n\n");

    switch(num_month)
    {
        case 1://like if
            printf("Month Number %d Is January !!!\n\n",num_month);
            break;
        case 2: //linke else if
            printf("Month Number %d Is February !!!\n\n",num_month);
            break;
        case 3 :// like else if
            printf("Month Number %d Is March !!!\n\n",num_month);
            break;
        case 4 ://like else if
            printf("Month Number %d Is April !!!\n\n",num_month);
            break;
        case 5 : //like else if
            printf("Month Number %d Is May !!!!\n\n",num_month);
            break;
        case 6 ://like else if
            printf("Month Number %d Is june !!!\n\n",num_month);
            break;
        case 7 ://like else if
            printf("Month Number %d Is July !!!|n\n",num_month);
            break; 
        case 8 ://like else if
            printf("Month Number %d Is August !!!\n\n",num_month);
            break;
        case 9 :// like else if
            printf("Month Number %d Is Sep !!!\n\n",num_month);
            break;
        case 10:// like else if
            printf("Month Number %d Is October !!!\n\n",num_month);
            break;
        case 11 : //like else if
            printf("Month Number %d is Novermber !!\n\n",num_month);
            break;
        case 12 ://like else if
            printf("Month Number %d is Decemeber !!!\n\n",num_month);
            break;
        default:
            printf("Invalid Month % d Entred !! Please Try Again..\n\n",num_month);
            break;

    }
    printf("Switch Case block Complete !!!\n\n");
    return 0;
}