#include<stdio.h>

int main(void)
{
    //Defining struct
    struct MyPoint
    {
        int x ;
        int y;
    }point; // Declarin a single variable of type ;struct Mypoint locally...

    //Defining Struct
    struct MyPointProperties
    {
        int quadrant;
        char axis_locations[10];
    }point_properties; //declaring a single variable of type struct MypointProperties locally...

    // code User Input For The Data Members of 'struct Mypoint variable point_A
    printf("\n\n");
    printf("Enter X-Coordinates For A Point :");
    scanf("%d",&point.x);
    printf("Enter Y-Coordinates For A point : ");
    scanf("%d",&point.y);

    printf("\n\n");
    printf("Piint Co-Ordinates (x,y) Are :(%d,%d) !!!\n\n",point.x,point.y);

    if(point.x ==0 && point.y == 0)
        printf("The Point Is The Origni (%d,%d) !!!|n",point.x,point.y);
    else // Atleast One of the two values (either x or y or Bithe) is a non-zerovalue
    {
        if(point.x == 0)// If x is  zero ..obvispuly Y  is the non zero value
        {
            if (point.y <0) // If Y Is -v2
                strcpy(point_properties.axis_locations,"Negative Y");
            if(point.y > 0) // If Y Is +ve
                strcpy(point_properties.axis_locations,"Positive Y");

            point_properties.quadrant = 0; /// A point lying on Any oifth Co-odinate is note a part of any quadrant...
            printf("The point Lies on the Axis %s !!!\n",point_properties.axis_locations);
        }
        else if(point.y ==0) // If Y is zeof ...obvisouly x is the non-zerovalue
        {
            if(point.x < 0)// If X is -ve
                strcpy(point_properties.axis_locations,"Negatibve X");
            if(point.x > 0)// If X is +ve
                strcpy(point_properties.axis_locations,"Positive X");

            point_properties.quadrant = 0; // A  point Lying On Any of the Co-ordinate Is not a part of any quadrnat...
            printf("The Point Lies on the s Axis !!!|n\n",point_properties.axis_locations);
        }
        else // Both X and Y Are non-zero
        {
            point_properties.axis_locations[0] = '\0';// A point Lying In Any of the 4 quadrnat cannot Be lying Onl Any of The Co-Ordinates axes...

            if(point.x > 0 && point.y >0)// X is +vr and Y is +ve
                point_properties.quadrant =1;
            
            else if(point.x < 0&& point.y >0)// x is -ve and Y is +ve
                point_properties.quadrant = 2 ;
            else if(point.x < 0 && point.y <0)// X is -ve and Y is -ve
                point_properties.quadrant = 3 ;
            else
                point_properties.quadrant = 4 ;
            
            printf("The Point Lies In Quadrnat Number %d !!\n\n",point_properties.quadrant);

        }
    }
    return 0;

}