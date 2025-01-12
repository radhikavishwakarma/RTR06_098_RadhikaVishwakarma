#include<stdio.h>

int main(void)
{
	//Variable Declarations
	int num;
	int* ptr = NULL;
	int ans;

	//Code
	num = 5;
	ptr = &num;

	printf("\n\n");
	printf(" num =  %d\n", num);
	printf(" &num =  %p\n", &num);
	printf(" *(&num) =  %d\n", *(&num));
	printf(" ptr =  %p\n", ptr);
	printf(" *ptr =  %d\n", *ptr);

	printf("\n\n");

	//ADD 10 TO 'ptr' which is the address of num....
	//Hence 10 will be Added to the address of 'num' and the resultant address will be displayed
	printf("Answer of(ptr + 10) = %p\n", (ptr + 10));

	//ADD 10 TO 'ptr' which is the address of num and give the value at the row address...
	//Hence 10 will be Added to the Address of 'num' and the resultant address will be displayed.
	printf("Answer of *(ptr + 10) = %d\n", *(ptr + 10));

	//Add 10 to '*ptr' which is the value at address of 'num' (i.e : 'num' i.e: 5) and give new value without any change in any address..
	//hence 10 will be added to the '*ptr' (num = 5) and the resultatnt value will be given (*ptr + 10) = (num + 10) = (5 + 10) = 15...
	printf("Answer of *(ptr + 10) = %d\n\n", (*ptr + 10));

	//Associativity of (value at address) and ++ and -- operators is from right to left

	//(RIGHT TO LEFT) consider value *ptr .... pre-increment *ptr... that is value at address 'ptr' i.e: *ptr is 
	//Pre incremented(++*ptr)
	++ptr; //*ptr is pre-incremented....*ptr is 5... after execution of this statement..ptr = 6
	printf("Answer of ++*ptr : %d\n", *ptr); // Brackets no necessary for pre increment / pre-decrement

	//(RIGHT TO LEFT) post incremented address ptr.... that is address 'ptr' i.e. ptr is post-incremented (ptr++) and then the value at new address is displayed (*ptr++)..
	++ptr; //Incorrect method of post-incrementing a value using pointer
	printf("Answer of ++*ptr : %d\n", *ptr); //Breckets are neccessary for post increment / post decrement

	// (RIGHT TO LEFT) Psot-increment value *ptr ...that is values at address 'ptr' i.e. *ptr is post incremented (*ptr)++
	ptr = &num;
	(*ptr)++;//correct method of post incrementing a value using Pointer *ptr is 6 at this statement *ptr remains 6 but at next statement *ptr = 7 (post-increment)
	printf("Answer of ++ (*ptr) : %d\n\n", *ptr); //Breckets are necessary for post-increment / post-decrement

	return(0);
}
