#include <stdio.h>

// Defining Struct....
struct MyPoint
{
	int x;
	int y;
};

//Defining Struct
struct MyPointProperties
{
	int quadrant;
	char axis_location[10];
};

int main(void)
{
 //Variable Declarations
struct MyPoint point; //Declaring a Single Variable Of Type 'Struct MyPoint' locally...

struct MyPointProperties point_properties; //Declaring a single Variable Of Type 'Struct MyPointProperties' locally...

	//Code
	// User Input For The Data Members of 'Struct MyPoint' Variable 'point_A'
	printf("\n\n");
	printf("Enter X- Cordinate For A point : ");
	scanf("%d", &point.x);
	printf("Enter Y- Cordinate For A point : ");
	scanf("%d", &point.y);

	printf("\n\n");
	printf("point Co-ordinates (x,y) Are : (%d,%d) !!!\n\n", point.x, point.y);

	if (point.x == 0 && point.y == 0)
		printf("The point Is The Origin (%d,%d) !!!\n", point.x, point.y);
	else // Atleast One Of The Two Values (either 'X' or 'Y' Or BOTH) is a non-zero value...
	{
		if (point.x == 0) //If 'X' Iz ZERO...Obviously 'Y' Is The Non-Zero value
		{
			if (point.y < 0) //If 'Y' is -ve
				strcpy(point_properties.axis_location, "Negative Y");

			if (point.y > 0) //If 'Y' is +ve
				strcpy(point_properties.axis_location, "Positive Y");

			point_properties.quadrant = 0; //A Point Lying On Any Of The Co-ordinates Axes Is Not A Part Of Any Quadrant....
			printf("The Point Lies On The %s Axis !!\n\n", point_properties.axis_location);
		}
		else if (point.y == 0) // If 'Y' Is Zero....Obviously 'X' Is The Non-Zero Value
		{
			if (point.x < 0) //If 'X' Is -ve
				strcpy(point_properties.axis_location, "Negative X");

			if (point.x > 0) //If 'X' is +ve
				strcpy(point_properties.axis_location, "Positive X");

			point_properties.quadrant = 0; //A Point Lying On Any Of The Co-ordinates Axes Is Not A Part Of Any Quadrant....
			printf("The Point Lies On The %s Axis !!\n\n", point_properties.axis_location);

		}
		else // Both 'X' And 'Y' Are NON-ZERO
		{
			point_properties.axis_location[0] = '\0'; // A Point Lying in Any of The 4 Quadrants cannot be Lying on Any Of The Co-ordinates axes..

			if (point.x > 0 && point.y > 0)  //'X' Is +ve and 'Y' is +ve
				point_properties.quadrant = 1;

			else if (point.x < 0 && point.y > 0) //'X' Is +ve and 'Y' is +ve
				point_properties.quadrant = 2;

			else if (point.x < 0 && point.y < 0)  //'X' Is +ve and 'Y' is +ve
				point_properties.quadrant = 3;

			else
				point_properties.quadrant = 4;

			printf("The Point Lies In Quadrant Number %d !!!\n\n", point_properties.quadrant);

		}
	}
	return(0);
}


