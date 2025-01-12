# include <stdio.h>

struct MyPoint
{
	int x;
	int y;
};

int main(void)
{
	//Variable declarations
	struct MyPoint point_A, point_B, point_C, point_D, point_E; //Declaring a 5 struct variables

	//Code
	//User Input for the Data Members of 'struct MyPoint' Variable 'point_A'
	printf("\n\n");
	printf("Enter X-Coordinates for point 'A' :");
	scanf("%d", &point_A.x);
	printf("Enter Y-Coordinate for point 'A' :");
	scanf("%d", &point_A.y);

	//User Input for the Data Members of 'struct MyPoint' Variable 'point_B'
	printf("\n\n");
	printf("Enter X-Coordinates for point 'B' :");
	scanf("%d", &point_B.x);
	printf("Enter Y-Coordinate for point 'B' :");
	scanf("%d", &point_B.y);

	//User Input for the Data Members of 'struct MyPoint' Variable 'point_C'
	printf("\n\n");
	printf("Enter X-Coordinates for point 'C' :");
	scanf("%d", &point_C.x);
	printf("Enter Y-Coordinate for point 'C' :");
	scanf("%d", &point_C.y);

	//User Input for the Data Members of 'struct MyPoint' Variable 'point_D'
	printf("\n\n");
	printf("Enter X-Coordinates for point 'D' :");
	scanf("%d", &point_D.x);
	printf("Enter Y-Coordinate for point 'D' :");
	scanf("%d", &point_D.y);

	//User Input for the Data Members of 'struct MyPoint' Variable 'point_E'
	printf("\n\n");
	printf("Enter X-Coordinates for point 'E' :");
	scanf("%d", &point_E.x);
	printf("Enter Y-Coordinate for point 'E' :");
	scanf("%d", &point_E.y);

	//Displaying Values Of The Data Members Of 'Struct-MyPoint' (All variables)
	printf("\n\n");

	printf("Co-ordinates (x,y) of point 'A' Are : (%d,%d)\n\n", point_A.x, point_A.y);
	printf("Co-ordinates (x,y) of point 'B' Are : (%d,%d)\n\n", point_B.x, point_B.y);
	printf("Co-ordinates (x,y) of point 'C' Are : (%d,%d)\n\n", point_C.x, point_C.y);
	printf("Co-ordinates (x,y) of point 'D' Are : (%d,%d)\n\n", point_D.x, point_D.y);
	printf("Co-ordinates (x,y) of point 'E' Are : (%d,%d)\n\n", point_E.x, point_E.y);

	return(0);

}