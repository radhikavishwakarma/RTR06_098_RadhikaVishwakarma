#include<stdio.h>
#include<stdlib.h>


//Defining Struct
struct MyData
{
    int i ;
    float f;
    double d;
};

int main(void)
{
    //variablle declarations
    int i_size;
    int f_size;
    int d_size ;
    int struct_MyData_size;
    int pointer_to_struct_MyData_size;

    struct MyData *pData= NULL;

    //code
    printf("\n\n");

    pData = (struct MyData *)malloc(sizeof(struct MyData));
    if(pData = NULL)
    {
        printf("Failed to allocate memeory to struct MYData !! Existing Now...\n\n");
        exit(0);
    }
    else 
        printf("Successfully allocated memeory to struct MYdata !!\n\n");

    pData->i = 30;
    pData->f = 11.45f;
    pData->d = 1.2999;

    //Displaying VAlues of the data memerbs of struct MyData
    printf("\n\n");
    printf("Data emebrs os struct MyDat Are :\n\n");
    printf("i = %d \n",pData->i);
    printf("f = %f\n",pData->f);
    printf("d = %lf \n",pData->d);

    //calculating Sizes In bytes of the data memebrs of struct MyData
    i_size = sizeof(pData->i);
    f_size = sizeof(pData->f);
    d_size = sizeof(pData->d);

    //Displaying Sizes (In Bytes) of the Data members of struct MyDat
    printf("\n\n");
    printf("Sizes (in bytes) of Data memerbs os truct MyDat Are :\n\n");
    printf("Size of i = %d bytes \n",i_size);
    printf("Size of f  = %d bytes\n",f_size);
    printf("Size of d = %d bytes \n",d_size);

    //calculating size (In bytes) of th entire struct MyData
    struct_MyData_size = sizeof(struct MyData);
    pointer_to_struct_MyData_size = sizeof(struct MyData *);

    //Displaing Sizes (In BYtes) of th entire sruct MyData
    printf("\n\n");
    printf("Size of struct MyData : %d bytes\n\n",struct_MyData_size);
    printf("Size of pointer to 'struct MyData' : %d bytes \n\n",pointer_to_struct_MyData_size);

    if(pData)
    {
        free(pData);
        pData = NULL;
        printf("Memery Allocated to struct MyDat Has been Successfully freed !!\n\n");
    }
    return 0;
}