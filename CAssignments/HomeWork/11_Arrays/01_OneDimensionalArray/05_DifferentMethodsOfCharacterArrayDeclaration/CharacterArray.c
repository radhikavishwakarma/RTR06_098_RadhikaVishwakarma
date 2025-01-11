#include <stdio.h>

int main(void)
{
    // variable declarations
    char chArray_01[] = {'A', 'S', 'T', 'R', 'O', 'M', 'E', 'D', 'I', 'C', 'O', 'M', 'P', '\0'};  // Must give \0 Explicitly for proper initialization
    char chArray_02[9] = {'W', 'E', 'L', 'C', 'O', 'M', 'E', '\0'};  // Must give \0 Explicitly for proper initialization
    char chArray_03[] = {'Y', 'O', 'U', '\0'};  // Must give \0 Explicitly for proper initialization
    char chArray_04[] = "To";  // \0 is assumed, size is given as 3, although string has only 2 characters
    char chArray_05[] = "REAL TIME RENDERING BATCH OF 2024-25";  // \0 is assumed, size is given as 40, although string has only 39 characters

    char chArray_WithoutNullTerminator[] = {'H', 'e', 'l', 'l', 'o'};

    // code
    printf("\n\n");
    printf("Size of chArray_01 : %lu\n\n", sizeof(chArray_01));
    printf("Size of chArray_02 : %lu\n\n", sizeof(chArray_02));
    printf("Size of chArray_03 : %lu\n\n", sizeof(chArray_03));
    printf("Size of chArray_04 : %lu\n\n", sizeof(chArray_04));
    printf("Size of chArray_05 : %lu\n\n", sizeof(chArray_05));
    printf("\n\n");
    
    printf("The Strings are: \n\n");
    printf("chArray_01 : %s\n\n", (chArray_01));
    printf("chArray_02 : %s\n\n", (chArray_02));
    printf("chArray_03 : %s\n\n", (chArray_03));
    printf("chArray_04 : %s\n\n", (chArray_04));
    printf("chArray_05 : %s\n\n", (chArray_05));

    printf("\n\n");
    printf("Size of chArrayWithoutNullTerminator : %lu\n\n", sizeof(chArray_WithoutNullTerminator));
    printf("chArrayWithoutNullTerminator : %s\n\n", chArray_WithoutNullTerminator); // Will display garbage value at the end of string due to absence of \0

    return 0;
}