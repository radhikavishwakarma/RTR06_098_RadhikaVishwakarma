#include<stdio.h>
int main(void)
{
    //variable declarations
    char ganii_option,ganii_ch= '\0';

    //code
    printf("\n\n");
    printf("Once The Infiite Loop Begins,Enter 'Q' or 'q' to Quit the Infinite For loop : \n\n");

    do
    {
        do
        {
            printf("\n\n");
            printf("In Loop...\n");
            ganii_ch = getch();// cotrol flow waites for character input..
        }while(ganii_ch != 'Q' && ganii_ch != 'q');
        printf("\n\n");
        printf("EXISTING USER COTROLLED INFINITE LOOP...");

        printf("\n\n");
        printf("DO YOU WANT TO BEGIN USER COTROLLED INFINITE LOOP AGAIN?...(Y/y)Yes,AnyOther key-No): ");
        ganii_option = getch();
    }while(ganii_option == 'Y' || ganii_option == 'y');
    return 0;
}