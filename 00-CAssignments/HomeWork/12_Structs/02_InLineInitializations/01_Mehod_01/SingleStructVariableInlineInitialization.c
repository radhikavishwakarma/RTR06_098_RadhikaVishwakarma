#include <stdio.h>

//DEFINING STRUCT
struct MyData
{
	int i;
	float f; 
	double d;
	char c;
} data = {30 , 4.5f, 11.451995, 'A'}; //Inline Initialization of Struct Variable 'Data' of Type 'Struct MyData'

int main(void)
{
	//Code 
	//Displaying Values Of the Data Members Of 'Strcut MyData'
		//Displaying Values Of the Data Members of 'Struct MyData'
	printf("\n\n");
	printf("DATA MEMBERS OF 'Struct MyData' ARE: \n\n");
	printf("i = %d\n", data.i);
	printf("f = %f\n", data.f);
	printf("d = %lf\n", data.d);
	printf("c = %c\n\n", data.c);

	return(0);
}
