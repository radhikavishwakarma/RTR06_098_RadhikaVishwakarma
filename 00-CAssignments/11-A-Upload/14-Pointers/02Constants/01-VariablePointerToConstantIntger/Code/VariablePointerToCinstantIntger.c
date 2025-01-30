#include<stdio.h>

int main(void)
{
    //variable declarations
    int num = 5;
    const int*ptr = NULL;//read this line from right to left =>left = ptr is a pointer(*) to intger (int) constant (const)

    //code
    ptr = &num ;
    printf("\n");
    printf("Current Value of num = %d\n",num);
    printf("Current ptr (Address of num) =%p\n",ptr);

    //Th following line does not give errro ...as we are modifynng the value of the variable individually
    num++;
    printf("\n\n");
    printf("After num++,value oif num = %d\n",num);

    //Teh Following line gives error and is hence commendted out.
    // we cannot alter the value stoired A pointer to constant intger
    //with respect to the pointer,the vaue it point to should be constant.
    // Uncomment it and se error.

    // (*ptr)++;
    
    //The Following line does not give errro
    // we do nit get error because we are changing the pointer (addre).
    //the pointer is nit constant.The Value to whcih the pointer points is constant.
    ptr++;

    printf("\n\n");
    printf("After ptr++,value of ptr = %p\n",ptr);
    printf("Value At this new ptr = %d\n",*ptr);
    printf("\n");
    return 0;

}
// Conclusion
//As ptr is variable pointer to cinstant intger - we can change the value of pointer ptr
//we can change the value of the variable (num) individually -whocse address if contained in ptr
//so in short we cannot change the value at address of ptr -we cannot change
//the value of num with respect to ptr => (*ptr)++ is not allowed.
//But we can change the address ptr itself =>so,ptr++ isallowed.