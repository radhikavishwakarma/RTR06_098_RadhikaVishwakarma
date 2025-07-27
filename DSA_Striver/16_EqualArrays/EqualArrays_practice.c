#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    int arr1[] = {5, 8, 11, 2}; 
    int arr2[] = {5, 8, 15, 17};

    // 1. check if both the arrays have same numbers of elements
    int arr1_size = sizeof(arr1);
    int arr2_size = sizeof(arr2);

    // Function declaration
    bool checkArray(int arr1[], int arr2[], int, int);


    bool flag = checkArray(arr1, arr2, arr1_size, arr2_size);

    if (flag)
    {
        printf("Arrays are equal!!\n");
    }
    else
    {
        printf("Arrays are not equal!!\n");
    }
     
    

    return 0;
}

bool checkArray(int arr1[], int arr2[], int arr1_size, int arr2_size)
{
    int int_size = sizeof(int);

    int arr1_elements = arr1_size / int_size;
    int arr2_elements = arr2_size / int_size;

    // 2. if both Arrays have same number of elements then check each and every elements
    if (arr1_elements == arr2_elements)
    {
        // check and compare every elements
        for (int i = 0; i < arr1_elements; i++)
        {
            if (arr1[i] != arr2[i])
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}