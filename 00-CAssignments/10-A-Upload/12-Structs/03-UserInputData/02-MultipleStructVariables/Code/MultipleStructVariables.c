#include<stdio.h>

struct MyPoint
{
    int ganii_x;
    int ganii_y;
};

int main(void)
{
    //varible declarations
    struct MyPoint point_A,point_B,Point_C,Point_D,point_E; // Declaring a 5 struct variables

    // code
    // User nput For The data members os struct MyPoint variable point_A
    printf("\n\n");
    printf("Enter X-Coordinattes For point A :");
    scanf("%d",&point_A.ganii_x);
    printf("ENter Y-Coodinate For Point A :");
    scanf("%d",&point_A.ganii_y);

    //User Input For the data members os struct Mypoint variable point+B
    printf("\n\n");
    printf("ENter X-Co-odinated For Point B : ");
    scanf("%d",&point_B.ganii_x);
    printf("Enter Y Co-ordinates  For Point A  :");
    scanf("%d",&point_B.ganii_y);

    //User Input For the data members of struct MyPoint variable point_c
    printf("\n\n");
    printf("Enter X-Coodinated For Point C :");
    scanf("%d",&Point_C.ganii_x);
    printf("Enter Y -Ordinated For Point C :");
    scanf("%d",&Point_C.ganii_y);

    // User Inout For the data members os truct MyPoint variable point_c
    printf("\n\n");
    printf("Enter X-Coodinated For point D:");
    scanf("%d",&Point_D.ganii_x);
    printf("Enter Y-Coordinated For Point D :");
    scanf("%d",&Point_D.ganii_y);

       // User Inout For the data members os truct MyPoint variable point_c
    printf("\n\n");
    printf("Enter X-Coodinated For point E:");
    scanf("%d",&point_E.ganii_x);
    printf("Enter Y-Coordinated For Point E :");
    scanf("%d",&point_E.ganii_y);

    printf("Co-ordinates (x,y) Of Point A Are : (%d,%d) \n\n",point_A.ganii_x,point_A.ganii_y);
    printf("Co-Ordinates (x,y) Of Point B Are :  (%d,%d)\n\n",point_B.ganii_x,point_B.ganii_y);
    printf("Co-Ordinates (x ,y) Of Point C Are : (%d,%d) \n\n",Point_C.ganii_x,Point_C.ganii_y);
    printf("Co-ordinates (x,y) Of Point D Are : (%d,%d) \n\n",Point_D.ganii_x,Point_D.ganii_y);
    printf("Co-Ordinates (x,y) pf Point R Are :(%d ,%d)\n\n",point_E.ganii_x,point_E.ganii_y);

    return 0;
}