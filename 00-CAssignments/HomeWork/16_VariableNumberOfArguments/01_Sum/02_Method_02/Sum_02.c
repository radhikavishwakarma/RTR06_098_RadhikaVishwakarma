#include <stdio.h>
#include <stdarg.h>

int main(void)
{
    // Variable declarations
    int answer;

    // Code
    printf("\n\n");

    answer = calcSum(5, 10, 20, 30, 40, 50);
    printf("Answer : %d\n\n", answer);

    answer = calcSum(10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
    printf("Answer : %d\n\n", answer);

    answer = calcSum(0); 
    printf("Answer : %d\n\n", answer);

    return (0);
}

int calcSum(int num, ...) // Variadic Function
{
    // Variable declarations
    int sum_total = 0;
    int n;

    va_list numbers_list;

    // Code
    va_start(numbers_list, num);

    while (num)
    {
        n = va_arg(numbers_list, int);
        sum_total += n;
        num--;
    }

    va_end(numbers_list);
    return sum_total;
}
