#include <stdio.h>

int main(void)
{
    // code
    printf("\n\n");
    printf("----------------------------\n");

    printf("Goin on to Next Line.... using \\n Escape Sequence\n");
    printf("Demonstrating \t Horizontal \t Tab \t Using \t \\t Escape Sequence\n");
    printf("\"This is a Double Quoted output\" Done Using \\\" Escape Sequence\n");
    printf("\'This is a Single Quoted output\' Done Using \\\' Escape Sequence\n");
    printf("BACKSPACE Turned to BACKSPACE\b Using Escape Sequence \\b\n\n");

    printf("\r Demonstrating Carriage Return Using \\r Escape Sequence\n");
    printf("Demonstrating \r Carriage Return Using \\r Escape Sequence\n");
    printf("Demonstrating Carriage \r Return Using \\r Escape Sequence\n");

    printf("Demonstrating \x41 Using \\xhh Escape Sequence \n\n");
    // ox41 is the hexadecimal code for letter 'A'. 'xhh' is the placeholder for 'x'
    // followed by 2 digits (hh), altogether forming a Hexadecimal Number
    printf("Demonstrating \102 using \\ooo Escape Sequence\n");
    // 102 is the octal code for letter 'B'. 'ooo' is the placeholder for 3 digits forming an octal number.

    return(0);
}