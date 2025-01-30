#include <stdio.h>

//Defining Struct
struct MyData
{
	int i;
	float f;
	double d;
};

struct MyData data;//Declaring a Single Strcut Variable type 'struct MyData' Globally...

// Variable Declarations
int i_size;
int f_size;
int d_size;
int struct_MyData_size;

int main(void)
{
	// Code
	//Assigning Data Values To Data Members of 'Struct MyData'
	data.i = 16;
	data.f = 99.99f;
	data.d = 2.6665;

	//Displaying Values Of the Data Members of 'Struct MyData'
	printf("\n\n");
	printf("DATA MEMBERS OF 'Struct MyData' ARE: \n\n");
	printf("i = %d\n", data.i);
	printf("f = %f\n", data.f);
	printf("d = %lf\n", data.d);

	// Calculating Sizes (In bytes) of The Data Members of 'Struct MyData'
	printf("\n\n");
	printf("SIZES (in bytes) Of Data Members of 'Struct MyData' ARE : \n\n");
	printf("Size of 'i' = %d bytes\n", i_size);
	printf("Size of 'f' = %d bytes\n", f_size);
	printf("Size of 'd' = %d bytes\n", d_size);

	//Calculating Size (In Bytes) Of The Entire 'Struct MyData'
	struct_MyData_size = sizeof(struct MyData); // Can Also Struct name -> sizeof(MyData)

	// Displaying sizes (In Bytes) of The Entire 'Struct MyData'
	printf("\n\n");
	printf("Size Of 'Struct MyData' : %d bytes\n\n", struct_MyData_size);

	return(0);

}

