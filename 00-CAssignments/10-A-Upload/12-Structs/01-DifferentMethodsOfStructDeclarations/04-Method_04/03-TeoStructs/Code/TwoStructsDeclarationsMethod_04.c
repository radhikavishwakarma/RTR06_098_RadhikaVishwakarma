#include<stdio.h>

//Defining struct
struct MyPoint
{
    int x;
    int y; 
};

//Defining struct
struct MyPointProperties
{
    int ganii_quadrant;
    char ganii_axis_location[10];
};

int main(void)
{
    struct MyPoint point;;// Declaring a single variable of type struct MyPoint locallly...
    struct MyPointProperties point_properties;// declaring a single variable of type struct MyPointProperties lcoalloy

    // code 
    //USer Input For the data memebrs pf struct ny point variable point_A
    printf("\n\n");
    printf("Enter X-Coordinates For A Point : ");
    scanf("%d",&point.x);
    printf("Enter Y-Coordinates For A Point :");
    scanf("%d",&point.y);

    printf("\n\n");
    printf("Point Co-ordinates (x,y) Are :(%d,%d) !!!\n\n",point.x,point.y);

    if(point. x ==0 && point.y == 0)
        printf("The Point IS The Origin (%d,%d) !!!|n\n",point.x,point.y);

    else//Atleast One of the two values (either x or Y or both) is non-zero value...
    {
        if(point.x == 0)//IF X is zero obviously Y is the non zero value
        {
            if(point.y <0) // IF Y IS  -ve
                strcpy(point_properties.ganii_axis_location,"Negative Y");
            if(point.y > 0) // If Y Is  +ve
                strcpy(point_properties.ganii_axis_location ,"Positive Y");
            
            point_properties.ganii_quadrant =0 ; // A point Lying On any of the Co-ordinates is not a part of any quadrant
            printf("The Point Lies On The %s Acis !!!|n\n,",point_properties.ganii_axis_location);
        }
        else if(point.y == 0)// If Y ise zero ..Obvisouley is the non -zero value
        {
            if(point.x < 0)// If X Is -ve
                strcpy(point_properties.ganii_axis_location, "Negative X");
            
            if (point.x  > 0) // If X is +ve
                strcpy(point_properties.ganii_axis_location,"Positive X");
            
            point_properties.ganii_quadrant = 0;// A point Lying On Ay of the coordinates axes is not a part of any quadrant..
            printf("The Point Lies On The %s Axis !!!\n\n",point_properties.ganii_axis_location);
        }
        else // Both X and Y are non-zero
        {
            point_properties.ganii_axis_location[0] = '\0'; // A point Lying In any of the 4 quadrant cannot be Lying On any of the Co-rdinate axes..

            if(point.x > 0 && point.y >0)// x is +ve and y is +ve
                point_properties.ganii_quadrant =1;
            else if(point.x < 0 && point.y > 0)
                point_properties.ganii_quadrant = 2; // x i s-ve and y us +ve
            else if(point.x < 0 && point.y < 0)// x us +ve and y is -ve
                point_properties.ganii_quadrant = 3;
            else 
                point_properties.ganii_quadrant = 4;

            printf("The Point Lies In Quadrnat Number  %d !!!|n\bn",point_properties.ganii_quadrant);
        }
    }
        return 0;
}