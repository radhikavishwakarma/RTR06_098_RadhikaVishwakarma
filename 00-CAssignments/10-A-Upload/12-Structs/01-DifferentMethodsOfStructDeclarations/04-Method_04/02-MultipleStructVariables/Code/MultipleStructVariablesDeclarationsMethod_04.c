#include<stdio.h>

//defining Struct
struct MyPoint
{
    int x;
    int y;

};

int main(void)
{
    // variable declarations
    struct MyPoint ganii_point_A,ganii_point_B,ganii_point_C,ganii_point_D,ganii_point_E;// declaring 5 struct variables of type 'struct MyPoint' locally..

    //code
    //Assigning Data Vaues To the data members of struct MyPoint; variable point_A
    ganii_point_A.x = 3;
    ganii_point_A.y = 0 ;

    //Assigning Data values to the data memebrs of struct yPoint variable point_B
    ganii_point_B.x= 1;
    ganii_point_B.y = 2 ;

    // Assigning Data values to the data memebrs os struct Mypoint variable point_C
    ganii_point_C.x = 9 ;
    ganii_point_C.y = 6;

    //Assigning Dta values to the data members of struct MyPoint variable point_D
    ganii_point_D.x = 8 ;
    ganii_point_D.y = 2 ;

    // Assigning data values to the data amembers of struct mypoint variable point_E
    ganii_point_E.x = 11;
    ganii_point_E.y = 8 ;

    //Displaying Values OF the Data memebrs os truct mypoint 
    printf("\n\n");
    printf("Co-ordinates (x,y) of Point A  Are : (%d,%d) \n\n",ganii_point_A.x,ganii_point_A.y);
    printf("Co-ordinates (x,y) of point B Are : (%d,%d) \n\n",ganii_point_B.x,ganii_point_B.y);
    printf("Co-ordinates (x,y) of point C Are : (%d,%d)\n\n",ganii_point_C.x,ganii_point_C.y);
    printf("CO-ordinates (x,y) of point D Are : (%d,%d) \n\n",ganii_point_D.x,ganii_point_D.y);
    printf("Co-ordinates (x ,y) of point E Are : (%d,%d) \n\n",ganii_point_E.x,ganii_point_E.y);

    return 0;

}