#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_STRING_LENGTH 1024

struct CharacterCount {
    char ch;
    int ch_count;
} character_and_count[] = {
    {'A', 0}, {'B', 0}, {'C', 0}, {'D', 0}, {'E', 0},
    {'F', 0}, {'G', 0}, {'H', 0}, {'I', 0}, {'J', 0},
    {'K', 0}, {'L', 0}, {'M', 0}, {'N', 0}, {'O', 0},
    {'P', 0}, {'Q', 0}, {'R', 0}, {'S', 0}, {'T', 0},
    {'U', 0}, {'V', 0}, {'W', 0}, {'X', 0}, {'Y', 0}, {'Z', 0}
    //Character And Count[25].ch = 'Z' & character and count[25].ch_count = 0
};

#define SIZE_OF_ENTIRE_ARRAY_OF_STRUCTS sizeof(character_and_count)
#define SIZE_OF_ONE_STRUCT_FROM_THE_ARRAY_OF_STRUCTS sizeof(character_and_count[0])
#define NUM_ELEMENTS_IN_ARRAY (SIZE_OF_ENTIRE_ARRAY_OF_STRUCTS / SIZE_OF_ONE_STRUCT_FROM_THE_ARRAY_OF_STRUCTS)

//Entry Point FUnction
int main(void) {
    // Variable declarations
    char str[MAX_STRING_LENGTH];
    int i, j, actual_string_length = 0;

    // Input string
    printf("\n\n");
    printf("\nEnter a string:\n");
    fgets(str, MAX_STRING_LENGTH, stdin);
    str[strcspn(str, "\n")] = '\0'; // Remove trailing newline, if any

    actual_string_length = strlen(str);

    printf("\n\n");
    printf("\nThe string you have entered is:\n%s\n", str);


    for (i = 0; i < actual_string_length; i++) {
        if (isalpha(str[i])) { // Check if the character is alphabetic
            str[i] = toupper(str[i]); // Convert to uppercase
            for (j = 0; j < NUM_ELEMENTS_IN_ARRAY; j++) {
                if (str[i] == character_and_count[j].ch) {
                    character_and_count[j].ch_count++; // Increment its Count By 1
                    break;
                }
            }
        }
    }

    // Display character counts
    printf("\n\n");
    printf("\nThe number of occurrences of all characters from the alphabet are as follows:\n");
    for (i = 0; i < NUM_ELEMENTS_IN_ARRAY; i++) {
        printf("Character %c = %d\n", character_and_count[i].ch, character_and_count[i].ch_count);
    }

    return (0);
}
