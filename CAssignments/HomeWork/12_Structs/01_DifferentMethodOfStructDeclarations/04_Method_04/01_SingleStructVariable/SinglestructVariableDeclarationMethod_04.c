#include <stdio.h>

 //Defining Struct
struct MyData
{
	int i;
	float f;
	double d;
	char c;
};

int main(void)
{
	// Variable Declarations
	struct MyData data; //Declaring a Single Strcut Variable type 'struct MyData' locally...
	int i_size;
	int f_size;
	int d_size;
	int struct_MyData_size;

	// Code
	//Assigning Data Values To Data Members of 'Struct MyData'
	data.i = 30;
	data.f = 11.45f;
	data.d = 1.2995;

	//Displaying Values Of the Data Members of 'Struct MyData'
	printf("\n\n");
	printf("DATA MEMBERS OF 'Struct MyData' ARE: \n\n");
	printf("i = %d\n", data.i);
	printf("f = %f\n", data.f);
	printf("d = %lf\n", data.d);

	//calculating Sizes (In bytes) of The Data Members of 'Struct MyData'
	i_size = sizeof(data.i);
	f_size = sizeof(data.f);
	d_size = sizeof(data.d);

	// Displaying Sizes (In bytes) of The Data Members of 'Struct MyData'
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


