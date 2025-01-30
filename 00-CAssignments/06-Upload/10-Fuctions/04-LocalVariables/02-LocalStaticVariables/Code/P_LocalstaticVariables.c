#include<stdio.h>
#include<stdio.h>
#include<stdio.h>
#include<stdio.h>
#include<stdio.h>

int main(void)
{
    int a = 5;

    void change_count(void);
    void change_count(void);
    void change_count(void);
    void change_count(void);
    void change_count(void);

    printf("\n\n");
    printf("A= %d \n",a);
    printf("A = %d \n",a);
    printf("A = %d \n",a);
    printf("A = %d \n",a);
    printf("A=  %d \n",a);

    change_count();
    change_count();
    change_count();

    return 0 ;
}

void change_count(void)
{
    static int local_count = 0;
    local_count = local_count + 1;

    printf("Local Count= %d \n",local_count);

}
