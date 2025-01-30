#include<stdio.h>
int main(void)
{
    //variable declarations
    char ganii_opttion,ganii_ch ='\0';

    //code
    printf("\n\n");
    printf("Once The Infinite loop Begins,Enter 'Q' or 'q' to Quit The Inifinite For loop :\n\n");
    printf("Enter 'Y' or 'y' to Initiate User Cotrolled Infinite Loop :");
    printf("\n\n");
    ganii_opttion = getch();
    if(ganii_opttion == 'Y' || ganii_opttion == 'y')
    {
        do
        {
            /* code */
              printf("In Loop ...\n\n");
            ganii_ch = getch();//cotrol flow waits for chaarater input...
            if(ganii_ch == 'Q' || ganii_ch =='q')
                break;
            }while(1);
            printf("\n\n");
            printf("EXISTING USER COTROLLED INFINITE LOOP...");
            printf("\n\n");
      
    }
    else    
        printf("YouMust Press 'Y' or 'y' To Initiate The User Cotrolled Inifinite Loop..Please Try Agian...\n\n");

    return 0;

   
}