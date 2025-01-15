#include<stdio.h>
#include<stdlib.h>

//Defining stryct 
struct MyData
{
    int i;
    float f;
    double d;
};

int main(void)
{
    // variable declarations 
    int i_size;
    int f_size;
    int d_size;
    int struct_MyData_size;
    int pointer_to_struct_MyData_size;

    typedef struct MyData* MyDataptr;

    MyDataptr pData;

    //code
    printf("\n\n");

    pData = (MyDataptr)malloc(sizeof(struct MyData));
    if (pData == NULL)
    {
        printf("Failed To Allocate memeory to struct MyData !!!Existing Now...\n\n");
        exit(0);
    }
    else
        printf("Successfully allocated memory to struct MyData !!\n\n");

    //Assigning Data values to the data memebrs of struct MyData
    pData->i = 30;
    pData->f = 11.45f;
    pData->d = 1.2995;

    //Displaying value of the data memebrs of struct MyData
    printf("\n\n");
    printf("Data memebers os struct MyData Are :\n\n");
    printf("i = %d \n", pData->i);
    printf("f = %f\n", pData->f);
    printf("d = %lf\n", pData->d);

    //Calculating Sizes (In BYtes) of the data memebers of struct MyData
    i_size = sizeof(pData->i);
    f_size = sizeof(pData->f);
    d_size = sizeof(pData->d);

    //Displaying Sizes (In Bytes) of tehe memebrs os struct MyData
    printf("\n\n");
    printf("Sizes (in bytes) of data memebrs of struct MyData Are :\n\n ");
    printf("Size of i = %d bytes\n", i_size);
    printf("Size of f = %d bytes\n", f_size);
    printf("Size of d = %d bytes \n", d_size);


    //Calculating Size (In BYtes) of the entire struct MyData
    struct_MyData_size = sizeof(struct MyData);
    pointer_to_struct_MyData_size = sizeof(MyDataptr);

    //Displaying Sizes (In Bytes) of the entire struct Mydata
    printf("\n\n");
    printf("Size of struct MyData : %d bytes\n", struct_MyData_size);
    printf("Size of pointer to struct MyData : %d bytes\n\n", pointer_to_struct_MyData_size);

    if (pData)
    {
        free(pData);
        pData = NULL;
        printf("Memeoy Allocated to struyct MyData has been successfully frees !!\n\n");

    }
    return (0);

}
