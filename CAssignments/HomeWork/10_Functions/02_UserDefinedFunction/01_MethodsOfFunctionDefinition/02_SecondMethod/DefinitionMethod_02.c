#include<stdio.h>  //'stdio.h' contains declaration of 'printf()'

// Entry-Point Function => main() => Valid Return Type (int) and 3 Parameters (int argc, char *argv[], char *envp[])
int main(int argc, char * argv[], char * envp[])
{
    // function prototype / declarations / signature
    int MyAddition(void);

    // variable declaration
    int result;

    // code
    result = MyAddition();  // function call

    printf("\n\n");
    printf("Sum = %d\n\n", result);

    return 0;
}
// User defined function : Method of definition 01
// ***** Valid Return Value (int), No Parameters *****
int MyAddition(void)  // function definition
{
    // variable declarations : local variables to MyAddition()
    int a, b, sum;

    // code
    printf("\n\n");
    printf("Enter Integer Value for 'A' : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Integer Value for 'B' : ");
    scanf("%d", &b);

    sum = a + b;
    
    return(sum);
}