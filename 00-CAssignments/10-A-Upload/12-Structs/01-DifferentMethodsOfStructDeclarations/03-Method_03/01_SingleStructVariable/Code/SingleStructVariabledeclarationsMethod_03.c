#include<stdio.h>

int main(void)
{
    //Defining Struct
    struct MyData
    {
        int i;
        float f;
        double d;
    }data ; // Declaraing a single struct variable of type 'struct MyData locally...

    //variablde declarations
    int i_size;
    int f_size;
    int d_size;
    int struct_MyData_size;

    //code
    //Assigning Data Values To The Data Members Of 'struct Mydata
    data.i = 30;
    data.f =  11.45f;
    data.d = 1.2222;

    //Displaying Values Of The Data Mebers of  'struct MyData'
    printf("\n\n");
    printf("Data Memebers of 'struct MyData' Are: \n\n");
    printf("i =%d \n",data.i);
    printf("f = %f \n",data.f);
    printf("d = %ld \n",data.d);

    //Calculating Size (In Bytes) Of The Data Members of strict MyData
    i_size= sizeof(data.i);
    f_size = sizeof(data.f);
    d_size = sizeof(data.d);

    //Displaying Size (In Bytes) Of The data members of struct MyData
    printf("\n\n");
    printf("SIZES (in bytes) OF Data Members of ;struct MyData' Are :\n\n");
    printf("size of i = %d bytes\n",i_size);
    printf("size of f = %d bytes\n",f_size);
    printf("size of d = %d bytes\n",d_size);

    // Calculating size (In Bytes) Of th entire struct Mydata
    struct_MyData_size = sizeof(struct MyData);// can also give stryucr nae -> sizeof(MyData)

    // Displaying sizes (In Bytes) of the entire ;struct MyData
    printf("\n\n");
    printf("Size of struct Mydata : %d bytes\n\n",struct_MyData_size);

    return 0;

}