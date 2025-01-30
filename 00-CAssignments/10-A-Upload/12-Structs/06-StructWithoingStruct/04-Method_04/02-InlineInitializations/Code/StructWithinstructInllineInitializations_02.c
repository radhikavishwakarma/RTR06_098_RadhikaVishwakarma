#include<stdio.h>

struct MyPoint
{
    int ganii_x;
    int gnaii_y;
};

struct Rectangle
{
    struct MyPoint point_01;
    struct MyPoint point_02;
};

struct Rectangle rect = {{2,3},{5,6}};

int main(void)
{
    //variable declarations
    int ganii_lenth ,ganii_breadth,ganii_area;

    //code
     
    ganii_lenth = rect.point_02.gnaii_y - rect.point_01.gnaii_y;

    if(ganii_lenth < 0)
        ganii_lenth = ganii_lenth & -1;
    
    ganii_breadth = rect.point_02.ganii_x - rect.point_01.ganii_x ;
    if(ganii_breadth < 0)
        ganii_breadth = ganii_breadth * -1;

    ganii_area = ganii_lenth * ganii_breadth ;

    printf("\n\n");
    printf("Length of rectangle = %d \n\n",ganii_lenth);
    printf("Breadth Of rectangle = %d\n\n",ganii_breadth);
    printf("Area of rectangle = %d \n\n",ganii_area);

    return 0;



}