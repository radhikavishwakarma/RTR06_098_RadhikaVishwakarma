#include<stdio.h>

int main(void)
{
    //defining struct
    struct MyData
    {
        
        int ganii_i;
        float ganii_f;
        double ganii_d;
        char ganii_c;
    }data = {5,9.1f,3.44545,'N'};

    // Displaying Vaues of the data memebrs os struct MyData
    printf("\n\n");
    printf("Data members of struct MyData are :\n\n");
    printf("i = %d\n",data.ganii_i);
    printf("f = %f\n",data.ganii_f);
    printf("d = %ld\n",data.ganii_d);
    printf("c = %c\n",data.ganii_c);
    return 0;
}