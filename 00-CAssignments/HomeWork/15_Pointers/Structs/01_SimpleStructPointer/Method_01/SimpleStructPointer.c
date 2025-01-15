#include<stdio.h>
#include<stdlib.h>

//Defining struct
struct MyData
{
    int i;
    float f;
    double d;
};

int main(void)
{
    //variable declarations
    int i_size;
    float f_size;
    double d_size;
    int struct_MyData_size;
    int pointer_to_struct_MyData_size;

    struct MyData* pData = NULL;

    //code
    printf("\n\n");

    pData = (struct MyData*)malloc(sizeof(struct MyData));
    if (pData == NULL)
    {
        printf("Failed to allocated memeory to struct MYData !!! Existing now...\n\n");
        exit(0);
    }
    else
        printf("Successfully Allocated memeory to struct MyData !!\n\n");

    // Assigning data values to the data members to struct MyData !!!|n\n
    (*pData).i = 30;
    (*pData).f = 11.45f;
    (*pData).d = 1.2995;

    printf("******************");
    //Displaying Values of the data members of struct MyData
    printf("\n\n");
    printf("Data memebers of struct MyDat Are :\n\n");
    printf("i = %d \n", (*pData).i);
    printf("f =%f \n", (*pData).f);
    printf("d = %lf \n", (*pData).d);


    // Calculating Sizes (In BYte) of the data memebrs os struct MyData
    i_size = sizeof((*pData).i);
    f_size = sizeof((*pData).f);
    d_size = sizeof((*pData).d);

    //Displaying Sizes (In Bytes) of Data memerbs of struct MyData
    printf("\n\n");
    printf("Sizes (in bytes) of data memebrs of struct MyDat Are :\n\n");
    printf("Size of i = %d bytes\n", i_size);
    printf("Size of f = %lf bytes\n", f_size);
    printf("Size of d = %lf bytes\n", d_size);

    //Calculating size (In BYtes) of the entire struct MyData
    struct_MyData_size = sizeof(struct MyData);
    pointer_to_struct_MyData_size = sizeof(struct MyData*);

    //Displaying Sizes(in bytes) of the entire struct MyDat
    printf("\n\n");
    printf("Size of struct MyData : %d bytes\n\n", struct_MyData_size);
    printf("Size of pointer to struct MyDat : %d bytes\n\n", pointer_to_struct_MyData_size);

    if (pData)
    {
        free(pData);
        pData = NULL;
        printf("Memory Alocated to struct MyDat Has been successfully freee !!!|n\n");
    }
    return (0);
}
