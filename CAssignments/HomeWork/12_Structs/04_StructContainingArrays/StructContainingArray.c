#include <stdio.h>

#define INT_ARRAY_SIZE 10
#define FLOAT_ARRAY_SIZE 5
#define CHAR_ARRAY_SIZE 26

#define NUM_STRINGS 10
#define MAX_CHARACTERS_PER_STRINGS 20

#define ALPHABET_BEGINING 65 // 'A'

struct MyDataOne
{
	int iArray[INT_ARRAY_SIZE];
	float fArray[FLOAT_ARRAY_SIZE];

};

struct MyDataTwo
{
	char cArray[CHAR_ARRAY_SIZE];
	char strArray[NUM_STRINGS][MAX_CHARACTERS_PER_STRINGS];

};

int main(void)
{
	//variable Declaration
	struct MyDataOne data_one;
	struct MyDataTwo data_two;
	int i;

	// code
	//***Piece-Meal Assignment (Hard-Coded) ***
	data_one.fArray[0] = 0.1f;
	data_one.fArray[1] = 1.2f;
	data_one.fArray[2] = 2.3f;
	data_one.fArray[3] = 3.4f;
	data_one.fArray[4] = 4.5f;

	//*** LOOP ASSIGNMENT (USER INPUT) ***
	printf("\n\n");
	printf("Enter %d Integers : \n\n",INT_ARRAY_SIZE);
	for (i = 0; i < INT_ARRAY_SIZE; i++)
		scanf("%d", &data_one.iArray[i]);

		//**Loop assignment (HARD CODED) ***
	for (i = 0; i < CHAR_ARRAY_SIZE; i++)
		data_two.cArray[i] = (char)(i + ALPHABET_BEGINING);

		//***PIECE-MEAL ASSIGNEMENT (HARD-CODED) ***
	strcpy(data_two.strArray[0], "Welcome !!!");
	strcpy(data_two.strArray[1], "THIS");
	strcpy(data_two.strArray[2], "IS");
	strcpy(data_two.strArray[3], "ASTROMEDICOMP");
	strcpy(data_two.strArray[4], "REAL");
	strcpy(data_two.strArray[5], "TIME");
	strcpy(data_two.strArray[6], "RENDERING");
	strcpy(data_two.strArray[7], "BATCH");
	strcpy(data_two.strArray[8], "OF");
	strcpy(data_two.strArray[9], "2024 - 2025 !!!");

	//Displaying Data Members of 'struct DataOne' AND THEIR VALUES ***
	printf("\n\n");
	printf("Members of 'Struct DataOne' Alongwith Their Assigned Values Are : \n\n");

	printf("\n\n");
	printf("Integer Array (data_one.iAraay[]) : \n\n");
	for (i = 0; i < INT_ARRAY_SIZE; i++)
		printf("data_one.iArray[%d] = %d\n", i, data_one.iArray[i]);

	printf("\n\n");
	printf("Floating-Point Array (data_one.fArray[]) : \n\n");
	for (i = 0; i < FLOAT_ARRAY_SIZE; i++)
		printf("data_one.fArray[%d] = %f\n", i, data_one.fArray[i]);

		// **** Displaying Data Members of Struct DataTwo' And Their Values ****
	printf("\n\n");
	printf("Members Of 'Struct DatTwo' AlongWith Thier Assigned Values are :\n\n");

	printf("\n\n");
	printf("Character Array (data_two.cArray[]) : \n\n");
	for (i = 0; i < CHAR_ARRAY_SIZE; i++)
		printf("data_two.cArray[%d] = %c\n", i, data_two.cArray[i]);


	printf("\n\n");
	printf("String Array (data_two.cArray[]) : \n\n");
	for (i = 0; i < NUM_STRINGS; i++)
		printf("%s", data_two.strArray[i]);

	printf("\n\n");

	return(0);

}
