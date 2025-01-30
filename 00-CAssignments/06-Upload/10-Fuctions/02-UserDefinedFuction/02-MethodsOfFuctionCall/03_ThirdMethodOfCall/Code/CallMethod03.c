#include<stdio.h>// 'stdio.h' contains declarations of printf()

// *********** USER DEFINED FUNCTIONS : METHOD OF CALLING FUNCTION 3 *******88
// *********8CALLING ONLY ONE FUNCTION DIRECTLY IN main() ,REST OF THE FUNCTIONS TRACE THEIR CALL INDIRECTLY TO  main()

int main(int agrc, char *argv[],char *envp[])
{
    //function prototypes
    void Function_Country();

    //code
    Function_Country();
    return 0;
}

void Function_Country(void)//function definition
{
    //function declaration
    void Function_OfAMC();

    //code
    Function_OfAMC();
    printf("\n\n");

    printf("I live In India");

    printf("\n\n");

}

void Function_OfAMC(void)//function definition
{
    //function declaration
    void Function_Surname(void);

    //code
    Function_Surname();

    printf("\n\n");

    printf("Of ASTROMEDICOMP");
}

void Function_Surname(void)//function definition
{
    //function declaration
    void Function_MiddleName(void);

    //code 
    Function_MiddleName();

    printf("\n\n");

    printf("Gaikwad");
}

void Function_MiddleName(void)//function definiion
{
    //function declaration
    void Function_FirstName(void);

    //code
    Function_FirstName();

    printf("\n\n");

    printf("Laxman");
}

void Function_FirstName(void)//function definition
{
    //function declaration
    void Function_Is(void);

    //code
    Function_Is();

    printf("\n\n");

    printf("Ganesh");
}
void Function_Is(void)//function definition
{
    //function declaration
    void Function_Name(void);

    //code
    Function_Name();

    printf("\n\n");

    printf("Is");

}

void Function_Name(void)//function definition
{
    //function declaration
    void Function_My(void);

    //code 
    Function_My();

    printf("\n\n");

    printf("Name");
}

//********User-Defined Function'Definition... */
void Function_My(void)// function definition
{
    //code 
    printf("\n\n");

    printf("My");
}