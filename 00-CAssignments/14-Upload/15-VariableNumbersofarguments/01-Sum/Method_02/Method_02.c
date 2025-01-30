#include<stdio.h>
#include<stdarg.h>

int main(void)
{
    // function prototype
    int calcSum(int, ...);

    // variable declarations
    int answer;

    // code
    printf("\n\n");
    answer = calcSum(5, 10, 20,30,40,50);
    printf("Answer : %d\n\n", answer);

    answer = calcSum(10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
    printf("Answer : %d\n\n", answer);

    answer = calcSum(0);
    printf("Answer : %d\n\n", answer);

    return (0);
}

int calcSum(int num, ...)
{
    // variable declarations
    int total = 0;
    int n;

    va_list num_list;

    // code
    va_start(num_list, num);

    total = va_Sum(num_list, num);

    va_end(num_list);

    return (total);
}

int va_Sum(int num_list, int num)
{
        int total = 0, n;

        while (num)
        {
            n = va_arg(num_list, int);
            total = total + n;
            num--;
        }

        return total;
}