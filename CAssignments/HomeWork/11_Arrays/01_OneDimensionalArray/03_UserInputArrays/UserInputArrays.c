#include <stdio.h>
#include <conio.h>

// MACRO CONSTANT used as Array size in subscript and as Array lenght.
/* Hence, this program's Array's sizes can be simply changed by changing these
following 3 GLOBAL MACRO CONSTANT values, before compiling, linking and executing
the program.
*/

#define INT_ARRAY_NUM_ELEMENTS 5
#define FLOAT_ARRAY_NUM_ELEMENTS 3
#define CHAR_ARRAY_NUM_ELEMENTS 15

int main(void)
{
    // variable declarations
    int iArray[INT_ARRAY_NUM_ELEMENTS];
    float fArray[FLOAT_ARRAY_NUM_ELEMENTS];
    char cArray[CHAR_ARRAY_NUM_ELEMENTS];
    int i, num;

    // code

    // ***** ARRAY ELEMENTS INPUT *****
    printf("\n\n");
    printf("Enter Elements for 'Integer' Array : \n");
    for (i = 0; i < INT_ARRAY_NUM_ELEMENTS; i++)
    {
        scanf("%d", &iArray[i]);
    }

    printf("\n\n");
    printf("Enter Elements for 'Floating-Point' Array : \n");
    for (i = 0; i < FLOAT_ARRAY_NUM_ELEMENTS; i++)
    {
        scanf("%f", &fArray[i]);
    }

    printf("\n\n");
    printf("Enter Elements for 'Character' Array : \n");
    for (i = 0; i < CHAR_ARRAY_NUM_ELEMENTS; i++)
    {
        cArray[i] = getch();
        printf("%c\n", cArray[i]);
    }

    // ***** ARRAY ELEMENTS INPUT *****
    printf("\n\n");
    printf("Integer Array entered by you : \n\n");
    for (i = 0; i < INT_ARRAY_NUM_ELEMENTS; i++)
    {
        printf("%d", iArray[i]);
    }

    printf("\n\n");
    printf("Floating-Point Array entered by you : \n\n");
    for (i = 0; i < FLOAT_ARRAY_NUM_ELEMENTS; i++)
    {
        printf("%d", fArray[i]);
    }
    
    return 0;
}