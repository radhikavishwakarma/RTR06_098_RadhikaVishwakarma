#include<stdio.h>

//Defining struct
struct MyData
{
    int i;
    float f ;
    double d;

};

int main(void)
{
    //variable declarations
    struct MyData data; // declaring a single struct variable of type struct MyData locally...

    int i_size;
    int f_size ;
    int d_size;
    int struct_MyData_size;

    // code
    //Assigning Dat vakues to the data memebrs of struct Mydat 
    data.i = 30;
    data.f = 11.45f;
    data.d = 1.2999;

    //Display Valuesof the dtaa members of struct MyDat
    printf("\n\n");
    printf("Data Members of struct MyDat Are :\n\n");
    printf("i = %d\n",data.i);
    printf("f = %f\n",data.f);
    printf("d = %lf\n",data.d);

    // Calculating size (InBhtes ) of the data members of struct MyDat
    i_size= sizeof(data.i);
    f_size = sizeof(data.f);
    d_size = sizeof(data.d);

    //Dispalying sizes (In Bytes) of the data members os struct MyDat
    printf("\n\n");
    printf("SIZES (in bytes) Of Data Members os struct MyData Are :\n\n");
    printf("size of i = %d bytes\n",i_size);
    printf("size of f = %d bytes\n",f_size);
    printf("size of d = %d bytes\n",d_size);

    // calculating szie (In Bytes) of entire struct MyData
    struct_MyData_size = sizeof(struct MyData); // can also give struct name->sizeof(Mydata)

    // Displaying sizes (In Bytes) of the entire struct mydata
    printf("\n\n");
    printf("Size of struct MyDat : %d bytes\n\n",struct_MyData_size);

    return 0;

}   