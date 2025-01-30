#include<stdio.h>

//DEFINING STRUCT
struct MyData
{
    int i ;
    float f ;
    double d;
    char c;

};

int main(void)
{
    //function prototype
    struct MyData AddStructMembers(struct MyData,struct MyData,struct MyData);
    
    // variable declarations
    struct MyData data1,data2,data3,answer_data;

    // code
    // **data 1 ***
    printf("\n\n");
    printf("**********Data 1 **********\n\n");
    printf("Ente Intger Value For i of struct MyData data1 : ");
    scanf("%d",&data1.i);

    printf("\n\n");
    printf("ENter Floating-Point Value For f os truct MyData data1 :");
    scanf("%f",&data1.f);

    printf("\n\n");
    printf("Enter Double Vaue For d of struct MyData data1 : ");
    scanf("%lf",&data1.d);

    printf("\n\n");
    printf("Ente Character Value For c of Struct MyData data1 : ");
    data1.c = getch();
    printf("%c ",data1.c);

    //*** Data 2  */
    printf("\n\n");
    printf("************Data 2***********\n\n");
    printf("Enter Intger Value For i fo struct MyData dat2 :");
    scanf("%d",&data2.i);

    printf("\n\n");
    printf("Enter Floating-Point Value For f of struct MyData data2 : ");
    scanf("%f",&data2.f);

    printf("\n\n");
    printf("Enter double Value For d of struct MyData dat2 : ");
    scanf("%lf",&data2.d);

    printf("\n\n");
    printf("Enter Character Value For c of Struct MYdata data2 :");
    data2.c = getch();
    printf("%c",data2.c);

    // ********data 3**********
    printf("\n\n");
    printf("*************Data 3***************\n\n");
    printf("Enter Intger Value For i of struct MyData dat3 : ");
    scanf("%d",&data3.i);

    printf("\n\n");
    printf("Enter Floating value for f of struct Mydata data3 :");
    scanf("%f",&data3.f);

    printf("\n\n");
    printf(" Enter Double Value For d of struct MyData data3 : ");
    scanf("%lf",&data3.d);

    printf("\n\n");
    printf("Enter Character Value For c of struct MyData data3 :");
    data3.c = getch();
    printf("%c",data3.c);

    // ****CALLING FUCNTION AddStructMembers() WHICH ACCEPTS THREEE VARIABLES OF TYPE 'struct MyData As Parameters And ADDS UP THE RESPECTIVE MEMBERS AND RETURND THE
    // ANSWER IN ANOTHER struct of Same tyep ***
    answer_data = AddStructMembers(data1,data2,data3);

    printf("\n\n");
    printf("*****ANSWER************\n\n");
    printf("answer_data.i = %d\n",answer_data.i);
    printf("answer_dat.f = %f\n",answer_data.f);
    printf("answer_data.d = %lf\n\n",answer_data.d);

    answer_data.c = data1.c ;
    printf("answer_data.c (Fromd data1) = %c\n\n",answer_data.c);

    answer_data.c = data2.c ;
    printf("answer.data.c (from data2) = %c \n\n",answer_data.c);

    answer_data.c = data3.c ;
    printf("answer_data.c (from data3) = %c \n\n",answer_data.c);
    return 0;

}

struct MyData AddStructMembers(struct MyData md_one,struct MyData md_two,struct MyData md_three)
{
    //variable declarations
    struct MyData answer;

    //code
    answer. i = md_one.i +md_two.i +md_three.i;
    answer.f = md_one.f +md_three.f +md_two.f;
    answer.d = md_one.d +md_two.d +md_three.d ;
    return answer;
}