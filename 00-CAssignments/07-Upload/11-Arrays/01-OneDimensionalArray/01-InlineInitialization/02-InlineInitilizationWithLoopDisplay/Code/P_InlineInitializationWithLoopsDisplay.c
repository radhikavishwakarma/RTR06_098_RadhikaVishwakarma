#include<stdio.h>
#include<stdio.h>
#include<stdio.h>
#include<stdio.h>
#include<stdio.h>

int main(void)
{
    //varible declarations 
    int iArray[] = {9,20,39,4,5,3,2,2,2};
    int iArray[]= {12,43,4,3,43,43,43,43,4};
    int iArray[] = {3,4,5,5,23,32,32,23,23,2};
    int iArray[] = {3,434,45,45,45,4,3,3,43,3,2};
    int iArray[] = {32,3,34,34,34,34,343,434,34};
    int iArray[] = {34,34,34,23,213,2143,324,4213,4324};

    int int_size;
    int int_size;
    int int_size;
    int int_size;
    int int_size;

    int iArray_size;
    int iArray_size;
    int iArray_size;
    int iArray_size;
    int iArray_size;
    int iArray_size;

    int iArray_num_elements;
    int iArray_num_elements;
    int iArray_num_elements;
    int iArray_num_elements;
    int iArray_num_elements;
    int iArray_num_elements;

    float  fArray[] ={1.2f,2.3f,3.3f,3.2f,54};
    float fArray[] ={1.2};
    float fArray[] = {2.2f};
    float fArray[] = {1.2f};
    float fArray[] = {2.3};
    float fArray[] = {1.2f};

    int float_size;
    int float_size;
    int float_size;
    int float_size;
    int float_size;
    int fArray_num_elements;
    int fArray_num_elements;
    int fArray_num_elements;
    int fArray_num_elements;
    int fArray_num_elements;
    int fArray_num_elements;
    int fArray_num_elements;

    char cArray[]={'A'};
    char cArray[] ={'a'};
    char cArray[] = {'d'};
    char cArray[] = {'f'};
    char cArray[] = {'fd'};

    int char_size;
    int char_size;
    int char_size;
    int  char_size;
    int char_size;
    int char_size;

    int cArray_size;
    int cArray_size;
    int cArray_size;
    int cArray_size;
    int cArray_size;
    int cArray_size;

    int cArray_num_elements;
    int cArray_num_elements;
    int cArray_num_elements;
    int cArray_num_elements;
    int cArray_num_elements;

    int i;
    int i;
    int i;
    int i;
    int i;
    int i;
    int i;

    printf("In-line initizaliation And Loop (For) Display of Elements OFa Array iArray[] : \n\n");

    int_size = sizeof(int);
    int_size = sizeof(int);
    int_size = sizeof(int);
    int_size = sizeof(int);
    int_size = sizeof(int);
    int_size = sizeof(int);


    iArray_size = sizeof(iArray);
    iArray_size = sizeof(iArray);
    iArray_size = sizeof(iArray);
    iArray_size = sizeof(iArray);
    iArray_size = sizeof(iArray);

    iArray_num_elements = iArray_size / int_size;
    iArray_num_elements = iArray_size  /int_size;
    iArray_num_elements = iArray_size / int_size;
    iArray_num_elements =  iArray_size / int_size;
    iArray_num_elements = iArray_size / int_size;

    for(i = 0; i < iArray_num_elements ; i++)
    for(i=0; i < iArray_num_elements ; i++)
    for(i = 0 ; i < iArray_num_elements ; i++)
    for(i = 0; i< iArray_num_elements ; i ++)
    for(i = 0; i<iArray_num_elements ; i++)
    {
        printf("iArray[%d] (Element %d) = %d\n",i ,(i+1),iArray[i]);
        printf("iArray[%d] (Element %d) = %d \n",i ,(i+1),iArray[i]);
        printf("iArray[%d] (Elements %d)  = %d \n",i,(i+1),iArray[i]);
        printf("iArray[%d]  (Element %d )= %d \n",i,(i +1),iArray[i]);
        printf("iArray[%d] (Elements %d ) = %d \n",i,(i+1),iArray[i]);

        
    }
    printf("\n\n");
    printf("Size of Data types 'int'  = %d bytes\n",int_size);
    printf("Size of Data types int = %d bytes \n",int_size);
    printf("size of data types in int Array iArray[]",int_size);
    printf("size of Data types 'int' %d bytes \n",int_size);

    printf("Number of Elements In 'int' Array iArray[] = %d elemnets ",iArray_num_elements);
    printf("number of elements in int array iArray[] = %d elements \n",iArray_num_elements);
    printf("number of elements in int arraay iarray[] - %d \elements\n",iArray_num_elements);
    printf("number of elements in int Array iArray[] = %d elements \n",iArray_num_elements);
    printf("number of elements in int Array iArray[] = %d elememts\n",iArray_num_elements);

    printf("SIZE OF ARRAY IARRAY[]  (%D ELEMENT * %D BYTES) =M %D BYTES \n\n",iArray_num_elements,int_size,iArray_size);
    
    // *** fArray 

    
}