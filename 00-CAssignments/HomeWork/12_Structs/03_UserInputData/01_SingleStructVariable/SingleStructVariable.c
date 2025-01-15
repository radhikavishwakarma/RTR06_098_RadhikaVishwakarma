#include <stdio.h>

//DEFINING STRUCT
struct MyData
{
	int i;
	float f;
	double d;
	char ch;
};

int main(void)
{
	//Variable declarations
	struct MyData data; //declaring a single struct variable

	//Code 
	//User input for values of data members of 'struct MyData'
	printf("\n\n");

	printf("Enter Integer Value for Data Member 'i' of 'struct MyData' : \n");
	scanf("%d", &data.i);

	printf("Enter Integer Value for Data Member 'f' of 'struct MyData' : \n");
	scanf("%f", &data.f);

	printf("Enter Integer Value for Data Member 'd' of 'struct MyData' : \n");
	scanf("%lf", &data.d);

	printf("Enter Integer Value for Data Member 'c' of 'struct MyData' : \n");
	data.ch = getch();

	//Displaying Values Of the Data Members Of 'Strcut MyData'
	printf("\n\n");
	printf("DATA MEMBERS OF 'Struct MyData' ARE: \n\n");
	printf("i = %d\n", data.i);
	printf("f = %f\n", data.f);
	printf("d = %lf\n", data.d);
	printf("c = %c\n\n", data.ch);
	return(0);
}


