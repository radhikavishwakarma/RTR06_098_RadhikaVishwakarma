#include<stdio.h>  //'stdio.h' contains declaration of 'printf()'
// ***** User defined Functions : Method of calling function 2 *****
// Calling only ONE functions in main(), Rest of the functions trace their call indirectly to main *****

// Entry-Point Function => main() => Valid Return Type (int) and 3 Parameters (int argc, char *argv[], char *envp[])
int main(int argc, char * argv[], char * envp[])
{
    // function prototype / declarations / signature
    void Function_Country(void);

    // code
    Function_Country(); // function call

    return 0;
}

void Function_Country(void) // function definition
{
    // function declarations
    void Function_OfAMC(void);

    // code
    Function_OfAMC();
    printf("\n\n");
    printf("I Live in India.");
}

void Function_OfAMC(void) // function definition
{
    // function declarations
    void Function_Surname(void);

    // code
    Function_Surname();
    printf("\n\n");
    printf("Of AstroMediComp");
}

void Function_Surname(void) // function definition
{
    // function declarations
    void Function_MiddleName(void);

    // code
    Function_MiddleName();
    printf("\n\n");
    printf("Vishwakarma");
}

void Function_MiddleName(void) // function definition
{
    // function declarations
    void Function_FirstName(void);

    // code
    Function_FirstName();
    printf("\n\n");
    printf("Premshankar");
}

void Function_FirstName(void) // function definition
{
    // function declarations
    void Function_Is(void);

    // code
    Function_Is();
    printf("\n\n");
    printf("Lokesh");
}

void Function_Is(void) // function definition
{
    // function declarations
    void Function_Name(void);

    // code
    Function_Name();
    printf("\n\n");
    printf("Is");
}

void Function_Name(void) // function definition
{
    // function declarations
    void Function_My(void);
    
    // code
    Function_My();
    printf("\n\n");
    printf("Name");
}

void Function_My(void) // function definition
{
    // code
    printf("\n\n");
    printf("My");
}