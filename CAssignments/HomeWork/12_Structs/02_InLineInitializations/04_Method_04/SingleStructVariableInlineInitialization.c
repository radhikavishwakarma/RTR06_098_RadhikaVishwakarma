#include <stdio.h>

//DEFINING STRUCT
struct MyData
{
	int i;
	float f;
	double d;
	char c;
};

int main(void)
{

	//variable Declaration
	// 35 Will Be Assigned to 'i' Of 'data_one'
	// 3.9 will be Assigned to 'f' 'data_one'
    // 1.23765 will be Assigned To 'd' of 'data_one'
	// 'A' will be assigned to 'c' of 'data_one'
	struct MyData data_one = { 35 , 3.9f, 1.23765, 'A' }; //Inline Initialization

	// 'P' will be Assigned to 'i' of 'data_two' ... but 'P' is a character (char) and 'i' is Integer..
	//so 'P' is converted into it decimal integer ASCII value (80) and 80 is Assigned to 'i' of data_two
	// 6.2 will be assigned to 'f' of 'data_two'
	//12.199523 will be assigned of 'd' of 'data_two'
	// 68 will be assigned to 'c' of 'data_two' ...but 68 is an Integer (int) and 'c' is a 'char'.. so 68 is an
	// is decimal ASCII value and its corresponding character ('D') is assigned to 'c' of data_two
	struct MyData data_two = { 'P', 6.2f, 12.199523, 68 }; //Inline Initialization

	// 36 Will be Assigned to 'i' of 'data_three'
	// 'G' is 'char' but 'f' of 'data_three' is float..hence 'G' is converted to its decimal integer ASCII value (71)
	// and this turn is converted to 'float' (71.00000000) and then it will be Assigned to 'f' 'data_three'
	//0.000000 will be assigned to 'd' of 'data_three'
	// No character will be assigned to 'c' of 'data_three'
	struct MyData data_three = { 36 , 'G' }; //Inline Initiallization

	//70 will be assigned to 'i' of 'data_four'
	//0.000000 will be Assigned to 'f' of 'data_four'
	//0.000000 will be Assigned to 'd' of 'data_four'
	//No character will be Assigned to 'c' of 'data_four'
	struct MyData data_four = { 79 };  //Inline Initiallization

	//Code 
	//Displaying Values Of the Data Members Of 'Strcut MyData'
	printf("\n\n");
	printf("DATA MEMBERS OF 'Struct MyData data_one' ARE: \n\n");
	printf("i = %d\n", data_one.i);
	printf("f = %f\n", data_one.f);
	printf("d = %lf\n",data_one.d);
	printf("c = %c\n", data_one.c);

	printf("\n\n");
	printf("DATA MEMBERS OF 'Struct MyData data_two' ARE: \n\n");
	printf("i = %d\n", data_two.i);
	printf("f = %f\n", data_two.f);
	printf("d = %lf\n", data_two.d);
	printf("c = %c\n", data_two.c);

	printf("\n\n");
	printf("DATA MEMBERS OF 'Struct MyData data_three' ARE: \n\n");
	printf("i = %d\n", data_three.i);
	printf("f = %f\n", data_three.f);
	printf("d = %lf\n", data_three.d);
	printf("c = %c\n", data_three.c);

	printf("\n\n");
	printf("DATA MEMBERS OF 'Struct MyData data_four' ARE: \n\n");
	printf("i = %d\n", data_four.i);
	printf("f = %f\n", data_four.f);
	printf("d = %lf\n", data_four.d);
	printf("c = %c\n", data_four.c);

	return(0);
}


