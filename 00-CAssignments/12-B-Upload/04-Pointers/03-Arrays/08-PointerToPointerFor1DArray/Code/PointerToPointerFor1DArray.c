#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    void MyAlloc(int **ptr,unsigned int numberofElements);

    //variable declarations
    int *piArray = NULL;
    unsigned int num_elements;
    int i;

    //code
    printf("\n\n");
    printf("How many Elements You want in Intgers array ? \n\n");
    scanf("%u",&num_elements);

    printf("\n\n");
    MyAlloc(&piArray ,num_elements);

    printf("Enter %u Elements To Fill Up Your Intger Array : \n\n",num_elements);
    for(i=0; i <num_elements; i++)
        scanf("%d",&piArray[i]);
    
    printf("\n\n");
    printf("The %u Elements Entered By You In The Intger Array : \n\n",num_elements);
    for(i =0; i < num_elements; i++)
        printf("%u\n",piArray[i]);
    
    printf("\n");
    if(piArray)
    {
        free(piArray);
        piArray = NULL;
        printf("Memory allocated Has Now Been successfully free !!\n\n");
    }
    return 0;

    
}

void MyAlloc(int **ptr,unsigned int numberOfElements)
{
    *ptr = (int*)malloc(numberOfElements *sizeof(int));
    if(*ptr = NULL)
    {
        printf("Could Not Allocate Memeory !!!Existing ..\n\n");
        exit(0);
    }

    printf("MyAlloc() has Suucessfully allocated %zu Bytes for Intger Array !!!|n\n",(numberOfElements *sizeof(int)));
}