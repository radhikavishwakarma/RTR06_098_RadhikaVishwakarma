#include <stdio.h>

#define MAX_STRING_LENGTH 512 

int main(void)
{
	// Function Prototype
	int MyStrlen(char[]);
	void MyStrcpy(char[], char[]);

	// Variable Declarations
	char chArray[MAX_STRING_LENGTH]; // A Character Array Is A String
	int iStringLength;
	int i;
	int word_count = 0, space_count = 0;

	//Code

	// * String Input *
	printf("\n\n");
	printf("Enter A String: \n\n");
	gets_s(chArray, MAX_STRING_LENGTH);

	iStringLength = MyStrlen(chArray);

	for (i = 0; i < iStringLength; i++)
	{
		switch (chArray[i])
		{
		case 32: // 32 THE ASCII VALUE FOR SPACE (' ') CHARACTER
			space_count++;
			break;
		default:
			break;
		}
	}

	word_count = space_count + 1;

	// String Output
	printf("\n\n");
	printf("String Entered By You Is : \n\n");
	printf("%s\n", chArray);

	printf("\n\n");
	printf("Number Of Spaces in the Input String = %d\n\n", space_count);
	printf("Number Of Words in the Input String = %d\n\n", word_count);

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
			string_length++;
	}
	return(string_length);
}