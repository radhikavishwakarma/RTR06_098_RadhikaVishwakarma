
#include <stdio.h>

int main(void)
{
    //code
    printf("\n\n");

    printf("Size of 'int'           = %1d bytes\n", sizeof(int));
    printf("Size of 'unsigned int'  = %1d bytes\n", sizeof(unsigned int));
    printf("Size of 'long'          = %1d bytes\n", sizeof(long));
    printf("Size of 'long long'     = %1d bytes\n", sizeof(long long));

    printf("Size of 'float'         = %1d bytes\n", sizeof(float));
    printf("Size of 'double'        = %1d bytes\n", sizeof(double));
    printf("Size of 'long double'   = %1d bytes\n", sizeof(long double));

    printf("\n\n");

    return(0);

}
