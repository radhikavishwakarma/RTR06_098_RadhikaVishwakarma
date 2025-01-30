#include<stdio.h>

int main(void)
{
    //varible declarations
    int length ,breadth,area;

    struct Rectangle
    {
        struct MyPoint
        {
            int x;
            int y;
        } point_01,point_02;
    }rect;

    //code 
    printf("\n\n");
    printf("Enter Leftmost X- Coordinate Of rectangle : ");
    scanf("%d",&rect.point_01.x);

    printf("\n\n");
    printf("Enter Bottommosy  Y-Coordinate Of rectangle : ");
    scanf("%d",&rect.point_01.y);

    printf("\n\n");
    printf("Enter Rightmost X-Coordinate of Rectangle : ");
    scanf("%d",&rect.point_02.x);

    printf("\n\n");
    printf("Enter Topmost Y-Coordinate Of rectangle : ");
    scanf("%d",&rect.point_02.y);

    length = rect.point_02.y - rect.point_01.y;
    if(length < 0)
        length = length * -1;
    
    breadth =rect.point_02.x - rect.point_01.x;
    if(breadth < 0)
        breadth = breadth * -1;
    
    area = length * breadth;

    printf("\n\n");
    printf("Length of Rectangle = %d \nn\n",length);
    printf("Breadth Of recatngle = %d \n\n",breadth);
    printf("Area of Rectangle  = %d \n\n",area);

    return 0;
}