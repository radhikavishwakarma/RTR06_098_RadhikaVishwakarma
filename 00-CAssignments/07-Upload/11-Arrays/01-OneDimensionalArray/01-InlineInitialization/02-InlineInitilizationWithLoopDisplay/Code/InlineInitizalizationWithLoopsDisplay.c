#include<stdio.h>
int main(void)
{
    //variable declarations
    int iArray[] = {9,30,6,12,98,95,20,23,2,45};
    int int_size;
    int iArray_size;
    int iArray_num_elements;

    float fArray[] = {1.2,2.3f,3.4f,4.5f,5.6f,6.7f,7.8f,8.9f,};
    int float_size;
    int fArray_size;
    int fArray_num_elements;

    char cArray[] = {'A','S','T','R','O','M','E','D','I','C','o','M','P'};
    int char_size;
    int cArray_size;
    int cArray_num_elements;

    int i;

    // code
    
    // *** iArray[] ***********8
    printf("\n\n");
    printf("In-ine Initialization And Loop (for) Display Of Elements Of Array 'iArray[] : \n\n");

    int_size = sizeof(int);
    iArray_size = sizeof(iArray);
    iArray_num_elements = iArray_size / int_size;

    for(i=0; i<iArray_num_elements;i++)
    {
        printf("iArray[%d] (Element %d) = %d \n",i,(i+1),iArray[i]);
    }

    printf("\n\n");
    printf("Size of data type 'float'  = %d bytes\n",int_size);
    printf("Number of Elemente In 'int' Array 'iArray[] = %d Elements \n",iArray_num_elements);

    // ***fArray[] ****************
    printf("\n\n");
    printf("In-line Initilization And Loop (while) Display Of Elements of Array 'fArray[] : \n\n");

    float_size = sizeof(float);
    fArray_size = sizeof(fArray);
    fArray_num_elements = fArray_size / float_size;

    for(i = 0; i < fArray_num_elements;i++)
    {
        printf("fArray[%d] (Element %d) = %f \n",i,(i+1) ,fArray[i]);
    }

    printf("Size of Data type 'float' = %d Bytes \n",float_size);
    printf("Number of Elements In 'float' Array 'fArray[]' =  %d Elements \n",fArray_num_elements);

    // *** cArray ********88
    printf("\n\n");
    printf("In-line Initilization And Loop for loop Display of Elements of cArray[] : \n\n ");

    char_size = sizeof(char);
    cArray_size = sizeof(cArray);
    cArray_num_elements = cArray_size / char_size;

    for( i = 0; i < cArray_num_elements; i++ )
    {
        printf("cArray[%d] (Elements%d) = %c \n",i,(i+1),cArray[i]);
    }

    printf("Size of Data types 'char' = %d Bytes \n",char_size);
    printf("Number of Elements In 'char' Array 'cArray[]' = %d Bytes \n\n",cArray_num_elements,char_size,cArray_size);

}