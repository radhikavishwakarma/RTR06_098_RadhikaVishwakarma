#include<stdio.h>

#define INT_ARRAY_SIZE 10
#define FLOAT_ARRAY_SIZE 5
#define CHAR_ARRAY_SIZE 26

#define NUM_STRINGS 10
#define MAX_CHARACTERS_PER_STRING 20

#define ALPHABET_BEGININGS 65 //'A'

struct MyDataOne
{
    int iArray[INT_ARRAY_SIZE];
    float fArray[FLOAT_ARRAY_SIZE];
};

struct MyDataTwo
{
    char cArray[CHAR_ARRAY_SIZE];
    char strArrray[NUM_STRINGS][MAX_CHARACTERS_PER_STRING];
};

int main(void)
{
    //varible declarations
    struct MyDataOne data_one;
    struct MyDataTwo data_two;
    int i;

    //code
    // *** PIECE-MEAL ASSIGNMENT (HARD-CODED)
    data_one.fArray[0] = 0.1f;
    data_one.fArray[1] = 1.2f;
    data_one.fArray[2] = 2.3f;
    data_one.fArray[3] = 3.4f;
    data_one.fArray[4] = 4.5f;

    // **LOOP ASSIGNMENT (USER INPUT )*****
    printf("\n\n");
    printf("ENter %d Intgers : \n\n",INT_ARRAY_SIZE);
    for(i =0; i < INT_ARRAY_SIZE; i++)
        scanf("%d",&data_one.iArray[i]);
    
    //*** Loop Assignment (Hard-coded) */
    for(i =0; i < CHAR_ARRAY_SIZE; i++)
        data_two.cArray[i] = (char)(i+ALPHABET_BEGININGS);
  
    // ***Piece meal Assignmemts (Hard-coded)
    strcpy(data_two.strArrray[0],"Welcome !!!");
    strcpy(data_two.strArrray[1],"This");
    strcpy(data_two.strArrray[2],"Is");
    strcpy(data_two.strArrray[3],"ASTROMEDICOMPS");
    strcpy(data_two.strArrray[4],"Real");
    strcpy(data_two.strArrray[5],"Time");
    strcpy(data_two.strArrray[6],"Rendering");
    strcpy(data_two.strArrray[7],"batch");
    strcpy(data_two.strArrray[8],"Of");
    strcpy(data_two.strArrray[9],"2024-2026 !!!!");

    // **DISPLAYING DATA MEMEBER OS struct DataOne And Their values
    printf("n\n");
    printf("Members of struct DataOne Alongwith Their Assigned Value Are :\n\n");
    printf("\n\n");
    printf("Intger Array (data_one.iArray[]) :\n\n");
    for(i = 0; i < INT_ARRAY_SIZE; i++)
        printf("data_one.iArray[%d] =%d\n",i,data_one.iArray[i]);
    
    printf("\n\n");
    printf("Floating-Point Array (data_one.fArray[]) : \n\n");
    for(i = 0; i <FLOAT_ARRAY_SIZE; i++)
        printf("data_one.fArray[%d] = %f\n",i,data_one.fArray[i]);

    // *** Displaying data members of struct dataTwo And Their values
    printf("\n\n");
    printf("Members os struct DataTwo Alongwith their assigned values are :\n\n");
    printf("\n\n");

    printf("Character Array (data_two.cArray[]) :\n\n");
    for(i = 0; i <CHAR_ARRAY_SIZE; i++)
        printf("data_two.cArray[%d] = %c\n",i ,data_two.cArray[i]);
    
    printf("\n\n");
    printf("String Array (data_two.strArray[]) : \n\n");
    for(i = 0; i <NUM_STRINGS; i++)
        printf("%s",data_two.strArrray[i]);
    printf("\n\n");

    return 0;

}