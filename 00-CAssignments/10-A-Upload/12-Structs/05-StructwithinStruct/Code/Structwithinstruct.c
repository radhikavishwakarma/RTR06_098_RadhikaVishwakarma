#include<stdio.h>

struct MyNumber
{
    int ganii_num;
    int ganii_num_table[10];
};

struct NumTables
{
    struct MyNumber a;
    struct MyNumber b;
    struct MyNumber c;
};

int main(void)
{
    //variable declarations
    struct NumTables tables;
    int ganii_i ;

    //code
    tables.a.ganii_num = 2;
    for(ganii_i = 0; ganii_i < 10; ganii_i++)
        tables.a.ganii_num_table[ganii_i] = tables.a.ganii_num *(ganii_i +1);
    printf("\n\n");
    printf("Table Of %d : \n\n",tables.a.ganii_num);
    for(ganii_i = 0; ganii_i < 10; ganii_i++)
        printf("%d * %d = %d \n",tables.a.ganii_num,(ganii_i +1),tables.a.ganii_num_table[ganii_i]);
    
    tables.b.ganii_num = 3;
    for(ganii_i = 0; ganii_i < 10; ganii_i++)
        tables.b.ganii_num_table[ganii_i] = tables.b.ganii_num * (ganii_i +1);
    printf("\n\n");
    printf("Table of d : \n\n",tables.b.ganii_num);
    for(ganii_i = 0; ganii_i < 10; ganii_i++)
        printf("%d * %d = %d\n",tables.b.ganii_num,(ganii_i +1),tables.b.ganii_num_table[ganii_i]);
    
    tables.c.ganii_num = 4;
    for(ganii_i = 0; ganii_i < 10; ganii_i++)
        tables.c.ganii_num_table[ganii_i] = tables.c.ganii_num *(ganii_i+1);
    printf("\n\n");
    printf("Table of %d : \n\n",tables.c.ganii_num);
    for(ganii_i =0; ganii_i < 10;ganii_i++)
        printf("%d & %d= %d\n",tables.c.ganii_num,(ganii_i +1),tables.c.ganii_num_table[ganii_i]);

    return 0;


}