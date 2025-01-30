#include<stdio.h>

struct MyNumber
{
    int ganii_num;
    int ganii_num_table[10];
};

struct NumTables
{
    struct MyNumber n;
};

int main(void)
{
    //variable declarations
    struct NumTables tables[10]; // an array of  10 'struct NumbTables'
    int ganii_i,ganii_j;

    // code
    for(ganii_i = 0; ganii_i < 10;ganii_i++)
    {
        tables[ganii_i].n.ganii_num = (ganii_i +1);
    }
    for(ganii_i = 0; ganii_i <10; ganii_i++)
    {
        printf("\n\n");
        printf("Table Of %d : \n\n",tables[ganii_i].n.ganii_num);
        for(ganii_j =0; ganii_j < 10; ganii_j++)
        {
            tables[ganii_i].n.ganii_num_table[ganii_j] = tables[ganii_i].n.ganii_num * (ganii_j +1);
            printf("%d * %d = %d \n",tables[ganii_i].n.ganii_num,(ganii_j +1),tables[ganii_i].n.ganii_num_table[ganii_j]);
        }   
        
    }
}