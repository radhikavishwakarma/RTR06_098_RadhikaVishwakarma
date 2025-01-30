#include<stdio.h>

//defining struct...
struct MyPoint
{
    int x ;
    int y ;

} point_A,point_B,point_C,point_D,point_E; // declaring 5 struct variables of type struct Mypoint globally...

int main(void)
{
    //code
    // assigning data values to the data members of striuct MyPoint variable point_a
    point_A.x = 3;
    point_A.y = 0;
    //
    //Assigning Data values to the data mebrs of struct MyPoint varible point_B
    point_B.x = 1;
    point_B.y = 2;

    //Assigning Data values to the data members of struct Mypoint variabe Point_c
    point_C.x = 9;
    point_C.y = 6;

    //Assigning Data values to the data members of struct MyPoint varuable Point_D
    point_D.x = 8;
    point_D.y =  2;

    // Assigning Data values tothe data members of struct Mypoint variable Point_E
    point_E.x = 11;
    point_E.y = 8;

    //Displayong Data Values to the data members of struct MyPoint (all variable)
    printf("\n\n");
    printf("Co-Ordinates (x,y) Of Point 'A' Are : (%d,%d)\n\n",point_A.x,point_A.y);
    printf("Co-Ordinates (x,y) O point 'B' Are :(%d,%d)\n\n",point_B.x,point_B.y);
    printf("Co-ordinates (x,y) Of Point 'C' Are :(%d,%d )\n\n",point_D.x,point_D.y);
    printf("Co-Ordinates (x,y) Of point 'E' Are : (%d,%d)\n\n",point_E.x,point_E.y);

    return 0;


}