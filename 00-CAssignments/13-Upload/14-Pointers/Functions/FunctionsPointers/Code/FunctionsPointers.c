#include<stdio.h>

int main(void)
{
    //functions delcarations
    int AddIntgers(int,int);
    int SubtractIntgers(int,int);
    float AddFloats(float,float);

    //variable declarations
    typedef int (*AddIntsFnPtr)(int ,int);
    AddIntsFnPtr ptrAddTwoIntgeres = NULL;
    AddIntsFnPtr ptrFunc = NULL;

    typedef float (*AddFloatsFnPtr)(float,float);
    AddFloatsFnPtr ptrAddTwoFloats = NULL;

    int iAnswer = 0;
    float fAnswer = 0;

    //code
    ptrAddTwoIntgeres = AddIntgers;
    iAnswer = ptrAddTwoIntgeres(9,30);
    printf("\n\n");
    printf("Sum of Intgers = %d \n\n",iAnswer);

    ptrFunc = SubtractIntgers;
    iAnswer = ptrFunc(9,30);
    printf("\n\n");
    printf("Subtraction of iNtgers = %d \n\n",iAnswer);

    ptrAddTwoFloats = AddFloats;
    fAnswer = ptrAddTwoFloats(11.45f,8.2f);
    printf("\n\n");
    printf("Sum of Floating-point Number = %f \n\n",fAnswer);

    return 0;

}

int AddIntgers(int a,int b)
{
    //variablde declarations
    int c;
    
    //code
    c = a + b;
    return c;
}

int SubtractIntgers(int a,int b)
{
    //variable declarations
    int c ;

    //code
    if(a > b)
        c = a - b;
    else    
        c = b - a ;
    return c;
}

float AddFloats(float f_num1,float f_num2)
{
    //variable declarations
    float ans;
    
    //code
    ans = f_num1 + f_num2;

    return  ans;
}