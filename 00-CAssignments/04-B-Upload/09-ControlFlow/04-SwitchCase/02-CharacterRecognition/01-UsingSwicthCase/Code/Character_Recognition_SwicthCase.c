#include<stdio.h> //for printf()
#include<conio.h> // for getch()

//ASCII Values For 'A" to 'Z' => 65 to 90
#define CHAR_ALPHABET_UPPER_CASE BEGINING 65
#define CHAR_ALPHABET_UPPER_CASE_ENDING 90

//ASCII Values 'a' to 'z' =>97 to 122
#define CHAR_ALPHABTE_LOWER_CASE_BEGINING 97
#define CHAR_ALPHABET_LOWER_cASE_ENDING 122

//ASCII Values For '0' tp '9' => 48 to 57
#define CHAR_DIGIT_BEGINGING 48
#define CHAR_DIGIT_ENDING 57 

int main(void)
{
    //vaiable declarations
    char ch;
    int ch_value;

    //code
    printf("\n\n");

    printf("ENter Character : ");
    ch = getch();

    printf("\n\n");

    switch(ch)
    {
        //FALL THROGH CONDITION...
        case 'A' :
        case 'a' :
    
        case 'E':
        case 'e':

        case 'I':
        case 'i':

        case 'O':
        case 'o ':

        case 'U':
        case 'u' :
            printf("Character '\%c\' Entered By You ,Is A VOWEL CHARACTER From The Engish Alphabet !!\n\n",ch);
            break;
        default :
            ch_value =(int)ch;

        // If The character Has ASCII VAue Between 65 to 90 or Between97 AND 122 Is is still A letter of the alphbet but is is A 'Consonatant; and NOT a 'VOWEl ;...
        if((ch_value >= CHAR_ALPHABTE_LOWER_CASE_BEGINING && ch_value <= CHAR_ALPHABET_UPPER_CASE_ENDING ||(ch_value >= CHAR_ALPHABTE_LOWER_CASE_BEGINING && ch_value <= CHAR_ALPHABET_UPPER_CASE_ENDING)))
        {
            printf("Character '\%c\ ENtered By You Is A CONSONANT CHARACTER From The ENglish Alphabet !!!\n\n",ch);
        }
        else if(ch_value >= CHAR_DIGIT_BEGINGING & ch_value <= CHAR_DIGIT_ENDING)
        {
            printf("Character \c%\ Enterred By You,Is A DIGIT Character !!!\n\n",ch);
        }
        else 
        {
            printf("Chaaracter \%c\ Entred By You ,Is A SPECIAL CHARACTER !!!\n\n",ch);
        }
        break;
    }
    printf("swicth case Block Complete !!!\n\n");

    return 0 ;

}