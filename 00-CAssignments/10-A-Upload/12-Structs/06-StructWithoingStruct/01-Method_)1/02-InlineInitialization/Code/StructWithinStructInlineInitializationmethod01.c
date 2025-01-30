#include<stdio.h>

struct Rectangle
{
    struct MyPoint
    {
        int ganii_x;
        int ganii_y;
    } point_01,point_02;
}rect = {{2,3},{5,6}};

int main(void)
{
    //varible declrations
    int ganii_length,ganii_breadth,ganii_area;

    //code
    ganii_length = rect.point_02.ganii_y -  rect.point_01.ganii_y;
    if(ganii_length < 0)
        ganii_length = ganii_length * -1;
    
    ganii_breadth = rect.point_02.ganii_x - rect.point_01.ganii_x;
    if(ganii_breadth < 0)
        ganii_breadth = ganii_breadth * -1;
    
    ganii_area = ganii_length * ganii_breadth ;

    printf("\n\n");
    printf("Length Of rectangle  = %d \n\n",ganii_length);
    printf("Breadth Of rectangle = %d \n\n",ganii_breadth);
    printf("Area of rectangle =%d \n\n",ganii_area);

    return 0;

}