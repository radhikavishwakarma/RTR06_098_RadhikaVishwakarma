#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//Function Prototype
	int MyStrlen(char[]);

	// Variable Declarations
	char chArray[MAX_STRING_LENGTH]; // A character Array Is A string
	int iStringLength;
	int count_A = 0, count_E = 0, count_I = 0, count_O = 0, count_U = 0; // Initials Counts = 0

	int i;

	//Code 

	//* String Input **
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(chArray, MAX_STRING_LENGTH);

	//* String Output***
	printf("\n\n");
	printf("String Entered By You Is : \n\n");
	printf("%s\n", chArray);

	iStringLength = MyStrlen(chArray);
	for (i = 0; i < iStringLength; i++)
	{
		switch (chArray[i])
		{
		case 'A':
		case 'a':
			count_A++;
			break;
		case 'E':
		case 'e':
			count_E++;
			break;
		case 'I':
		case 'i':
			count_I++;
			break;
		case 'O':
		case 'o':
			count_O++;
			break;
		case 'U':
		case 'u':
			count_U++;
			break;
		default:
			break;
		}
	}
	printf("\n\n");
	printf("In The String Entered By You The Vowels And The Number of Their Occurence are as Follows : \n\n");
	printf("'A' Has Occured = %d Times !!!\n\n", count_A);
	printf("'E' Has Occured = %d Times !!!\n\n", count_E);
	printf("'I' Has Occured = %d Times !!!\n\n", count_I);
	printf("'O' Has Occured = %d Times !!!\n\n", count_O);
	printf("'U' Has Occured = %d Times !!!\n\n", count_U);

	return(0);
}

int MyStrlen(char str[])
{
	//Variable Declarations
	int j;
	int string_length = 0;

	//Code
	//**Determining Exact Length Of the The String By Deetecting The First Occurence of NULL- TERMINATING CHARACTER (\0)
	for (j = 0; j < MAX_STRING_LENGTH; j++)
	{
		if (str[j] == '0')
			break;
		else
			string_length++;
	}
	return(string_length);
}