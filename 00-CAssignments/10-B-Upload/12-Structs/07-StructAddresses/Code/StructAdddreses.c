#include<stdio.h>

struct MyData
{
    int ganii_i;
    float ganii_f;
    double ganii_d;
    char ganii_c;
};

int main(void)
{
    //variable delcarations
    struct MyData data;

    //code
    //Assigning Data Values To Teh Data members os truct MyDat
    data.ganii_i = 30;
    data.ganii_f = 11.45f;
    data.ganii_d = 1.2222;
    data.ganii_c ='A';

    //Displaying Values Of The Data Members os struct MyData
    printf("\n\n");
    printf("Data memebrs of  struct NyData Are :\n\n");
    printf("i = %d\n",data.ganii_i);
    printf("f = %f\n",data.ganii_f);
    printf("d = %ld\n",data.ganii_d);
    printf("c = %c \n",data.ganii_c);

    printf("\n\n");
    printf("Addresses OF data mumbers of struct MyData Are :\n\n");
    printf("i = Occupies Addresess from %p \n",&data.ganii_i);
    printf("f Occupies Addresess From %p \n",&data.ganii_f);
    printf("d  Occupies Addreses from %p\n",&data.ganii_d);
    printf("c  Occupies Address %p \n\n",&data.ganii_c);

    printf("Starting Address of struct MyData variable data = %p\n\n",&data);

    return 0;

}