#include<stdio.h>

enum{
        NEGATIVE  = -1 ,
        ZERO,
        POSITIVE
};

int main(void)
{
    //function declarations
    int Difference(int,int , int *);

    //variable declaratiosn
    int a;
    int b;
    int answer ,ret ;

    //code
    printf("\n\n");
    printf("Enter Value of 'A' : ");
    scanf("%d",&a);

    printf("\n\n");
    printf("Enter Value of 'B'  : ");
    scanf("%d",&b);

    ret = Difference(a,b,&answer);

    printf("\n\n");
    printf("Differnce of %d and %d = %d \n\n",a,b,answer);

    if(ret ==POSITIVE)
        printf("The Difference of %d and %d Is Posistive !!!|n\n",a,b);
    else if(ret == NEGATIVE)
        printf("Teh Difference of %d And %d is negative !!!\n\n",a,b);
    else
        printf("Teh Differnce of %d and %d Is Zero !!!\n\n",a,b);
    
    return 0;

}

/*We want our fucbtsion Difference() To Perifmr 2 Jobs...
Oen IS To subtract the input Numbers ('y' From 'x') And Teh Secod,Is to Tell
whether the Differnece of 'x' and 'y' is positive or negative or zero...a
// Buy any function has only valid return value,then how can we manage to return
two value to the calling fucntsion ?
This is where parameterized return value comes Into the pictuire...asm//we can return the actual differnce of 'x' and 'y'  That is, The actual answer
value Via out-paramaters /Parameterriuzed return value
// we can return the statatus of the answer (positive/negative/zero) via the actual return value of the function...asm


*/

int Difference(int x,int y,int  *diff)
{
    //code
    *diff = x - y;

    if(*diff > 0)
        return(POSITIVE);
    else if(*diff < 0)
        return(NEGATIVE);
    
    else
        return(ZERO);

}