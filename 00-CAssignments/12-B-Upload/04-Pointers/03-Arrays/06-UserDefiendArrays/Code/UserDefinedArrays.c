#include<stdio.h>
#include<stdlib.h>

#define INT_SIZE sizeof(int)
#define FLOAT_SIZE sizeof(float)
#define DOUBLE_SIZE  sizeof(double)
#define CHAR_SIZE sizeof(char)

int main(void)
{
    // variable declarations
    int *ptr_iArray = NULL;
    unsigned int intArrayLength = 0;

    float *ptr_fArray = NULL;
    unsigned int floatArrayLength = 0;

    double *ptr_dArray = NULL;
    unsigned int doubleArratLenght  = 0;

    char *ptr_cArray = NULL;
    unsigned int charArrayLength = 0;

    int i ;

    //code

    // ****INTGER ARRAY ****
    printf("\n\n");
    printf("Enter The Number of Elements You want In the Intger Array : ");
    scanf("%u",&intArrayLength);

    ptr_iArray = (int*)malloc(INT_SIZE *intArrayLength);
    if(ptr_iArray == NULL)
    {
        printf("\n\n");
        printf("Memory Allocation For Intger Array Failed !!!Existing now...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("Memory Allocation For Intger Array Succeeded !!!\n\n");
    }
    printf("\n\n");
    printf("Enter The %d  Intger Elememts To fill Up the Intger Array : \n\n",intArrayLength);
    for(i = 0; i < intArrayLength; i++)
        scanf("%d",(ptr_iArray + i));
    
    // ****Float Array ****
    printf("\n\n");
    printf("Enter The Number of ELements Your want in the float array : ");
    scanf("%u",&floatArrayLength);


    ptr_fArray = (float*)malloc(FLOAT_SIZE *floatArrayLength);
    if(ptr_fArray == NULL)
    {
        printf("\n\n");
        printf("Memory allocation For floating-poinmyer Array failed !!! Existing now...\n\n");
        exit(0);
    }
    else 
    {
        printf("\n\n");
        printf("Memory Allocation for floating-point array succeded !!!|n\n");
    }

    printf("\n\n");
    printf("ENter The %d Floating-Point Elememnts To Fill Up The float Array :\n\n",floatArrayLength);
    for(i = 0 ; i < floatArrayLength; i++)
        scanf("%f",(ptr_fArray + i));
    
    /// ****Double Array 
    printf("\n\n");
    printf("Enter The Num,ber of Elements Ypu want in the double array : ");
    scanf("%u",&doubleArratLenght);

    ptr_dArray = (double*)malloc(DOUBLE_SIZE * doubleArratLenght);
    if(ptr_dArray == NULL)
    {
        printf("\n\n");
        printf("Memory Allocation For Double Array Failed !!!! Existing now ..\n\n");
        exit(0);
    }
    else 
    {
        printf("\n\n");
        printf("Memory Allcoation For double Array Succeeded !!!|n\n");
    }
    
    printf("\n\n");
    printf("Enter The %d Double Elememts To Fill Up The double Array : \n\n",doubleArratLenght);
    for(i =0; i < doubleArratLenght;  i++)  
        scanf("%lf",(ptr_dArray + i));
    
    // ***Char Array ******
    printf("\n\n");
    printf("Enter the NUmber of elements you want in the character Array : ");
    scanf("%u",&charArrayLength);

    ptr_cArray = (char*)malloc(CHAR_SIZE *charArrayLength);
    if(ptr_cArray == NULL)
    {
        printf("\n\n");
        printf("Memeory Allocation For character Array Failed !!! Existing now...\n\n");
        exit(0);
    }
    else 
    {
        printf("\n\n");
        printf("Memory Allocation for character array succeeded !!!\n\n");
    }

    printf("\n\n");
    printf("ENter the %d Character Elements To Fill Up The Characxter array : \n\n",charArrayLength);
    for(i =0; i < charArrayLength; i++)
    {
        *(ptr_cArray + i) = getch();
        printf("%c\n",*(ptr_cArray + i));
    }

    // *** Displaying FO Arrayss***

    // ***Interger Array ***
    printf("\n\n");
    printf("The Intger Array Entered By uyou and Consisting of %d Elements Is As Follows :\n\n",intArrayLength);
    for(i =0; i < intArrayLength; i++)
        printf("%d \t \t  At Address : %p \n",*(ptr_iArray + i),(ptr_iArray+ i));
    
    // ****Float Array ****
    printf("\n\n");
    printf("The Float Array Entered By you and Consiting of %d Elements Is a follows :\n\n",floatArrayLength);
    for(i =0; i < floatArrayLength; i++)
        printf("%f \t \t At address %p \n",*(ptr_fArray + i),(ptr_fArray +i));
    
    // ***Double **
    printf("\n\n");
    printf("The Double Array Entred By Your and Consisting of %d Elements is As follows : \n\n",doubleArratLenght);
    for(i =0; i < doubleArratLenght;i++)
        printf("%lf \t \t  At address : %p\n",*(ptr_dArray+ i),(ptr_dArray + i));
    
    // *****Character Array ******
    printf("\n\n");
    printf("The Character Array Entered By You and consiting fo %d Elements Is As Follows : \n\n",charArrayLength);
    for(i =0; i < charArrayLength ; i++)
        printf("%c  \t \t  At Address : %p \n",*(ptr_cArray + i),(ptr_cArray +i));

    //// *****Freeing memeory Occupied by arrays (In revers order of allocation)
    if(ptr_cArray)
    {
        free(ptr_cArray);
        ptr_cArray = NULL;

        printf("\n\n");
        printf("Memory Occupied by Character Array has been successfully freeed !!!|n\n");
    }

    if(ptr_dArray)
    {
        free(ptr_dArray);
        ptr_dArray = NULL;

        printf("Memory Occupied by double has beedb successfully freeed !!!\n\n");
    }

    if(ptr_fArray)
    {
        free(ptr_fArray);
        ptr_fArray = NULL;

        printf("\n\n");
        printf("Memeory Occupired by floating-Point array ahs beedn Successfu;lly freee !!!\n\n");

    }

    if(ptr_iArray)
    {
        free(ptr_iArray);
        ptr_iArray = NULL;

        printf("\n\n");
        printf("Memory Occupied By Intger array hass been successfuly freee !!\n\n");
    }

    return 0;
}
