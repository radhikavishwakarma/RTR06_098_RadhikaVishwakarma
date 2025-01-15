#include <stdio.h>
#include <stdlib.h>

void MyAlloc(int** ptr, unsigned int numberOfElements);

int main(void)
{
    // Variable declarations
    int* piArray = NULL;
    unsigned int num_elements;
    int i;

    // Code
    printf("\n\n");
    printf("How many elements do you want in the integer array? \n\n");
    scanf("%u", &num_elements);

    printf("\n\n");
    MyAlloc(&piArray, num_elements);

    printf("Enter %u elements to fill up your integer array: \n\n", num_elements);
    for (i = 0; i < num_elements; i++)
        scanf("%d", &piArray[i]);

    printf("\n\n");
    printf("The %u elements entered by you in the integer array: \n\n", num_elements);
    for (i = 0; i < num_elements; i++)
        printf("%d\n", piArray[i]);

    printf("\n");
    if (piArray)
    {
        free(piArray);
        piArray = NULL;
        printf("Memory allocated has now been successfully freed!\n\n");
    }
    return (0);
}

void MyAlloc(int** ptr, unsigned int numberOfElements)
{
    //Code
    *ptr = (int*)malloc(numberOfElements * sizeof(int));
    if (*ptr == NULL)
    {
        printf("Could not allocate memory! Exiting...\n\n");
        exit(0);
    }

    printf("MyAlloc() has successfully allocated %zu bytes for the integer array!\n\n", numberOfElements * sizeof(int));
}

