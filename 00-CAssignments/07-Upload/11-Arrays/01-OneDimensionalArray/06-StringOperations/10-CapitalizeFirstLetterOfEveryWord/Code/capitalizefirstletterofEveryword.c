#include <stdio.h>

#define MAX_STRING_LENGTH 512

#define SPACE ' '

#define FULLSTOP '.'
#define COMMA ','
#define EXCLAMATION '!'
#define QUESTION_MARK '?'

int main(void)
{
	// Function Prototype
	int MyStrlen(char[]);
	char MyToUpper(char);

		// Variable Declarations
		char chArray[MAX_STRING_LENGTH], chArray_CapitalizedFirstLetterOfEveryWord[MAX_STRING_LENGTH];
		// A character Array is A String
	int iStringLength;
	int i, j;

	// Code

	// String Input
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(chArray, MAX_STRING_LENGTH);

	iStringLength = MyStrlen(chArray);
	j = 0;
	for (i = 0; i < iStringLength; i++)
	{
		if (i == 0) //First Letter Of Any Sentence Must Be A Capital Letter
			chArray_CapitalizedFirstLetterOfEveryWord[j] = MyToUpper(chArray[i]);

		else if (chArray[i] == SPACE) // First Letter Of Every Word In The Sentence Must Be A Capital Letter Words Are Seprated By Spaces
		{
			chArray_CapitalizedFirstLetterOfEveryWord[j] = chArray[i];
			chArray_CapitalizedFirstLetterOfEveryWord[j + 1] = MyToUpper(chArray[i + 1]);

			// Since Already Two Characters (At Indices 'i' And 'i+1' Have Been Considered In This else if Block..We Are Extra-Incrementing 'i' And 'j' By 1)
			j++;
			i++;
		}
		else if ((chArray[i] == FULLSTOP || chArray[i] == COMMA || chArray[i] == EXCLAMATION || chArray[i] == QUESTION_MARK) && (chArray[i] != SPACE))
			// First Letter Of Every Word After Punctuation Mark, In the Sentence Must Be A Capital Letter WOrds
			// Are Seprated By Punctuations
		{
			chArray_CapitalizedFirstLetterOfEveryWord[j] = chArray[i];
			chArray_CapitalizedFirstLetterOfEveryWord[j + 1] = SPACE;
			chArray_CapitalizedFirstLetterOfEveryWord[j + 2] = MyToUpper(chArray[i + 1]);

			// Since Already Two character (At Indices 'i' And i + 1 Have been Considered  In This else if block we Are Extra-Incrementing 'i' By 1
			// Since Already Three Characters (At Indices 'j' And (j+1) and   (j + 2 ) have been Considered In this else if block... Are Extra-Incremnting 'j' by 2

			j = j + 2;
				i++;
		}
		else
			chArray_CapitalizedFirstLetterOfEveryWord[j] = chArray[i];

		j++;
		
	}
	chArray_CapitalizedFirstLetterOfEveryWord[j] = '\0';

	// * String Output *
	printf("\n\n");
	printf("String Entered By You Is : \n\n");
	printf("%s\n", chArray);

	printf("\n\n");
	printf("String After Capitalizing First Letter Of Every Word : \n\n");
	printf("%s\n", chArray_CapitalizedFirstLetterOfEveryWord);

	return(0);
}

int MyStrlen(char str[])
{
	// Variable Declarations
	int j;
	int string_length = 0;

	// Code 
	// ** Determining Exact Length Of The String By You Detecting The First Occurence Of Null Terminating
	// Character (\0) **
	for (j = 0; j < MAX_STRING_LENGTH; j++)
	{
		if (str[j] == '\0')
			break;
		else
			string_length++;
	}
	return(string_length);
}

char MyToUpper(char ch)
{
	// Variable Declarations
	int num;
	int c;

	// Code

	// ASCII VALUE Of 'a' (97) - ASCII VALUE OF 'A' (65) = 32
	//THIS SUBSTRACTION WILL GIVE THE EXACT DIFFERENCE BETWEEN THE UPPER AND LOWER CASE
	//COUNTERPARTS OF EACH LETTER OF THE ALPHABET
	// IF THIS RESULTANT ASCII VALUE WILL BE THAT IF ITS UPPER CASE COUNTER PART ,HENCE HELPING US TO FIND
	// ITS UPPER CASE LETTER !!!
	//ASCII VALUES OF 'a' TO 'z' => 97 TO 122
	//ASCII VALUES OF 'A' TO 'z' => 65 TO 90
	num = 'a' - 'A';

	if ((int)ch >= 97 && (int)ch <= 122)
	{
		c = (int)ch - num;
		return((char)c);
	}
	else
		return(ch);
}