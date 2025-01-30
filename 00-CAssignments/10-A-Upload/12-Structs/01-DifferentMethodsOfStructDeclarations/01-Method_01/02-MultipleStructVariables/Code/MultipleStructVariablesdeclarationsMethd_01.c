#include<stdio.h>

//Defining struct...
struct MyPoint
{
    int x ;
    int y;
} point_A,point_B,point_C,point_D,point_E; // declarig 6 struct variable of type struct Mypoint globally

int main(void)
{
    //code
    // Assigning Data Values To The Data Members of 'struct MyPoint' variable point_A
    point_A.x = 3;
    point_A.y = 0 ;

    // Assigning Data values to the data mebers of struct MyPoint variable point_B
    point_B.x = 1;
    point_B.x =2 ;

    // Assigning data values to the data mebers of struct Mypoint variable point_c 
    point_C.x =9 ;
    point_C.y = 6;

    //Assigning data values to the data mbers of struct Mypoint variable point_d 
    point_D.x = 8;
    point_D.y = 2; 

    // Assigning data values to the data members of struct Mypoint variable point_E
    point_E.x = 11;
    point_E.y = 8;

    //Displaying Values of The Data memebrs of struct mystruct
    printf("\n\n");
    printf("Co-Ordinates (x,y) Of Pint 'A' Are : (%d ,%d)\n\n",point_A.x,point_A.y);
    printf("Co-Ordinates (x ,y) Of Point 'B' Are : (%d ,%d)\n\n",point_B.x,point_B.y);

    printf("Co-Ordinates (x,y) of Point 'C' Are : (%d,%d)\n\n",point_C.x,point_C.y);
    printf("Co-Ordinates (x,y) of Point 'D' Are :(%d,%d )\n\n",point_D.x,point_D.y);
    printf("Co-ordinates (x,y) Of Point 'E' Are :( %d ,%d)\n\n",point_E.x,point_E.y);

    return 0;
}   