#include<stdio.h>

int main(void)
{
	//Variable Declarations
	int num = 5;
	int* const ptr = &num; // Read This Line From Right to Left => 'ptr' is a constant (const) pointer (*) to integer (int)"

	//Code
	printf("\n");
	printf("Current Value of 'num' = %d\n", num);
	printf("Current 'ptr' (Address of 'num') = %p\n", ptr);

	// The Following Lines Does Not Give Error...As We Are modifying The Value Of the Variable individually
	num++;
	printf("\n\n");
	printf("After num++, value of'num' = %d\n", num);

	//The Following Lines Gives Error And it hence commneted Out.
	//We cannot Alter The 'ptr' value as 'ptr' is constant pointer to integer
	//With respect to Pointer The Value it Points to is not constant but the pointer itself is constant
	//uncomment it and see the error.

	//ptr++;
	

	//The Following Line Does Not Give Error 
	//We do Not Get Error because we Are Changing the value at a constant pointer (address)
	//The Pointer is not constant .the value to which the Pointer points is not constant.

	(*ptr)++;

	printf("\n\n");
	printf("After ptr++, value of 'ptr' = %p\n", ptr);
	printf("Value at this new 'ptr' =%d\n", *ptr);
	printf("\n");
	return(0);
}

// CONCLUSION
//As 'ptr' is a Variable pointer to variable integer - we can change the Value of stored at address 'ptr' but we cannot change the 'ptr' (address) itself"
//We can change the value of the variable (num) individually whose address is contained in 'ptr'
//we can also change the value at address of ptr we can change the value of 'num' with respect to ptr => (*ptr)++ is allowed
//We cannot change the value of 'ptr' => That we cannot store a new Address inside 'ptr' => so ptr++ Is not Allowed

