#include<stdio.h>

//Defining struct
struct MyPoint
{
    int x;
    int y;
}point; // declaring a single variable of type 'struct MyPoint' globally...

//defining struct
struct MyPointProperties
{
    int quadrant;
    char axis_locations[10];
}point_properties; // declarin a single variable of type ;struct MypointProperties globally...

int main(void)
{
    //code
    // User Unput For The Data Members of struct Mypoint variable point_a
    printf("\n\n");
    printf("Enter X-Coodrinates For A point :");
    scanf("%d",&point.x);
    printf("ENter Y-Coordinates For A Point :");
    scanf("%d",&point.y);
    printf("\n\n");
    printf("Point Co-ordinates (x,y) Are :(%d,%d) !!\n\n",point.x,point.y);

    if(point.x ==0 && point.y ==0)
        printf("Point Co-ordinates (X,y)Are : (%d,%d) !!\n\n",point.x,point.y);

    else//Atleast One Of the two Values (either 'x' or 'y' or Boithe) is non-zero value..
    {
        if(point.x ==0) //Is 'X' Is Zero ..Obviusly 'Y; Is the Non-zero value
        {
            if(point.y < 0) // If 'Y; Is -v3
                strcpy(point_properties.axis_locations,"Negative Y");
            if(point.y > 0) // Is 'Y' Is +ve
                strcpy(point_properties.axis_locations,"Positive Y");
            
            point_properties.quadrant =0;// A point Lying On ANy Of The Co-Ordinate axes Is not A part of Any Quadrant..
            printf("The Point Lies On Th %s Axis !!!\n\n",point_properties.axis_locations);

        }

        else if(point.y ==0)//If 'Y' Is Zero ..obvisously 'X' Is the Non zero value
        {
            if (point.x < 0) // Is 'x Is -ve
                strcpy(point_properties.axis_locations,"Negative X");
            if(point.x >0)  // Is 'X' is +ve
                strcpy(point_properties.axis_locations,"Positive X");
            
            point_properties.quadrant = 0;// A point Lying On Any of the Co-ordinates axes is not a part of any quadrant..
            printf("The Point Lies On The %s Axis !!!\n\n",point_properties.axis_locations);

        }
        else    //Botj X and Y Are Non-zer
        {
            point_properties.axis_locations[0] = '\0';// A point Lying In Any of the 4 qudrant Canoot be Lying On Any ofthe Co-Ordinates axes...

            if(point.x > 0 && point.y >0) // 'X' Is +vr And 'Y' Is +ve
                point_properties.quadrant = 1;
            
            else if(point.x < 0 && point.y > 0) // 'X' IS _v3 And 'Y' Is  +ve
                point_properties.quadrant = 2;
            else if(point.x < 0 && point.y < 0)// 'X' Is -ve and 'Y; Is -ve
                point_properties.quadrant = 3;
            else // X Is +ve And 'Y' Is -ve
                point_properties.quadrant = 4;
            
            printf("The Point Lies In Quadrnt Number %d !!!\n\n",point_properties.quadrant);

        }
    }
    return 0;
}