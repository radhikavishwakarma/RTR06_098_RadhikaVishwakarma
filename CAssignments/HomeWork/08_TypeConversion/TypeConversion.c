#include<stdio.h>

int main(void)
{
    // vvariable declaration
    int i, j;
    char ch_01, ch_02;

    int a, result_int;
    float f, result_float;

    int i_explicit;
    float f_explicit;

    // code
    printf("\n\n-----------------------------------------------------\n\n");

    // Inter-conversion and IMPLICIT type casting between 'char' and 'int' types
    i = 70;
    ch_01 = i;
    printf("I = %d\n", i);
    printf("Character 1 (after ch_01 = i) = %c\n\n", ch_01);

    ch_02 = 'Q';
    j = ch_02;
    printf("Character 2 = %c\n", ch_02);
    printf("J (after j = ch_02) = %c\n\n", j);

    // IMPLICIT Conversion of 'int' to 'float'
    a = 5;
    f = 7.8f;
    result_float = a + f;
    printf("Integer a = %d and Floating-point Number %f added gives Floating-point Sum = %f\n\n", a, f, result_float);

    result_int = a + f;
    printf("Integer a = %d and Floating-point Number %f added gives Floating-point Sum = %d\n\n", a, f, result_int);

    // EXPLICIT type casting using CAST operator
    f_explicit = 30.121995f;
    i_explicit = (int)f_explicit;
    printf("Floating point number which will be TYPE CASTED EXPLICITLY = %f\n", f_explicit);
    printf("Resultant INTEGER after EXPLICIT TYPE CASTING of %f = %d\n", f_explicit, i_explicit);
    return(0);
}