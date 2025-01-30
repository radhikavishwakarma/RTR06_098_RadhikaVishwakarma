#include<stdio.h> // stdio.h conatins declaration of printf()

//*******USER DEFINED FUCTIONS : MTHOD OF CALLING FUCTION 2 ***** */
///******CALLING ONLY TWO FUCTION DIRECTLY IN main() ,REST OF THE FUCTIONS TRACE THEIR CALL INDIRECTLY TO main()*******
int main(int argc, char *argv[],char *envp[])
{
    //fuction prototype
    void display_information(void);
    void Function_Country(void);

    //code 
    display_information();//fuction call
    Function_Country();// fuction call

    return 0;
}

// ***********************Usr Defined Fuction 'Definitions ...*******
void display_information(void)// fuction definition
{
    // fuction prototypes
    void Function_My(void);
    void Function_Name(void);
    void Function_Is(void);
    void Function_FirstName(void);
    void Function_MiddleName(void);
    void Function_SurName(void);
    void Function_OfAMC(void);
    
    // code 

    // *** FUNCTION CALLS ***
    Function_My();
    Function_Name();
    Function_Is();
    Function_FirstName();
    Function_MiddleName();
    Function_SurName();
    Function_OfAMC();
}

void Function_My(void)//fuction definition
{
    //code 
    printf("\n\n");

    printf("MY");
}

void Function_Name(void)// function definition
{
    //code
    printf("\n\n");

    printf("Name");
}

void Function_Is(void)//fuction definition
{
    //code
    printf("\n\n");

    printf("Is");
}

void Function_FirstName(void)
{
    printf("\n\n");

    printf("Ganesh");
}

void Function_MiddleName(void)
{
    printf("\n\n");

    printf("Laxman");
}

void Function_SurName(void)
{
    printf("\n\n");

    printf("Gaikwad");
}

void Function_OfAMC(void)// function definition
{
    printf("\n\n");

    printf("Of ASTOMEDICOMP");
}

void Function_Country(void)// function definition
{
    printf("\n\n");

    printf("I Live In India");

    printf("\n\n");
}


