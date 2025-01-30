#include<stdio.h>

// *** GLOBAL SCOPR 

int main(void)
{
    //function prototypes
    void change_count(void);

    //variable declaration
    extern int global_count; 

    //code
    printf("\n\n");
    printf("Value of global_count before change_count() = %d \n",global_count); 
    change_count();
    printf("Vaue of global_count after change_count() = %d \n",global_count);
    printf("\n");

    return 0; 
}

// *****GLOBAL SCCPE *******8
//global_count is global variable.
//Since,it is declared before change_count() it can be accessed and used asany ordinary global variable in change_count()
//singce ,it is declared after main(),it must be first-redeclared in main() as as external global variable by means of 'extern' keyword and the type of the variable.
// Once this is done,it can be used as an ordinary global variable in main as weell

int global_count= 0;

void change_count(void)
{
    //code
    global_count = 5;
    printf("Value of global_count in change_count = %d \n",global_count);
}