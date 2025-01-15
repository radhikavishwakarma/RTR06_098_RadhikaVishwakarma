#include<stdio.h>

int main(void)
{
    //variable declarations
    int iArray[] = { 10,20,30,40,50,60.70,80,90,100 };//IntgerArray 
    int* ptr_Array = NULL; //Integer Pointer
    //code

    //######USING Array Name As Array i.e. Value of xthe lemenet of iArrat
    //iArray[x] And Address of xthe element of iArray : &[iArrayx] 

    printf("\n\n");
    printf("***Using Array NAe As Array i.i. Value of x the Element of iArrat : iArray[x] Array Elements And Theirr Addreses : \n\n");
    printf("iArray[0] = %d \t \t  At Address &iArray[0] : %p\n", iArray[0], &iArray[0]);
    printf("iArray[1] = %d \t \t At Address &iArray[1] : %p\n ", iArray[1], &iArray[1]);
    printf("iArray[2] = %d \t \t At Address &iArray[2] : %p\n", iArray[2], &iArray[2]);
    printf("iArray[3] = %d \t \t At Address &iArrat[3] : %p \n", iArray[3], &iArray[3]);
    printf("iArray[4] = %d \t \t At Address &iArray[4] : %p\n", iArray[4], &iArray[4]);
    printf("iArray[5] = %d \t \t At Address &iArray[5] : %p\n", iArray[5], &iArray[5]);
    printf("iArray[6] = %d \t \t At Address &iArray[5] : %p\n", iArray[6], &iArray[6]);
    printf("iArray[7] = %d \t \t At Address &iArray[7] %p\n", iArray[7], &iArray[7]);
    printf("iArray[8] = %d \t \t At Address &iArray[7] : %p\n", iArray[8], &iArray[8]);
    printf("iArray[9] = %d \t \t At Address &iArray[9] : %p \n", iArray[9], &iArray[9]);

    // ASSIGNING BASE ADDRESS OF INTGER ARRAY 'iArray' To INTEGER POINTER 'ptr+iArray'
    // NAme Of Any Array Is Its Own Base Address
    ptr_Array = iArray; //Same .. As ptr_Array = &iArray[0]

    //####Using Poinyer As Pointer i.e. Value of xthe Element of iArray : (ptr +iArray + x) And Address of xthe Elements of iArray : (ptr_Array + x)
    printf("\n\n");
    printf("***USing Pointer As Pointer i.e. Value of xthe Element of iArray : (ptr+iArray +x) And Address of xthe Element of iArray: (ptr_Array +x)***\n\n");
    printf("*(ptr_iArray + 0) = %d \t \t At Address (ptr_iArray + 0) : %p\n", *(ptr_Array + 0), (ptr_Array + 0));
    printf("*(ptr_iArray + 1) = %d \t \t At Address (ptr_iArray + 1) : %p \n", *(ptr_Array + 1), (ptr_Array + 1));
    printf("*(ptr_iArray + 2 )= %d \t \t At Address (ptr_iArray +2 ) : %p \n", *(ptr_Array + 2), (ptr_Array + 2));
    printf("*(ptr_iArray + 3 )= %d \t \t At Address (ptr_iArray +3 ) : %p \n", *(ptr_Array + 3), (ptr_Array + 3));
    printf("*(ptr_iArray + 4 )= %d \t \t At Address (ptr_iArray +4 ) : %p \n", *(ptr_Array + 4), (ptr_Array + 4));
    printf("*(ptr_iArray + 5 )= %d \t \t At Address (ptr_iArray +5 ) : %p \n", *(ptr_Array + 5), (ptr_Array + 5));
    printf("*(ptr_iArray + 6 )= %d \t \t At Address (ptr_iArray +6 ) : %p \n", *(ptr_Array + 6), (ptr_Array + 6));
    printf("*(ptr_iArray + 7 )= %d \t \t At Address (ptr_iArray +7 ) : %p \n", *(ptr_Array + 7), (ptr_Array + 7));
    printf("*(ptr_iArray + 8 )= %d \t \t At Address (ptr_iArray +8 ) : %p \n", *(ptr_Array + 8), (ptr_Array + 8));
    printf("*(ptr_iArray + 9) = %d \t \t At Address (ptr_iArray + 9) : %p\n", *(ptr_Array + 9), (ptr_Array + 9));

    return (0);

}

