#include<stdio.h>  //'stdio.h' contains declaration of 'printf()'
// ***** User defined Functions : Method of calling function 2 *****
// Calling only TWO functions in main(), Rest of the functions trace their call indirectly to main *****

// Entry-Point Function => main() => Valid Return Type (int) and 3 Parameters (int argc, char *argv[], char *envp[])
int main(int argc, char * argv[], char * envp[])
{
    // function prototype / declarations / signature
    void display_information(void);
    void Function_Country(void);

    // code
    display_information(); // function call
    Function_Country(); // function call

    return 0;
}

// User defined function's definitions.... ***
void display_information(void)  // function definition
{
    // function prototypes
    void Function_My(void);
    void Function_Name(void);
    void Function_Is(void);
    void Function_FirstName(void);
    void Function_MiddleName(void);
    void Function_Surname(void);
    void Function_OfAMC(void);

    // *** Function Calls ***
    Function_My();
    Function_Name();
    Function_Is();
    Function_FirstName();
    Function_MiddleName();
    Function_Surname();
    Function_OfAMC();
}

void Function_My(void) // function definition
{
    // code
    printf("\n\n");
    printf("My");
}

void Function_Name(void) // function definition
{
    // code
    printf("\n\n");
    printf("Name");
}

void Function_Is(void) // function definition
{
    // code
    printf("\n\n");
    printf("Is");
}

void Function_FirstName(void) // function definition
{
    // code
    printf("\n\n");
    printf("Lokesh");
}

void Function_MiddleName(void) // function definition
{
    // code
    printf("\n\n");
    printf("Premshankar");
}

void Function_Surname(void) // function definition
{
    // code
    printf("\n\n");
    printf("Vishwakarma");
}

void Function_OfAMC(void) // function definition
{
    // code
    printf("\n\n");
    printf("Of AstroMediComp");
}

void Function_Country(void) // function definition
{
    // code
    printf("\n\n");
    printf("I Live in India.");
}