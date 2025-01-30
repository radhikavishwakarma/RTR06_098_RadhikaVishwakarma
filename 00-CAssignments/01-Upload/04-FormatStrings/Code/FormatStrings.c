#include <stdio.h>

int main(void)
{
    // code
    printf("\n\n");
    printf("---------------------------\n");

    printf("Hello Words !!!\n\n");

    int a = 13;
    printf("Integer Decimal Value of 'a' = %d\n", a);
    printf("Integer Octal Value of 'a' = %o\n", a);
    printf("Integer Hexadecimal Value of 'a' (Hexadecimal Letters in lower case) = %x\n", a);
    printf("Integer Hexadecimal Value of 'a' (Hexadecimal Letters in upper case) = %X\n", a);

    char ch = 'A';
    printf("Character ch = %c\n", ch);
    char str[] = "AstroMediComp's Real Time Rendering Batch";
    printf("String str = %s\n", str);

    long num = 30121995L;
    printf("Long Integer = %ld\n", num);

    unsigned int b = 10;
    printf("Unsigned Integer 'b' = %u\n", b);

    float f_num = 3012.1996f;
    printf("Floating Point Number with just %%f 'f_num' = %f\n", f_num);
    printf("Floating Point Number with just %%4.2f 'f_num' = %4.2f\n", f_num);
    printf("Floating Point Number with just %%6.5f 'f_num' = %6.5f\n", f_num);

    double d_pi = 3.14159265358979323846;
    printf("Double precision floating point number without exponential = %g\n", d_pi);
    printf("Double precision floating point number with exponential (Lower case) = %e\n", d_pi);
    printf("Double precision floating point number with exponential (Upper case) = %E\n", d_pi);
    printf("Double Hexadecimal value of 'd_pi' (Hexadecimal in Lower case) = %a\n", d_pi);
    printf("Double Hexadecimal value of 'd_pi' (Hexadecimal in Upper case) = %A\n", d_pi);

    printf("---------------------------\n");
    return(0);
}