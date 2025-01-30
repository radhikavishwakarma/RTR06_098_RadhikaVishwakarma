#include<stdio.h>  //'stdio.h' contains declaration of 'printf()'

// Entry-Point Function => main() => Valid Return Type (int) and 3 Parameters (int argc, char *argv[], char *envp[])
int main(int argc, char * argv[], char * envp[])
{
    // function prototype / declarations / signature
    int MyAddition(int, int);

    // variable declaration : local variables to main
    int a, b, result;

    // code
    printf("\n\n");
    printf("Enter Integer Value for 'A' : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Integer Value for 'B' : ");
    scanf("%d", &b);
    result = MyAddition(a, b);  // function call

    printf("\n\n");
    printf("Sum of %d and %d = %d\n\n", a, b, result);

    return 0;
}
// User defined function : Method of definition 01
// ***** Valid Return Value (int), Valid Parameters (int, int) *****
int MyAddition(int a, int b)  // function definition
{
    // variable declarations : local variables to MyAddition()
    int sum;

    // code
    sum = a + b;
    return(sum);
}