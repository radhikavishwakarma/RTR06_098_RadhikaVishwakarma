#include<stdio.h>
#include<string.h> //for upper()

int main()
{
    //variable declarations
    char ch,ch_i;
    unsigned int ascii_ch = 0;

    //code
    printf("\n\n");
    printf("Enter The First Character of First Name : ");
    ch = getch();

    ch = toupper(ch);

    for(ch_i = 'A' ; ch_i <= 'Z'; ch_i++)
    {
        if(ch == ch_i)
        {
            ascii_ch =(unsigned int)ch;
            goto result_output; // Prorgam flow jumps directly to label result_output

        }
    }
        printf("\n\n");
        printf("Go to Statement not executed ,so printing \"Hello,World !!!\n");// will be omitted if got statmenet is executed
        
result_output://Label itself does not alter flow of program ,following code
        //is executed regardless of wheteher goto statement is executed or not.
        printf("\n\n");
        if(ascii_ch == 0)
        {
            printf("You Must have a struanbge name ! Could not find the characters %c in the entire English Alphabet !\n",ch);
        }
        else
        {
            printf("Character %c found .Ithas ASCII  value %u \n",ch,ascii_ch);
        }
        printf("\n\n");

        return 0;
    }
