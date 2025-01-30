#include<stdio.h>

int main(void)
{
    //variable declarations
    int num;
    int *ptr = NULL;
    int *copy_ptr = NULL;

    //code
    num = 5;
    ptr = &num ;

    printf("\n\n");
    printf("***Before copy_ptr = ptr *****\n\n");
    printf("    num         =%d\n\n",num);
    printf("    &num        =%p\n",&num);
    printf("    *(&num)     =%d\n",*(&num));
    printf("    ptr         =%  p\n",ptr);
    printf("    *ptr        =%d\n",*ptr);

    //* ptr is an intger pointer variable ..that it it can hold the addres of any intger variable onluy
    ///copy_ptr is another intger pointer variable
    // If ptr = &num...ptr will contain address of intger variable num
    //..if ptr is aassigned to copy_ptr,copy_ptr will also contains address of intger variable num
    //hence,not both,ptr and copy_ptr will point ti num...

    copy_ptr = ptr; // copy_ptr = ptr = &num

    printf("\n\n");
    printf("******After copy_ptr = ptr ****\n\n");
    printf("num=            =%d\n",num);
    printf("&num            =%p\n",&num);
    printf("*(&num)         =%d\n",*(&num));
    printf("ptr             =%p\n",ptr);
    printf("*ptr            =%d\n",*ptr);
    printf("copy_pt r       =%p\n",copy_ptr);
    printf("*copy_ptr        =%d\n",*copy_ptr);

    return 0;

}