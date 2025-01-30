#include<stdio.h>
#include<conio.h>

struct Mydata
{
    int ganii_i;
    float ganii_f;
    double ganii_d;
    char ganii_ch;
};

int main(void)
{
    //variablel declarations
    struct Mydata data;// declaring a single struct variable

    // code
    //User Input For Valuea of Data Members of struct Mydat
    printf("\n\n");

    printf("Enter Intger Value For Data Member 'i' of struct MyData :\n");
    scanf("%d",&data.ganii_i);

    printf("Enter Floating_point BValue For Data member of f of struct MyData :\n");
    scanf("%lf",&data.ganii_f);

    printf("ENter double Vlaue for data memmber d of struct myDta :\n");
    scanf("%lf",&data.ganii_d);

    printf("Enter Character Value For Data member of c of struct MyDat :\n");
    data.ganii_ch = getch();

    printf("\n\n");
    printf("Data Member of Struct Mydata Are :\n\n");
    printf("i = %d\n",data.ganii_i);
    printf("f = %f\n",data.ganii_f);
    printf("d = %lf\n",data.ganii_d);
            printf("c = %c\n",data.ganii_ch);

            return 0;

}