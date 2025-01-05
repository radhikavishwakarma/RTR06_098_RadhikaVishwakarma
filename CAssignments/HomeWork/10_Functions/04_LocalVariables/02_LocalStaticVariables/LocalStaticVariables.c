#include <stdio.h>

// *** GLOBAL SCOPE ***

int main(void)
{
    // *** LOCAL SCOPE OF main() begins ***

    // variable declarations
    // 'a' is a local variable. It is local to main() only.
    int a = 5;

    // function prototypes
    void change_count(void);

    // code
    printf("\n");
    printf("A = %d\n\n", a);

    // local_count is initialized to 0
    // local_count = local_count + 1 = 0 + 1 = 1
    change_count();

    /*
    Since, 'local_count' is an ordinary local variable of change_count(), it
    will NOT retain it's value from previous call to change_count().
    So local_count is AGAIN initialized to 0
    local_count = local_count + 1 = 0 + 1 = 1
    */
    change_count();

    /*
    Since, 'local_count' is an ordinary local variable of change_count(), it
    will NOT retain it's value from previous call to change_count().
    So local_count is AGAIN initialized to 0
    local_count = local_count + 1 = 0 + 1 = 1
    */
    change_count();

    // *** LOCAL SCOPE of main() ends ***

    return 0;
}

// *** GLOBAL SCOPE ***
void change_count(void)
{
    // *** LOCAL SCOPE of change_count() begin ***

    // variable declarations
    // 'local_count' is a local variable. It is local to change_count() only.
    // It will retain its value between calls to change_count()
    static local_count = 0;

    // code
    local_count = local_count + 1;
    printf("Local Count = %d\n", local_count);

    // LOCAL SCOPE of change_count() ends ***
}

// *** GLOBAL SCOPE ***