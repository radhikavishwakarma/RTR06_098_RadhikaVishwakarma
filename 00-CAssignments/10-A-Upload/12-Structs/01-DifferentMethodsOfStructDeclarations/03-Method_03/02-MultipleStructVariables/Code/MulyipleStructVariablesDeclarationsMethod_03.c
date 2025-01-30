#include<stdio.h>   

int main(void)
{
    //DEFININg Struct..
    struct MyPoint
    {
        int x;
        int y ;

    }point_A,point_B,point_C,point_D,point_E; // Declaring 5 struct variables of type struct mypoint locally...

    // code
    //Assigning dta values to the data memebrs of struct mypoint variabkes point_A
    point_A.x =3;
    point_A.y = 0;

    // Assigning Data values to the data memebers of struct MyPoint Variables point_B 
    point_B.x = 1;
    point_B.y = 2;

    // Assigning Data values to the data memebres of struct Mypoint variables point_c
    point_C.x = 9;
    point_C.y = 6;

    // Assigning Data values to the data memebrs of struct MyPoint variables point_D
    point_D.x = 8 ;
    point_D.y = 2;

    //Ssigning Dta values to the data memebers of struct mypoint variable point_E
    point_E.x = 11;
    point_E.y = 8;

    //Displaing vakues of the data memebrs of struct Mypoint (all variables)
    printf("\n\n");
    printf("Co-ordinates (x ,y) of Pounbt 'A' Are :(%d,%d)\n\n",point_A.x,point_A.y);
    printf("Co-ordinates (x,y) of point 'B' Are : (%d,%d)\n\n",point_B.x,point_B.y);
    printf("Co-ordinates (x,y) of Point 'C' Are : (%d,%d)\n\n",point_D.x,point_D.y);
    printf("Co-ordiantes (x,y) of Point 'E' are : (%d,%d) \n\n",point_E.x,point_E.y);

    return 0;

}