#include<stdio.h>

int main(void)
{
	//Variable Declarations
	int num = 5;
	const int* const ptr = &num; // Read This Line From Right to Left => 'ptr' is a constant (const) pointer (*) to integer (int) constant (const)"

	//Code
	printf("\n");
	printf("Current Value of 'num' = %d\n", num);
	printf("Current 'ptr' (Address of 'num') = %p\n", ptr);

	// The Following Lines Does Not Give Error...As We Are modifying The Value Of the Variable individually
	num++;
	printf("\n\n");
	printf("After num++, value of'num' = %d\n", num);

	//The Following Lines Gives Error And it hence commneted Out.
	//We cannot Alter The 'ptr' value as 'ptr' is constant pointer to constant integer
	//With respect to Pointer The Value it Points to is  constant and  the pointer itself is also constant
	//uncomment it and see the error.

	//ptr++;


	//The Following Line also Give Error and is hence commented Out
	//We can Not Alter the value stored in 'ptr' as 'ptr' is A constant pointer to constant Pointer to constant integer"
	//With respect to the Pointer the value it points to is constant AND the pointer itself is also constant
	//uncomment it and see the error.

	//(*ptr)++;

	return(0);
}

// CONCLUSION
//As 'ptr' is a Constant pointer to Constant integer - we cannot change the Value of stored at address 'ptr' AND we cannot change the 'ptr' (address) itself"
//We can change the value of the variable (num) individually whose address is contained in 'ptr'
//we cannot also change the value at address of ptr we cannot change the value of 'num' with respect to ptr => (*ptr)++ is Not allowed
//We cannot change the value of 'ptr' => That we cannot store a new Address inside 'ptr' => so ptr++ Is not Allowed

