#include<stdio.h>  //'stdio.h' contains declaration of 'printf()'

// Entry-Point Function => main() => Valid Return Type (int) and 3 Parameters (int argc, char *argv[], char *envp[])
int main(int argc, char * argv[], char * envp[])
{
    // function prototype / declarations / signature
    void MyAddition(void);

    // code
    MyAddition();  // function call

    return 0;
}
// User defined function : Method of definition 01
// ***** No Return Value, No Parameters *****
void MyAddition(void)  // function definition
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
    printf("\n\n");
    printf("Sum of %d and %d = %d\n\n", a, b, sum);
}