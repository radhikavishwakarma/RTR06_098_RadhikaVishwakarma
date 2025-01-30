#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMN 3
#define DEPTH 2

int main(void)
{
    //varible declarations

    // In-LINE INITIALIZATION
    int iArray[NUM_ROWS][NUM_COLUMN][DEPTH] ={{{9,18},{27,36},{45,54}},
    {{8,16},{24,32},{40,48}},
    {{7,14},{21,28},{35,42}},
    {{6,12},{18,24},{30,36}},
    {{5,10},{15,20},{25,30}}

    };

    int ganii_i,ganii_j,ganii_k;

    int ganii_iArray_1D[NUM_ROWS *NUM_COLUMN *DEPTH];// 5*3*2 Elements = 30 elements In 1d array

    //code

    //***Display 3D Array **** */
    printf("\n\n");
    printf("Elements In The 3D Array :\n\n");
    for(ganii_i = 0;ganii_i < NUM_ROWS; ganii_i++)
    {
        printf("*********ROW %d ********\n",(ganii_i +1));
        for(ganii_j = 0; ganii_j < NUM_COLUMN; ganii_j++)
        {
            printf("*****COLUMN %d *******\n",(ganii_j +1));
            for(ganii_k = 0;ganii_k < DEPTH; ganii_k++)
            {
                printf("iArray[%d][%d][%d] = %d\n",ganii_i,ganii_j,ganii_k,iArray[ganii_i][ganii_j][ganii_k]);
            }
            printf("\n");
        }
        printf("\n");
    }

    // **** Converting 3d to 1 D ****
    for(ganii_i = 0; ganii_i < NUM_ROWS ; ganii_i++)
    {
        for(ganii_j =0; ganii_j < NUM_COLUMN; ganii_j++)
        {
            for(ganii_k = 0; ganii_k <DEPTH; ganii_k++)
            {
                ganii_iArray_1D[(ganii_i * NUM_COLUMN * DEPTH) + (ganii_j *DEPTH) +ganii_k] = iArray[ganii_i][ganii_j][ganii_k];
            }
        }
    }

    // ****Display 1D Array ***********
    printf("\n\n");
    printf("Elements In The 1D Array  : \n\n");
    for(ganii_i = 0;ganii_i <(NUM_ROWS *NUM_COLUMN *DEPTH); ganii_i++)
    {
        printf("iArray_1D[%d] = %d\n",ganii_i,ganii_iArray_1D[ganii_i]);
    }
    return 0;
}