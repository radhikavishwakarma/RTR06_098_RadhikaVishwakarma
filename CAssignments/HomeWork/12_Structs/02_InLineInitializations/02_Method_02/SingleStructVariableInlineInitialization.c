#include <stdio.h>

//DEFINING STRUCT
struct MyData
{
	int i;
	float f;
	double d;
	char c;
};

struct MyData data = { 9, 8.2f, 9.61998, 'P' };

int main(void)
{
	//Code 
	//Displaying Values Of the Data Members Of 'Strcut MyData'
	printf("\n\n");
	printf("DATA MEMBERS OF 'Struct MyData' ARE: \n\n");
	printf("i = %d\n", data.i);
	printf("f = %f\n", data.f);
	printf("d = %lf\n", data.d);
	printf("c = %c\n\n", data.c);
	return(0);
}
