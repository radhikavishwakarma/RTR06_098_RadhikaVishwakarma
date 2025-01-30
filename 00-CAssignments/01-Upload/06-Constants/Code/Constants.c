#include <stdio.h>
#define MY_PI 3.1415926535897932
#define AMC_STRING "AstroMediComp RTR 2024"

// If first constant is not assigned a value, it is assumed to be 0 i.e, 'SUNDAY' will be 0
// And the rest of the constants are assigned consecutive integer values
// from 0 onwards i.e, 'MONDAY' will be 1, 'TUESDAY' will be 2, and so on....

// Un-named enums
enum
{
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
};

enum
{
    JANUARY,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
};

// Named enums
enum Numbers
{
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE = 5,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN
};

enum boolean
{
    TRUE = 1,
    FALSE = 0
};

int main(void)
{
    // local constant declarations
    const double epsilon = 0.000001;

    // code
    printf("\n\n");
    printf("---------------------------\n");

    printf("Local constant Epsilon = %lf\n", epsilon);
    printf("Sunday is day number = %d\n", SUNDAY);
    printf("Monday is day number = %d\n", MONDAY);
    printf("Tuesday is day number = %d\n", TUESDAY);
    printf("Wednesday is day number = %d\n", WEDNESDAY);
    printf("Thursday is day number = %d\n", THURSDAY);
    printf("Friday is day number = %d\n", FRIDAY);
    printf("Saturday is day number = %d\n", SATURDAY);

    printf("\n\n");
    printf("---------------------------\n");

    printf("January is month number = %d\n", JANUARY);
    printf("February is month number = %d\n", FEBRUARY);
    printf("March is month number = %d\n", MARCH);
    printf("April is month number = %d\n", APRIL);
    printf("May is month number = %d\n", MAY);
    printf("June is month number = %d\n", JUNE);
    printf("July is month number = %d\n", JULY);
    printf("August is month number = %d\n", AUGUST);
    printf("September is month number = %d\n", SEPTEMBER);
    printf("October is month number = %d\n", OCTOBER);
    printf("November is month number = %d\n", NOVEMBER);
    printf("December is month number = %d\n", DECEMBER);

    printf("\n\n");
    printf("---------------------------\n");

    printf("Value of TRUE is = %d\n", TRUE);
    printf("Value of FALSE is = %d\n", FALSE);

    printf("\n\n");
    printf("---------------------------\n");
    printf("MY_PI Macro value = %.10lf\n", MY_PI);
    printf("Area of circle of radius 2 units = %f\n", (MY_PI * 2.0f * 2.0f));
    // pi * r * r = area of circle of radius 'r'

    printf("\n\n");
    printf("---------------------------\n");
    printf(AMC_STRING);
    printf("\n---------------------------\n");
    
    return(0);
}