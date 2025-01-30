#include<stdio.h>

// Defining Struct
struct MyData
{
    int ganii_i;
    float ganii_f;
    double ganii_d;
    char ganii_c;

} data = {10,4.5f,11.4555,'A'};// Inlica Initializations of struct vaiables dat of type struct MyData

int main(void)
{
    //code
    // Displaying value aof the data memerbs of struct MyDat
    printf("\n\n");
    printf("Data Members os struct MyDat Are :\n\n");
    printf("i = %d\n",data.ganii_i);
    printf("f = %f\n",data.ganii_f);
    printf("d= %ld\n",data.ganii_d);
    printf("c= %c\n",data.ganii_c);

    return 0;
}