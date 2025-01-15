#include<stdio.h>

struct MyData
{
    int i;
    float f;
    double d;
    char c;
};

int main(void)
{
    //variable delcarations
    struct MyData data;

    //code
    //Assigning Data Values To Teh Data members os truct MyDat
    data.i = 30;
    data.f = 11.45f;
    data.d = 1.2222;
    data.c = 'A';

    //Displaying Values Of The Data Members os struct MyData
    printf("\n\n");
    printf("Data memebrs of  struct NyData Are :\n\n");
    printf("i = %d\n", data.i);
    printf("f = %f\n", data.f);
    printf("d = %lf\n", data.d);
    printf("c = %c \n", data.c);

    printf("\n\n");
    printf("Addresses OF data mumbers of struct MyData Are :\n\n");
    printf("i = Occupies Addresess from %p \n", &data.i);
    printf("f Occupies Addresess From %p \n", &data.f);
    printf("d  Occupies Addreses from %p\n", &data.d);
    printf("c  Occupies Address %p \n\n", &data.c);

    printf("Starting Address of struct MyData variable data = %p\n\n", &data);

    return (0);

}
