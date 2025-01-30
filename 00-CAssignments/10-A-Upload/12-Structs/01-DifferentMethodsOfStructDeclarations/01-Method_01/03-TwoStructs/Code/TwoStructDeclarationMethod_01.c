#include<stdio.h>

//Defining struct
struct MyPoint
{
    int x;
    int y ;

}point; //declaring a single variable of type struct Mypoint globally...

// Defining struct
struct MyPointPropertines
{
    int quadrant;
    char axis_location[10];
} point_properties; // declarring a single variables of type struct MyPointProperties globally..

int main(void)
{
    //code
    //User Input For The Data Members 'struct Mypoint' variable point_a
    printf("\n\n");
    printf("Enter X-Coordinates For A point : ");
    scanf("%d",&point.x);
    printf("Enter Y-Coordinates For A Point : ");
    scanf("%d",&point.y);

    printf("\n\n");
    printf("Point Co-ordinates (x,y) Are : (%d,%d) !!!\n\n",point.x,point.y);

    if(point.x == 0 && point.y == 0)
        printf("The Point Is The Origin (%d,%d) !!!\n",point.x, point.y);
    else  // Atleast One OF the two values (either 'x' or ;y; or both) is non zero values/...
    {
        if(point.x ==0) // IS 'X' IS ZERO.. OBVIOUSLY 'Y' IS THE NON-ZERO VALUE
        {
            if(point.y < 0) // Is 'Y; IS -ve
                strcpy(point_properties.axis_location, "Positive Y");
            
            if(point.y > 0)// If 'Y; Is  +ve
                strcpy(point_properties.axis_location, "Positive Y");
            
            point_properties.quadrant =0; // A Point Lying On Any of The Co-Ordinate Axes Is not A pArt Of Any quadrant...
            printf("The Point Lines On Th %s Axis !!!\n\n",point_properties.axis_location);

        }
        else if(point.y ==0) // If 'Y' Is zero ..Obviously 'x' Is The Non-zero value
        {
            if(point.x < 0)// Is 'X' Is -ve
                strcpy(point_properties.axis_location, "Negative X");
            
            if(point.x > 0) // If ' X' Is +ve
                strcpy(point_properties.axis_location, "Positive X");

            point_properties.quadrant = 0;// A Point Lying On Any Of The Co-Ordinate Axes Is Not A part of Any quadrant...
            printf("The Point Lies On The %s Axis !!!\n\n",point_properties.axis_location);

        }
        else // Both 'X' AND 'Y' Are Non zero 
        {
            point_properties.axis_location[0] = '\0';// A Point Lying In Any Of The 4 Quadrant Cannot Be lying On Any The Co-ordinates Axes...

            if(point.x > 0 && point.y > 0) // 'X' IS +ve AND 'Y' Is +ve
                point_properties.quadrant =0;
            else if(point.x < 0 && point.y > 0)// 'X' IS -ve AND 'Y' IS +ve
                point_properties.quadrant =1;
            else if(point.x < 0 && point.y <0)  // 'X'  IS -ve ANd 'Y' Is -ve
                point_properties.quadrant = 2;
            else    
                point_properties.quadrant = 4; // 'X' Is +ve AND Is -ve
            
            printf("The Point Lies In Quadrant Number %d !!!\\n",point_properties.quadrant);
        }
    }
    return 0;

}