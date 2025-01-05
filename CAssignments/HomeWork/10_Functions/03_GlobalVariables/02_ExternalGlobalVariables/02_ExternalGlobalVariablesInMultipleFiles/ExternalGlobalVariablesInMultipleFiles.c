#include <stdio.h>

// *** GLOBAL SCOPE ***
int global_count = 0;

int main(void)
{
    // function prototypes
    void change_count_one(void);
    void change_count_two(void);    // function defined in File_01.c
    void change_count_three(void);  // function defined in File_02.c

    // code
    printf("\n");

    change_count_one();
    change_count_two();     // function defined in File_01.c
    change_count_three();   // function defined in File_02.c

    return 0;
}

// *** GLOBAL SCOPE ***
void change_count_one(void)
{
    // code
    global_count = global_count + 1;
    printf("Global Count = %d\n", global_count);
}

// *** GLOBAL SCOPE ***
void change_count_two(void)
{
    // code
    global_count = global_count + 1;
    printf("change_count_two(): Value of global_count = %d\n", global_count);
}

// *** GLOBAL SCOPE ***
void change_count_three(void)
{
    // code
    global_count = global_count + 10;
    printf("change_count_three(): Value of global_count = %d\n", global_count);
}