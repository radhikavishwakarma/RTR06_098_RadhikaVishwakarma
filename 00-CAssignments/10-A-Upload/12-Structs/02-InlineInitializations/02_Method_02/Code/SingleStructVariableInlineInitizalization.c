#include<stdio.h>

// Defining Struct
struct MyData
{
    int ganii_i;
    float ganii_f;
    double ganii_d;
    char ganii_c;
};

struct MyData data = {9,8.2f,9.43434,'P'};

int main(void)
{
    //code
    // Displaying Values of the data memebrs of struct MyDat
    printf("\n\n");
    printf("Data memebrs of struct mydat are :\n\n");
    printf("i = %d\n",data.ganii_i);
    printf("f = %d\n",data.ganii_f);
    printf("d = %lf\n",data.ganii_d);
    printf("c = %c\n",data.ganii_c);

    return 0;

}