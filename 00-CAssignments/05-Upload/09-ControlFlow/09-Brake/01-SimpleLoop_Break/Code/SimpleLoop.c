#include<stdio.h>
#include<conio.h>
int main(void)
{
    //varibale declarations
    int ganii_i;
    char ganii_ch;

    //code
    printf("\n\n");

    printf("Priniting EVen Number From 1 to 100 For Every User Input,Exiciting the Loop when ENteres Character 'Q' or 'q' :\n\n");
    printf("ENter Character 'Q' or 'q' To Exit Loop :\n\n");

    for(ganii_i =0; ganii_i <= 100; ganii_i++)
    {
        printf("\t%d\n",ganii_i);
        ganii_ch = getch();
        if(ganii_ch == 'Q' || ganii_ch =='q')
        {
            break;
        }
    }
    printf("\n\n");
    printf("EXISING LOOP...");
    printf("\n\n");

    return 0;
}