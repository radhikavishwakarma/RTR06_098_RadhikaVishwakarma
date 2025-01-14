#include<stdio.h>
#include<stdlib.h>

//Defining Struct
struct MyData
{
    int i;
    float f;
    double d;
};

int main(void)
{
    //functsion prototypes
    void ChangeValues(struct MyData*);

    //variablde declaaratiosn
    struct MyData* pData = NULL;

    //code
    printf("\n\n");

    pData = (struct MyData*)malloc(sizeof(struct MyData));
    if (pData == NULL)
    {
        printf("Failed to allocate memeory to struct MyData !!! Existing no...\n\n");
        exit(0);

    }
    else
        printf("Successfully Allocated memeory to struct MyData  !!!|n\n");

    //Assigning  Data VAlues to the data memebrs of struct MyDat
    pData->i = 30;
    pData->f = 11.45f;
    pData->d = 1.2995;

    //Displaying Value sof the Data ememrbs of struct MyDat
    printf("\n\n");
    printf("Data Members of struct MyData Are : \n\n");
    printf(" i = %d \n", pData->i);
    printf("f = %f \n", pData->f);
    printf("d = %lf \n", pData->d);

    ChangeValues(pData);

    //Displaing Values of the ata Members of strict MyDtaa
    printf("\n\n");
    printf("Data Members of struct MyData Are :\n\n");
    printf("i = %d\n", pData->i);
    printf("f = %f\n", pData->f);
    printf("d = %lf \n", pData->d);

    if (pData)
    {
        free(pData);
        pData = NULL;
        printf("Memory Allocated To strucy MyData Has Been Successfully FEED !!! \n\n");
    }

    return (0);

}

void ChangeValues(struct MyData* pParam_Data)
{
    //code

    pParam_Data->i = 9;
    pParam_Data->f = 8.2f;
    pParam_Data->d = 6.1998;

    //Can also do this as..

    /*
        (*pParam_Data).i = 9;
        (*pParam_Data).f = 8.2f;
        (*pParam_Data).d = 6.1998;
    */
}