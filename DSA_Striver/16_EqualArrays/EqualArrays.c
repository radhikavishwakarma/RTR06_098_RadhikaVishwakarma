#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    int arr1[] = {5, 8, 11, 2}; 
    int arr2[] = {5, 8, 11, 2};
    int arr1_size = sizeof(arr1);
    int arr2_size = sizeof(arr2);

    
    // function declaration
    bool checkArray(int arr1[], int arr2[], int, int);   

    if (checkArray(arr1, arr2, arr1_size, arr2_size))
    {
        printf("Arrays are equal\n");
    }
    else
    {
        printf("Arrays are not equal\n");
    }
    
    return 0;
}

bool checkArray(int arr1[], int arr2[], int arr1_size, int arr2_size)
{
    int int_size = sizeof(int);

    int arr1_elements = arr1_size / int_size;
    int arr2_elements = arr2_size / int_size;

    // printf("int size: %d\n", int_size);
    // printf("arr1 size: %d\n", arr1_size);
    // printf("arr2 size: %d\n", arr2_size);
    // printf("arr1 elements: %d\n", arr1_elements);
    // printf("arr2 elements: %d\n", arr2_elements);

    if (arr1_elements == arr2_elements)
    {
        // check all the elements of both arrays
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