#include<stdio.h>

struct MyStruct
{
	int i;
	float f;
	double d;
	char c;
};

union MyUnion
{
	int i;
	float f;
	double d;
	char c;
};

int main(void)
{
	// Variable Declarations
	struct MyStruct s;
	union MyUnion u;

	//Code
	printf("\n\n");
	printf("Size Of MyStruct = %zu\n", sizeof(s));
	printf("\n\n");
	printf("Size Of MyUnion = %zu\n", sizeof(u));
	return(0);
}
