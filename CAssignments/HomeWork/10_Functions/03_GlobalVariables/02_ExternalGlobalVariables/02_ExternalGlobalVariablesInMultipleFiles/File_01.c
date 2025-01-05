#include <stdio.h>

extern int global_count;

void change_count_one(void)
{
    // code
    global_count = global_count + 1;
    printf("change_count_one() : Value of global_count in File_01 = %d\n", global_count);
}