#include<stdio.h>

int main(void)
{
    //variable declarations
    int num = 5;
    const int* const ptr = &num; //Read this line from right to left=>ptr is a cinstant (const) pointer(*) to intger (int) constant (const).

    //code
    printf("\n");
    printf("Current Value of num = %d\n",num);
    printf("Current ptr (Addres of num) =%p\n",ptr);

    //Teh following lies does not give ..errror as we are modifyning the value of the variable individually
    num++;
    printf("\n\n");
    printf("After num++,value of num =m%d\n",num);

    //The Foloowing line give error is and hence commented out.
    //we cannot alter the 'ptr; value as ptr is  a constant pointer to constant intger.".
    //with repsect to the pointer. the value it poins to is constant AND th pointer itself is alsom constant.
    //Uncomment it and see the error.

    //ptr++;

    //The Following line also give error and is hence commendted out.
    //we cannot alter the value stored in 'ptr; as 'ptr' is A" constant pointer to constant intger.
    //with repsect to the pointer ,the value it points to is constant AND the pointer itself is also constant.
    //Uncomment it and see the error.

    // (*ptr)++;

    return 0;

}
//Conclusion:
// As 'ptr' is a "constant pointer to a constant intger"- we cannot change the value stored at addres "ptr" AND we cannot change the ptr (Address) itself.
//We can change the value of the avriable (num) individually-whose address is conatined in"ptr".
//we cannot also change th e"the value at address of ptr=we cannot change the value of  "num"witgh respect to "ptr" =>(*ptr)++is nOT allowed
//we cannot change the value of 'ptr' =>That us wecannot store a new address inside 'ptr' => So,,ptr++ is also NOT allowed