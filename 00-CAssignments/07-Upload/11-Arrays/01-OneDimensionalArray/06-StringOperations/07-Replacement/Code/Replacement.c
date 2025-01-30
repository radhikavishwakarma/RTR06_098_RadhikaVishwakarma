// * This Program Replaces All Vowels In The Input String With The * (Asterisk) Symbol *
// ** For Example Original String 'Dr Vijay Dattaray Gokhale Astromedicomp' Will Become The 
// 'Dr. V*j*y*D*tt*t*r*y* G*k*le *str*m*d*c*mp' 

#include <stdio.h>

#define MAX_STRING_LENGTH 512 

int main(void)
{
	// Function Prototype
	int MyStrlen(char[]);
	void MyStrcpy(char[], char[]);

	// Variable Declarations
	char chArray_Original[MAX_STRING_LENGTH], chArray_VowelsReplaced[MAX_STRING_LENGTH]; // A Character Array Is A String
	int iStringlength;
	int i;

	//Code

	// * String Input *
	printf("\n\n");
	printf("Enter A String: \n\n");
	gets_s(chArray_Original, MAX_STRING_LENGTH);

	// * String output *
	MyStrcpy(chArray_VowelsReplaced, chArray_Original);

	iStringlength = MyStrlen(chArray_VowelsReplaced);

	for (i = 0; i < iStringlength; i++)
	{
		switch (chArray_VowelsReplaced[i])
		{
		case 'A':
		case 'a':
		case 'E':
		case 'e':
		case 'I':
		case 'i':
		case 'O':
		case 'o':
		case 'U':
		case 'u':
			chArray_VowelsReplaced[i] = '*';
			break;
		default:
			break;
		}
	}

		// String Output
		printf("\n\n");
		printf("String Entered By You Is : \n\n");
		printf("%s\n", chArray_Original);

		printf("\n\n");
		printf("String After Replacement of Vowel By * Is : \n\n");
		printf("%s\n", chArray_VowelsReplaced);

		return(0);
}

int MyStrlen(char str[])
{
	// Variable Declarations
	int j;
	int string_length = 0;

	//Code
	//* Determining Exact Length Of the String By Detecting The First Occuerence Of Null-Terminating character (\0) *
	for (j = 0; j < MAX_STRING_LENGTH; j++)
	{
		if (str[j] == '\0')
			break;
		else
			string_length;
	}
	return(string_length);
}

void MyStrcpy(char str_destination[], char str_source[])
{
	// Function Prototype
	int MyStrlen(char[]);

	// Variable Declarations
	int iStringLength = 0;
	int j;

	// code
	iStringLength = MyStrlen(str_source);
	for (j = 0; j < iStringLength; j++)
		str_destination[j] = str_source[j];

	str_destination[j] = '\0';
}