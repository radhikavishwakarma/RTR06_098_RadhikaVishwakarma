#include<stdio.h>  // for printf()
#include<conio.h>  // for getch()

// ASCII Values for 'A' to 'Z' => 65 to 90
#define CHAR_ALPHABET_UPPER_CASE_BEGINNING 65
#define CHAR_ALPHABET_UPPER_CASE_ENDING 90

// ASCII Values for 'a' to 'z' => 97 to 122
#define CHAR_ALPHABET_LOWER_CASE_BEGINNING 97
#define CHAR_ALPHABET_LOWER_CASE_ENDING 122

// ASCII Values for '0' to '9' => 48 to 57
#define CHAR_DIGIT_BEGINNING 48
#define CHAR_DIGIT_ENDING 57

int main(void)
{
    // variable declaration
    char ch;
    int ch_value;

    // code
    printf("\n\n");
    printf("Enter Character: ");
    ch = getch();
    printf("\n\n");

    if((ch == 'A' || ch == 'a') || (ch == 'E' || ch == 'e') || (ch == 'I' || ch == 'i') || (ch == 'O' || ch == 'o') || (ch == 'U' || ch == 'u'))
    {
        printf("Character \'%c\' Entered by you is a VOWEL Character from the English alphabet !!!\n\n", ch);
    }
    else
    {
        ch_value = (int)ch;
        /* If the character has ASCII value between 65 and 90 OR between 97 and 122,
        it is still a letter of the alphabet, but it is a 'CONSONANT', and NOT a 'VOWEL'...
        */
        if ((ch_value >= CHAR_ALPHABET_UPPER_CASE_BEGINNING && ch_value <= CHAR_ALPHABET_UPPER_CASE_ENDING) || (ch_value >= CHAR_ALPHABET_LOWER_CASE_BEGINNING && ch_value <= CHAR_ALPHABET_LOWER_CASE_ENDING))
        {
            printf("Character \'%c\' Entered by you is a CONSONANT Character from the English Alphabet !!!\n\n", ch);
        }
        else if (ch_value >= CHAR_DIGIT_BEGINNING  && ch_value <= CHAR_DIGIT_ENDING)
        {
            printf("Character \'%c\' Entered by you is a DIGIT Character !!!\n\n", ch);
        }
        else
        {
            printf("Character \'%c\' Entered by you is a SPECIAL Character !!!\n\n", ch);
        }
    }
    
    return 0;
}