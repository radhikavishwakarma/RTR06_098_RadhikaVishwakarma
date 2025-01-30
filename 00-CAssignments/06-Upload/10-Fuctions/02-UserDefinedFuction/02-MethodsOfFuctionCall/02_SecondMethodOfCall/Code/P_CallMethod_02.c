#include<stdio.h>

int main(int argc, char *argv[],char *envp[])
{
    //fuction prototypes
    void display_infromation(void);
    void Function_Country(void);

    //code
    display_information();
    Function_Country();

    return 0;
}

void display_information(void)
{
    void Function_My(void);
    void Function_Name(void);
    void Function_Is(void);
    void Function_FirstName(void);
    void Function_MiddleName(void);
    void Function_SurName(void);
    void Function_OfAMC(void);

    Function_My();
    Function_Name();
    Function_Is();
    Function_FirstName();
    Function_MiddleName();
    Function_Surname();
    Function_OFAMC();

}

void Function_My(void)
{
    printf("My");
}
void Function_Name(void)
{
    printf("Name");

}
void Function_Is(void)
{
    printf("Is");
}
void Function_FirstName(void)
{
    printf("Ganesh");
}
void Function_MiddleName(void)
{
    printf("Laxman");
}
void Function_SurName(void)
{
    printf("Gaikwad");
}
void Function_OfAMC(void)
{
    printf("OF ASTROMEDICOMP");
}
void Function_Country(void)
{
    printf("I Live In India ");
}