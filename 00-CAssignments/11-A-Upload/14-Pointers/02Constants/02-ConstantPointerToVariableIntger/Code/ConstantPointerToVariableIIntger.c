#include<stdio.h>

int main(void)
{
    //variable declarations
    int num = 5;
    int* const ptr = &num ;//read this line from right to left= > ptr is a constant (const) pointer(*) to integer (int)

    //code
    printf("\n");
    printf("Current VAlue of num = %d\n",num);
    printf("Current ptr (Address of num = %p\n)",ptr);

    //The Following lined does not give error...as we are modifyng the value of the variable individually.
    num++;
    printf("\n\n");
    printf("After num++,value of num = %d\n",num);

    //The Following line give errror and is hencce commneted out.
    //we cannot alter the ptr value as ptr is  a constant pointer to intger
    // we respeect to the pointer  the value it points tro is not constants but yhe pointer itself is constant.
    //Uncomment it and see the errro.

    // ptr++;

    //The Following lies does not give error
    //we do not get error because we are changing the value at a constant a pointer(addre).
    //The pointer is constant.The value to which the pointer pointer is not constant.
    (*ptr)++;

    printf("\n\n");
    printf("After (*ptr)++,Value of 'ptr' = %p\n",ptr);
    printf("Value at this 'ptr' =%d\n",*ptr);
    printf("\n\n");
    return 0;

}
//Conclusion:
//As ptr is a constamt pointer to a variable intger -we can change the value stored at address ptr but we cannot change 'pptr (Address) itself.
//we can change the value oif th variable (num) individually -whose address is conatined in ptr.
// we can also change the value at address of ptr.we can change the value of num with respect to ptr => (*ptr)++ is allowed
//we cannot chnage the value of 'ptr' =>That us we cannot store a new address inside 'ptr' =>So,ptr++ is not Allowed