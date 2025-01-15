#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    // function prototypes OR declarations
    int MyAddition(int, int);

    // variable declarations
    int r;
    int num_01, num_02;
    int num_03, num_04;

    // code
    num_01 = 10;
    num_02 = 20;
    num_03 = 30;
    num_04 = 40;

    r = MyAddition(MyAddition(num_01, num_02), MyAddition(num_03, num_04));

    return 0;
}

// *** Function Definition of MyAddition() ***
int MyAddition(int a, int b)
{
    // variable declaration
    int sum;

    // code
    sum = a + b;
    return(sum);
}