#include<stdio.h>

int main(void)
{
    //varible declaratiosn
    char ch;
    char *ptr = NULL; //Delcarations Method 1 - *ptr is a variable of type char

    //code
    ch = 'A';

    printf("\n\n");
    printf("**********Before ptr = &ch ***********\n\n");
    printf("Value of ch     = %c\n\n",ch);
    printf("Address of ch      =%p\n\n",&ch);
    printf("Value At address of ch = %c\n\n",*(&ch));

    //Assigning adddre of variable ch to pointer variable ptr */
    //ptr now conatins address of ch ...hence ptr is same &ch
    ptr = &ch;

    printf("\n\n");

    printf("*****After ptr = &ch*********\n\n");
    printf("Value of ch         =%c\n\n",ch);
    printf("Address of ch       %p\n\n",ptr);
    printf("Value At address of ch  =%c \n\n",*ptr);

    return 0;
}