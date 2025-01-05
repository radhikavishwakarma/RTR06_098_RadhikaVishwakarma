#include <stdio.h>

int main(void)
{
    // variable declarations
    int iArrayOne[10];
    int iArrayTwo[10];

    // code
    // ***** iArrayOne *****
    iArrayOne[0] = 3;
    iArrayOne[1] = 6;
    iArrayOne[2] = 9;
    iArrayOne[3] = 12;
    iArrayOne[4] = 15;
    iArrayOne[5] = 18;
    iArrayOne[6] = 21;
    iArrayOne[7] = 24;
    iArrayOne[8] = 27;
    iArrayOne[9] = 30;

    printf("\n\n");
    printf("Piece-meal (Hard-coded) Assignment and Display of Elements to Array 'iArrayOne[]': \n\n");
    printf("1st Element of Array of 'iArrayOne[]' or Element at 0th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[0]);
    printf("2nd Element of Array of 'iArrayOne[]' or Element at 1th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[1]);
    printf("3rd Element of Array of 'iArrayOne[]' or Element at 2th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[2]);
    printf("4th Element of Array of 'iArrayOne[]' or Element at 3th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[3]);
    printf("5th Element of Array of 'iArrayOne[]' or Element at 4th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[4]);
    printf("6th Element of Array of 'iArrayOne[]' or Element at 5th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[5]);
    printf("7th Element of Array of 'iArrayOne[]' or Element at 6th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[6]);
    printf("8th Element of Array of 'iArrayOne[]' or Element at 7th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[7]);
    printf("9th Element of Array of 'iArrayOne[]' or Element at 8th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[8]);
    printf("10th Element of Array of 'iArrayOne[]' or Element at 9th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[9]);

    // ***** iArrayTwo *****
    printf("\n\n");
    printf("Enter the 1st Element of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo[0]);
    printf("Enter the 2nd Element of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo[1]);
    printf("Enter the 3rd Element of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo[2]);
    printf("Enter the 4th Element of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo[3]);
    printf("Enter the 5th Element of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo[4]);
    printf("Enter the 6th Element of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo[5]);
    printf("Enter the 7th Element of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo[6]);
    printf("Enter the 8th Element of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo[7]);
    printf("Enter the 9th Element of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo[8]);
    printf("Enter the 10th Element of Array 'iArrayTwo[]' : ");
    scanf("%d", &iArrayTwo[9]);


    printf("\n\n");
    printf("Piece-meal (User Input) Assignment and Display of Elements to Array 'iArrayTwo[]': \n\n");
    printf("1st Element of Array of 'iArrayTwo[]' or Element at 0th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[0]);
    printf("2nd Element of Array of 'iArrayTwo[]' or Element at 1th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[1]);
    printf("3rd Element of Array of 'iArrayTwo[]' or Element at 2th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[2]);
    printf("4th Element of Array of 'iArrayTwo[]' or Element at 3th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[3]);
    printf("5th Element of Array of 'iArrayTwo[]' or Element at 4th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[4]);
    printf("6th Element of Array of 'iArrayTwo[]' or Element at 5th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[5]);
    printf("7th Element of Array of 'iArrayTwo[]' or Element at 6th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[6]);
    printf("8th Element of Array of 'iArrayTwo[]' or Element at 7th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[7]);
    printf("9th Element of Array of 'iArrayTwo[]' or Element at 8th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[8]);
    printf("10th Element of Array of 'iArrayTwo[]' or Element at 9th Index of Array 'iArrayTwo[]' = %d\n", iArrayOne[9]);


    return 0;
}