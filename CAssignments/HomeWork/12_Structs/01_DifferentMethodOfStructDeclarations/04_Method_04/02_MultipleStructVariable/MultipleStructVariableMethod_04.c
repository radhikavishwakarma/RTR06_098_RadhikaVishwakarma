#include <stdio.h>

// Defining Struct....
struct MyPoint
{
	int x;
	int y;
};

int main(void)
{
// Variable Declarations
 struct MyPoint point_A, point_B, point_C, point_D, point_E; //Declaring 5 Struct Variable Of Type 'Struct MyPoint' locally...

	//Code
	//Assigning Data Values to the Data Members Of 'Struct MyPoint' variable 'point_A'
	point_A.x = 3;
	point_A.y = 0;

	//Assigning Data Values to the Data Members Of 'Struct MyPoint' variable 'point_B'
	point_B.x = 1;
	point_B.y = 2;

	//Assigning Data Values to the Data Members Of 'Struct MyPoint' variable 'point_C'
	point_C.x = 9;
	point_C.y = 6;

	//Assigning Data Values to the Data Members Of 'Struct MyPoint' variable 'point_D'
	point_D.x = 8;
	point_D.y = 2;

	//Assigning Data Values to the Data Members Of 'Struct MyPoint' variable 'point_E'
	point_E.x = 11;
	point_E.y = 8;

	//Displaying Values Of the Data Members of 'Struct MyPoint' (All Variables)
	printf("\n\n");
	printf("Co-ordinates (x,y) of Point 'A' Are : (%d, %d)\n\n", point_A.x, point_A.y);
	printf("Co-ordinates (x,y) of Point 'B' Are : (%d, %d)\n\n", point_B.x, point_B.y);
	printf("Co-ordinates (x,y) of Point 'C' Are : (%d, %d)\n\n", point_C.x, point_C.y);
	printf("Co-ordinates (x,y) of Point 'D' Are : (%d, %d)\n\n", point_D.x, point_D.y);
	printf("Co-ordinates (x,y) of Point 'E' Are : (%d, %d)\n\n", point_E.x, point_E.y);

	return(0);
}



