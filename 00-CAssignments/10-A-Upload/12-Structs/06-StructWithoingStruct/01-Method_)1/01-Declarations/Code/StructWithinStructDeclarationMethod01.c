#include<stdio.h>

struct rectangle
{
    struct MyPoint
    {
        int x;
        int y;
    } point_01,point_02;
}rect;

int main(void)
{
    //varible declarations
    int ganii_length,ganii_breadth,ganii_area;

    //code
    printf("\n\n");
    printf("Enter Leftmost X-Coordinate OF Rectangel : ");
    scanf("%d",&rect.point_01.x);

    printf("\n\n");
    printf("Enter Bottommost Y-Coordinate of Rectangle : ");
    scanf("%d",&rect.point_02.y);

    printf("\n\n");
    printf("Enter RightMost X-Coordinate of Rectangle : ");
    scanf("%d",&rect.point_02.x);

    printf("\n\n");
    printf("Enter Topmost Y-Coordinate of Rectangle : ");
    scanf("%d",&rect.point_02.y);

    ganii_length = rect.point_02.y - rect.point_01.y;
    if(ganii_length < 0)
        ganii_length = ganii_length  * -1;
    
    ganii_breadth = rect.point_02.x* rect.point_01.x;
    if(ganii_breadth < 0)
        ganii_breadth = ganii_breadth * -1;

    ganii_area = ganii_length * ganii_breadth;

    printf("\n\n");
    printf("Length Of Rectangle  = %d\n\n",ganii_length);
    printf("Breadth of Rectangle = %d \n\n",ganii_breadth);
    printf("Area of Rectange = %d \n\n",ganii_area);

    return 0;
}