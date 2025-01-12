#include<stdio.h>

int main(void)
{
	//Variable Declarations
	int num = 5;
	const int* ptr = NULL; // Read This Line From Right to Left => 'ptr' is a pointer (*) to integer (int) constant (const)"

	//Code
	ptr = &num;
	printf("\n");
	printf("Current Value of 'num' = %d\n", num);
	printf("Current 'ptr' (Address of 'num') = %p\n",ptr);

	// The Following Lines Does Not Give Error...As We Are modifying The Value Of the Variable individually
	num++;
	printf("\n\n");
	printf("After num++, value of'num' = %d\n", num);

	//The Following Lines Gives Error And it hence commneted Out.
	//We cannot Alter The Value Stored In 'A Pointer to constant integer'
	//With respect to Pointer The Value it Points Should be constant 
	//uncomment it and see the error.

	// (*ptr)++;

	//The Following Line Does Not Give Error 
	//We do Not Get Error because we Are Changing the Pointer (address).
	//The Pointer is not constant .the value to which the Pointer points its constant.

	ptr++;

	printf("\n\n");
	printf("After ptr++, value of 'ptr' = %p\n", ptr);
	printf("Value at this new 'ptr' =%d\n", *ptr);
	printf("\n");
	return(0);
}

// CONCLUSION
//As 'ptr' is a Variable pointer to constant integer - we can change the Value of Pointer "ptr"
//We can change the value of the variable (num) individually whose address is contained in 'ptr'
//So in short we cannot change the value at address of 'ptr' we cannot change the value of num with respect to 'ptr' => (*ptr)++
// is NOT allowed
//But we can Change the address 'ptr' itself => so ptr++ is allowed.

