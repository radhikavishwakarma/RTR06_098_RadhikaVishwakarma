#include<stdio.h>

struct Mypoint
{
    int ganii_x;
    int ganii_y ;
};

struct Rectangle
{
    struct Mypoint point_01,point_02;
};

int main(void)
{
    //variable declratons
    int ganii_length ,ganii_breadth,ganii_area;
    struct Rectangle rect;

    //code
    printf("\n\n");
    printf("Enter LeftMost X-Coordinates Of Rectangle : ");
    scanf("%d",&rect.point_01.ganii_x);

    printf("\n\n");
    printf("Enter Bottommost Y-Coordinate Of Rectangle : ");
    scanf("%d",&rect.point_01.ganii_y);

    printf("\n\n");
    printf("Enter Rightmost X-Coordinate of Rectangel : ");
    scanf("%d",&rect.point_02.ganii_x);

    printf("\n\n");
    printf("Enter Topmost Y-Coordinate Of rectangel : ");
    scanf("%d",&rect.point_02.ganii_y);

    ganii_length =  rect.point_02.ganii_y - rect.point_01.ganii_y;
    if(ganii_length < 0)
        ganii_length= ganii_length * -1;
    ganii_breadth = rect.point_02.ganii_x - rect.point_01.ganii_x;
    if(ganii_breadth < 0)
        ganii_breadth = ganii_breadth * -1;
    
    ganii_area = ganii_length *ganii_breadth;

    printf("\n\n");
    printf("Length Of rectangle = %d \n\n",ganii_length);
    printf("Breadth Of rectangle %d \n\n",ganii_breadth);
    printf("Area of rectangle = %d \n\n",ganii_area);

    return 0;

}