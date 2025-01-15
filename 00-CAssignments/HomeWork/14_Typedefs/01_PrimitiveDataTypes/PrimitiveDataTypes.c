#include <stdio.h>

// Global typedef declarations
typedef int MY_INT; // "type" int has been re"def"ined as MY_INT
typedef float PVG_FLOAT; // "type" float has been re"def"ined as PVG_FLOAT
typedef char CHARACTER; // "type" char has been re"def"ined as CHARACTER
typedef double MY_DOUBLE; // "type" double has been re"def"ined as MY_DOUBLE

// Typedef for HANDLE-like usage
typedef unsigned int UINT;
typedef UINT HANDLE;
typedef HANDLE HWND;
typedef HANDLE HINSTANCE;

// Function prototype
MY_INT Add(MY_INT, MY_INT);

int main(void) {
    // Variable Declarations
    MY_INT a = 10, i;
    MY_INT iArray[] = { 9, 18, 27, 36, 45, 54, 63, 72, 81, 90 };

    PVG_FLOAT f_avg = 30.9f;
    const PVG_FLOAT f_pvg_pi = 3.14f;

    CHARACTER ch = '*';
    CHARACTER chArray_01[] = "Hello";
    CHARACTER chArray_02[][10] = { "RTR", "Batch", "2020-2021" };

    MY_DOUBLE d = 8.041997;

    UINT uint = 3456;
    HANDLE handle = 987;
    HWND hwnd = 9876;
    HINSTANCE hInstance = 14466;

    // Code
    printf("\n\n");
    printf("Type MY_INT Variable a = %d\n", a);

    printf("\n\n");
    for (i = 0; i < (sizeof(iArray) / sizeof(int)); i++) {
        printf("Type MY_INT array variable iArray[%d] = %d\n", i, iArray[i]);
    }

    printf("\n\n");
    printf("Type PVG_FLOAT Variable f_avg = %f\n", f_avg);
    printf("Type PVG_FLOAT Constant f_pvg_pi = %f\n", f_pvg_pi);

    printf("\n\n");
    printf("Type MY_DOUBLE Variable d = %lf\n", d);

    printf("\n\n");
    printf("Type CHARACTER Variable ch = %c\n", ch);

    printf("\n\n");
    printf("Type CHARACTER Array Variable chArray_01 = %s\n", chArray_01);

    printf("\n\n");
    for (i = 0; i < (sizeof(chArray_02) / sizeof(chArray_02[0])); i++) {
        printf("%s\t", chArray_02[i]);
    }
    printf("\n\n");

    printf("\n\n");
    printf("Type UINT variable uint = %u\n", uint);
    printf("Type HANDLE variable handle = %u\n", handle);
    printf("Type HWND variable hwnd = %u\n", hwnd);
    printf("Type HINSTANCE variable hInstance = %u\n", hInstance);
    printf("\n\n");

    MY_INT x = 90;
    MY_INT y = 30;
    MY_INT ret;

    ret = Add(x, y);
    printf("Result of Add(x, y): %d\n\n", ret);

    return (0);
}

// Function Definition
MY_INT Add(MY_INT a, MY_INT b) {
    MY_INT c = a + b;
    return c;
}
