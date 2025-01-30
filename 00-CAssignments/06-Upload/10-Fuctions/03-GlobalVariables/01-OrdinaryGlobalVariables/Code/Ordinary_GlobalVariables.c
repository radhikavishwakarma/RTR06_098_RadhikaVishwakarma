#include<stdio.h>

//  *** GLOBAL SCOPE **************

//If Not initialized by us, global variables are initialized to their zero values (with respect to thier data types i.e. 0 for int ,0.0 fof float and doubel etc.by default)
//but stil for good programming dicipline ,we shall explicitly initialize our global variable with 0 

int global_count = 0;

int main(void)
{
    //function prototypes
    void change_count_one(void);
    void change_count_two(void);
    void change_count_theee(void);

    //code
    printf("\n");

    printf("main() : Value of global_count = %d \n",global_count);

    change_count_one();
    change_count_two();
    change_count_theee();

    printf("\n");
    return 0;

}

// *** GLOBAL SCOPE ****

void change_count_one(void)
{
    //code
    global_count = 100;
    printf("change_count_one( ): Value of global_count = %d \n",global_count);

}

// *****GLOBAL SCOPE *****

void change_count_two(void)
{
    //code 
    global_count = global_count + 1;
    printf("change_counttwo() : Value of global_count =  %d \n",global_count);
}

// ** GLOBAL SCOPR ****

void change_count_theee(void)
{
    //code 
    global_count = global_count + 1;
    printf("change_count_three() : Value of global_count : = %d \n",global_count);
}

// ****GLOBAL SCOPE *******