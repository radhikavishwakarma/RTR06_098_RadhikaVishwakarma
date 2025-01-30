#include<stdio.h>

// *** GLOBAL SCOPE  ******

int main(void)
{
    // *** LOCAL SCOPR OF main() begins ****

    // variable declaration
    // 'a' is local Variable,It is local to main() only 
    int a= 5;

    //function prototypes
    void change_count(void);

    //code 
    printf("\n");
    printf("A = %d \n\n",a);

    //local_count is initialized to 0 
    //local_count = local_count + 1 = 0 + 1 = 1
    change_count();

    //Sunce,'local_count' is as ordinary local variable of change_count(),it
    //will NOT retain its value from previous call to change_count().
    //son local_count = local_count + 1 = 1 + 0
    change_count();

    //Since,'local_count' is an ordinary local variable of change_count(),it
    //will not retain iys value from previous call to chhnag_count().
    //So local_count is AGAIN initialized to 0
    //local_count = local_count _1 =  0 + 1 = 1 
    change_count();

    return 0;

    // **LOCAL SCOPE OF  main() ens ****
}

// **GLOBAL SCOPR  *****

void change_count(void)
{
    // *** LOCAL SCOPE OF change_count() begins 

    //variable declarations
    // 'local_count; is alocal Variable It is local to change_count() obly
    int local_count =0;

    //code
    local_count = local_count + 1;

    printf("Local COunt = %d \n",local_count);

    // ****LOCAL SCCOPR OF change_count() ends ****

}
 /// *******GLOBAL SCOPE 