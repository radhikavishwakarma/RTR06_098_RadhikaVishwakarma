#include<stdio.h>

int main(void)
{
    //variabldecalrations
    int iArray[] = { 10,20,30,40,50,60,70,80,90,100 };//Intger Array
    int* ptr_iArray = NULL; //Integer Pointer

    // code

    // ####USing Array NAme As Pointer i.e.  : Value of x  the element of iArray : *(iArray +x) Adn Address if xth Element of iArray : (iArray +x)
    printf("\n\n");
    printf("***Using Array Name As A pointer i.e. : Value of x the Elememt of iArray  : *(iArray +x) And Address x th element of iArray : (iArray +x)***\n\n");
    printf("Integer  Array Elememt And Their Addresses : \n\n");
    printf("*(iArray + 0) = %d \t At Address (iArray + 0) :%p\n", *(iArray + 0), (iArray + 0));
    printf("*(iArray + 1) = %d \t At Address (iArray +1) :%p\n", *(iArray + 1), (iArray + 1));
    printf("*(iArray +2 ) = %d \t At Address (iArray +2) :%p\n", *(iArray + 2), (iArray + 2));
    printf("*(iArray + 3) = %d \t At Address (iArray + 3) :%p \n", *(iArray + 3), (iArray + 3));
    printf("*(iArray + 4 ) = %d \t At Address (iArray + 4) :%p \n", *(iArray + 4), (iArray + 4));
    printf("*(iArray + 5) = %d \t At Address (iArray +5) :%p \n", *(iArray + 5), (iArray + 5));
    printf("*(iArray + 6) = %d \t At Address (iArray + 6) :%p\n", *(iArray + 6), (iArray + 6));
    printf("*(iArray + 7) = %d \t At Address (iArray + 7) :%p \n", *(iArray + 7), (iArray + 7));
    printf("*(iArray + 8 )= %d \t At Address (iArray + 8) :%p \n", *(iArray + 8), (iArray + 8));
    printf("*(iArray + 9) = %d \t At Address (iArray + 9) :%p \n", *(iArray + 9), (iArray + 9));

    // Asssigning Base address of Intger Arrat iArray' To Intger pointer 'ptr_iArray'
    ptr_iArray = iArray;//Same as ...ptr_iArray = &iArray[0]

    //***Using Pointer as array name i.e. Vaue of xthe Element of iArray : ptr_iArray[x] And Address of xthe element of aiArray : &ptr_iArray[x] *****
    printf("\n\n");
    printf("***USing pointer as arrat i.e. Valye of xxth Element of iArray ptr_iArray[x] And addres sof xth Element of iArray : &ptr_iArray[x] ***\n\n");
    printf("Intger Array Element and Their Addresses : \n\n");
    printf("ptr_iArray[0] =%d \t At Addres &ptr_iArray[0] : %p\n ", ptr_iArray[0], &ptr_iArray[0]);
    printf("ptr_iArray[1] = %d \t At address &ptr_iArray[1] : %p \n", ptr_iArray[1], &ptr_iArray[1]);
    printf("ptr_iArray[2] = %d \t At address &ptr_iArray[2] : %p \n", ptr_iArray[2], &ptr_iArray[2]);
    printf("ptr_iArray[3] = %d \t At address &ptr_iArray[3] : %p \n", ptr_iArray[3], &ptr_iArray[3]);
    printf("ptr_iArray[4] = %d \t At address &ptr_iArray[4] : %p \n", ptr_iArray[4], &ptr_iArray[4]);
    printf("ptr_iArray[5] = %d \t At address &ptr_iArray[5] : %p \n", ptr_iArray[5], &ptr_iArray[5]);
    printf("ptr_iArray[6] = %d \t At address &ptr_iArray[6] : %p \n", ptr_iArray[6], &ptr_iArray[6]);
    printf("ptr_iArray[7] = %d \t At address &ptr_iArray[7] : %p \n", ptr_iArray[7], &ptr_iArray[7]);
    printf("ptr_iArray[8] = %d \t At address &ptr_iArray[8] : %p \n", ptr_iArray[8], &ptr_iArray[8]);
    printf("ptr_iArray[9] = %d \t At address &ptr_iArray[9] : %p \n", ptr_iArray[9], &ptr_iArray[8]);

    return (0);

}
