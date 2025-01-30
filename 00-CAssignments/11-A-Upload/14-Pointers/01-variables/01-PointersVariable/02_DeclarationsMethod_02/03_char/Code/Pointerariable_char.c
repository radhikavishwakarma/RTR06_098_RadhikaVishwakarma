#include<stdio.h>

int main(void)
{
    //variable declarations
    char ganii_ch;
    char* ganii_ptr = NULL; //Declarations Method 2 -ptr is a variable of type char*

    //code
    ganii_ch ='A';

    printf("\n\n");

    printf("******Before ptr = &ch*****\n\n");
    printf("Value of ch         %c\n\n",ganii_ch);
    printf("Address of ch       %p\n\n",&ganii_ch);
    printf("Value At Address of ch  = %c\n\n",*(&ganii_ch));

    //Assigning address of variable ch to pointer variable ptr
    //ptr now contains address of ch..hence ptr is ssame as &ch 
    ganii_ptr = &ganii_ch;

    printf("\n\n");

    printf("****After ptr = &ch*****\n\n");
    printf("Value of ch         %c\n\n",ganii_ch);
    printf("Address of ch       =%p\n\n",ganii_ptr);
    printf("Value At Address of ch      = %c\n\n",*ganii_ptr);

    return 0;
}