#include<stdio.h>

int main(void)
{
    //variable declarations
    int num;
    int* ptr = NULL;
    int** pptr = NULL; //Declarations method 1 - **pptr is a variable of type int

    //code
    num = 10;

    printf("\n\n");
    printf("*****Before ptr = &num*****\n\n");
    printf("Value of num  = %d \n\n", num);
    printf("Address of num  = %p\n\n", &num);
    printf("Value At Address of num = %d \n\n", *(&num));

    //Assigning address of variable num to pointer variable ptr
    //ptr now conatins address of num ..hence ptr is same as &num
    ptr = &num;

    printf("\n\n");
    printf("*****After ptr = &num****\n\n");
    printf("Value of num     = %p\n\n", ptr);
    printf("Address of num = %p\n\n", ptr);
    printf("Value At Address of num = %d\n\n", *ptr);

    //Assigning address of variable ptr to  pointer -to-pointer variable pptr 
    //pptr now contains the address of ptr which in turn contains the address of num
    //hence pptr is same as &ptr
    //ptr is same &num
    //hence pptr = &ptr - &(&num)
    // If pptr = &num = and *ptr =*(&num) = value at address of num
    //Then,pptr = &ptr and * pptr = *(&ptr) = ptr = Value at address of ptr
     // i .e. ptr i.e. = address of num
     //then ,**pptr = **(&ptr) = *(*(&ptr) = *ptr = *(&num) = num = 10)
     //Hence,num = *(&num) = *ptr = *(*pptr) = **pptr

    pptr = &ptr;

    printf("\n\n");
    printf("****After  pptr = &ptr ****\n\n");
    printf("Value of num    = %d\n\n", num);
    printf("address of num (ptr )   = %p\n\n", ptr);
    printf("Address of ptr (pptr) = %p\n\n", pptr);
    printf("Value At address of ptr (*pptr) = %p\n\n", *pptr);
    printf("Value At address of num (*ptr) (*pptr) = %d\n\n", **pptr);

    return (0);
}
