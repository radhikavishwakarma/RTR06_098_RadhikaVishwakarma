#include<stdio.h>  //'stdio.h' contains declaration of 'printf()'
// ***** User defined Functions : Method of calling function 1 *****
// Calling all functions in main() directly *****

// Entry-Point Function => main() => Valid Return Type (int) and 3 Parameters (int argc, char *argv[], char *envp[])
int main(int argc, char * argv[], char * envp[])
{
    // function prototype / declarations / signature
    void MyAddition(void);
    int MySubtraction(void);
    void MyMultiplication(int, int);
    int MyDivision(int, int);

    // variable declaration : local variables to main
    int result_subtraction;
    int a_multiplication, b_multiplication;
    int a_division, b_division, result_division; 

    // code

    // *** ADDITION ***
    MyAddition();  // function call

    // *** SUBTRACTION ***
    result_subtraction = MySubtraction(); // function call
    printf("\n\n");
    printf("Subtraction Yields Result = %d\n", result_subtraction);

    // *** MULTIPLICATION ***
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Multiplication : ");
    scanf("%d", &a_multiplication);

    printf("\n\n");
    printf("Enter Integer Value For 'B' For Multiplication : ");
    scanf("%d", &b_multiplication);

    MyMultiplication(a_multiplication, b_multiplication); //function call

    // *** DIVISION ***
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Division : ");
    scanf("%d", &a_division);

    printf("\n\n");
    printf("Enter Integer Value For 'B' For Division : ");
    scanf("%d", &b_division);
    
    result_division = MyDivision(a_division, b_division); // function call
    printf("\n\n");

    printf("Division of %d and %d gives = %d (Quotient)\n", a_division, b_division, result_division);
    printf("\n\n");

    return 0;
}
// User defined function : Method of definition 01
// ***** No Return Value, No Valid Parameters *****
void MyAddition(void)  // function definition
{
    // variable declarations : local variables to MyAddition()
    int a, b, sum;

    // code
    printf("\n\n");
    printf("Enter Integer Value for 'A' For Addition : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Integer Value for 'B' For Addition : ");
    scanf("%d", &b);

    sum = a + b;
    printf("\n\n");
    printf("Sum of %d and %d = %d\n\n", a, b, sum);
}

// *** Function Definition of MySubtraction() ***
int MySubtraction(void)
{
    // variable declarations :  local variables to MySubtraction()
    int a, b, subtraction;

    // code
    printf("\n\n");
    printf("Enter Integer Value for 'A' For Subtraction : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Integer Value for 'B' For Subtraction : ");
    scanf("%d", &b);

    subtraction = a - b;
    return(subtraction);
}

// *** Function Definition of MyMultiplication() ***
void MyMultiplication(int a, int b)
{
    // variable declarations : local variables to MyMultiplication()
    int multiplication;

    // code
    multiplication = a * b;

    printf("\n\n");
    printf("Multiplication of %d and %d = %d\n\n", a, b, multiplication);
}

// *** Function Definition of MyDivision() ***
int MyDivision(int a, int b)
{
    // variable declarations : local variables to MyDivision()
    int division_quotient;

    // code
    if (a > b)
        division_quotient = a / b;
    else
        division_quotient = b / a;
    
    return(division_quotient);
}
