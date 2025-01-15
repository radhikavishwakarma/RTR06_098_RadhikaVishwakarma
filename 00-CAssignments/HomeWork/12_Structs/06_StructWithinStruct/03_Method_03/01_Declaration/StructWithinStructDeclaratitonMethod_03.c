#include <stdio.h>

int main(void)
{
	//Variable Declarations
	int length, breadth, area;

	struct Rectangle
	{
		struct MyPoint
		{
			int x;
			int y;
		}point_01 , point_02;
	}rect;

	//Code
	printf("\n\n");
	printf("Enter Leftmost X-Coordinates Of Rectangle : ");
	scanf("%d", &rect.point_01.x);

	printf("\n\n");
	printf("Enter Bottommost Y-Coordinates Of Rectangle : ");
	scanf("%d", &rect.point_01.y);

	printf("\n\n");
	printf("Enter Topmost X-Coordinates Of Rectangle : ");
	scanf("%d", &rect.point_02.x);

	printf("\n\n");
	printf("Enter Topmmost Y-Coordinates Of Rectangle : ");
	scanf("%d", &rect.point_02.y);

	length = rect.point_02.y - rect.point_01.y;
	if (length < 0)
		length = length * -1;

	breadth = rect.point_02.x - rect.point_01.x;
	if (breadth < 0)
		breadth = breadth * -1;

	area = length * breadth;

	printf("\n\n");
	printf("Length Of Rectangle = %d\n\n", length);
	printf("Breadth Of Rectangle = %d\n\n", breadth);
	printf("Area Of Rectangle = %d\n\n", area);

	return(0);
}
