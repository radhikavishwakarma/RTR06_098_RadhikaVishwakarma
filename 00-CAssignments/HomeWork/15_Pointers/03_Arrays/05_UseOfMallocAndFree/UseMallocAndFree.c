#include<stdio.h>
#include<stdlib.h>//contains prototype of malloc and free())

int main(void)
{
    //variable declarations 
    int* ptr_iArray = NULL; //It is Good Disciplie to initailizae any pointer
    // with NULL Address to prevent any garbage value getting Into IT,That way It make It easy to check
    // for success or failure of memory allocations later on after malloc()...
    unsigned int intArrayLength = 0;
    int i;

    //code
    printf("\n\n");
    printf("Enter the number of elements you want in Your Intger Array : ");
    scanf("%d", &intArrayLength);

    /****Allocating as much memory required to the intger array
     * *****memory reuired for intger array = sizes in bytes of onew intger number of
     * intgers to b stored in array
     * ****To Allocate said amount of memory,Fucntions malloca() will be used
     *
     * ***malloc() will allocate said amount od memory and will return the inital./
     *  Starting/ abse address of the allocated memory which must be captured in apointer variable
     * ***Using This base address ,the intger array can be accesed and used

    */

    ptr_iArray = (int*)malloc(sizeof(int) * intArrayLength);
    if (ptr_iArray == NULL)//If ptr_iArray Is still NUMM,Even after call to malloc() It menas malloc() has failed to allocate
        //memory and no address has been returned malloc() intptr_iArray...
    {
        printf("\n\n");
        printf("Memory Allocations For Intger Array Failked !!!! Existing Now...\n]n");
        exit(0);
    }
    else // Is ptr_iArray is not NULL it means it must contain a valid address which is rturedn by malloc(),Hence,malloc() has succeded in memory allocations..
    {
        printf("\n\n");
        printf("Memory Allocation for intger Array Has succeded !!!|n\n");
        printf("Memory Addresses from %p to %p Have been Allocated to intger array !!!\n\n", ptr_iArray, (ptr_iArray + (intArrayLength - 1)));

    }
    printf("\n\n");
    printf("Enter %d Elements For Teh Intger Array : \n\n", intArrayLength);
    for (i = 0; i < intArrayLength; i++)
        scanf("%d", (ptr_iArray + i));
    printf("\n\n");
    printf("The Intger Array Entred By You,Consisting of %d Elements : \n\n", intArrayLength);
    for (i = 0; i < intArrayLength; i++)
    {
        printf("ptr_iArray[%d] = %d \t \t At address &ptr_iArray[%d] : %p\n", i, ptr_iArray[i], i, &ptr_iArray[i]);

    }

    printf("\n\n");
    for (i = 0; i < intArrayLength; i++)
    {
        printf("*(ptr_iArray + %d ) = %d \t \t At Address (ptr_iArray + %d) : %p \n", i, *(ptr_iArray + i), i, (ptr_iArray + i));
    }
    /****Checksing if memeory is still allocated by checking validity of base address 'ptr_iArray' ***
     *If Address is valid that is if 'ptr_iArray'Exists That is ,if
     * it is not null,memeory is still alolcated***
     * ***In That case,the allocated using amlloc() and freed using free()***
     * ***Once memeory is freed Using free(),the base address must be
     * cleaned,that is it must be  re-initialized to 'NULL' to keep away
     * cleaned ,that is it must be re-initialized t to null to keep away
     * Garbage values.This is not compulsory ,But it is good coding discipline**
    */
    if (ptr_iArray)
    {
        free(ptr_iArray);
        ptr_iArray = NULL;

        printf("\n\n");
        printf("Memory Allocated For Intger Array Has been successfully Freee !!!|n\n");

        return (0);
    }
}
