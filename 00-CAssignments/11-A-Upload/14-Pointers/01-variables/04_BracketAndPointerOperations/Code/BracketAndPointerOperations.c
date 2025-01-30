#include<stdio.h>

int main(void)
{
    //variable declarations
    int num;
    int* ptr = NULL;
    int ans;

    //code
    num = 5;
    ptr = &num;

    printf("\n\n");
    printf("num     =%d\n",num);
    printf("&num        =%p\n",&num);
    printf("*(&num)     =%d\n",*(&num));
    printf("ptr     =%p\n",ptr);
    printf("*ptr    =%d\n",*ptr);

    printf("\n\n");

    //Add 10 to ptr which is th address of num...
    //Hence 10 will be added to the addres if num and the resultant address will be displayed
    printf("Answer of (ptr+10)  =%p\n",(ptr+10));

    // add 10 to ptr which is thr address if num and give value at the new addres..
    //Hence 10 will be added to the address of num and ye value at resulatnt address will be displayed...
    printf("Answer of*(ptr+10) =%d\n",*(ptr+10));

    //Add 10 to *ptr which is the value at address of num (i.e :num i.e. 5) and give new value .without any change in any address..
    //hence 10 will be added to the ptr (num =5) and thr esulttant value will be given(*ptr + 10) =(num +10) = (5_10) =15..

    printf("Answer if *(ptr+!) =%d\n",*(ptr+10));

    //add 10 to *ptr which is the valuye at address of num(i.e : num i.e.:5) and give new value .withput any change in any address....
    //Hence,10 will be added to the *ptr (num =6) and the resultant value will be gibve (*ptr +10) = (num+10) (5_+10) 15..
    printf("Answer of(*ptr+10) = %d\n\n",(*ptr+10));

    //**ASSOCIATIVITY OF *(Value At Address) and ++Add -- Operator Is fromRight Left */

    //(RIGHT TO LEFT) Consider value *ptr ..Preincreamnet *ptr..that is,value at address ptr.i.e. *ptr is preincremanents(++*ptr)
    ++*ptr;//*ptr is preincremenented,,,*ptr us 5,,,after execytion of this statement...*ptr = 6
    printf("Answer of ++*ptr : %d\n",*ptr);//Bracketed not necessary for preincrement.predecrement

    //(RIGHT TO LEFT) Post-increment address ptr... That is address ptr.i.e.ptr is post-incremeneted(ptr++) and then the value at the new address if Displayed(*ptr++)..
    *ptr++;//Incorrect method of postincrementing a value using pointer...
    printf("Answer of *ptr++ :%d\n",*ptr);// Brackted Are neesary fot postincrement.post-decrement

    //(RIGHT TO LEFT) Post-increment value *ptr ...that us value at address ptr i.e. *ptr is post-increments(*ptr)++
    ptr = &num;
    (*ptr)++;//Correct method of post-incremenung a value using pointer...
    //*ptr us 6... at thi state,ent *ptr remains 6 but at next stateent *ptr 7 (post increment)
    printf("Answer of(*ptr)++ : %d\n\n",*ptr);//Brackted are necessary for post increment/post decrement

    return 0;
}