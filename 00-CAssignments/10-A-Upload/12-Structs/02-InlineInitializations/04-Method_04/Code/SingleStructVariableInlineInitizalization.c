#include<stdio.h>

//Defining Struct
struct MyData
{
    int ganii_i;
    float ganii_f;
    double ganii_d;
    char ganii_c;
};

int main(void)
{
    // variable declarations

    //35 will be assigned to i of data_one
    // 3.9 will be assigned to of data_two
    // 1.23243 willbe assigned to d of data_one
    // A will be assigned ti c of data_one
    struct MyData data_one = {35,3.9f,1.237565,'A'}; // Inline initializations

    // P will be assigned to i of data_two ..bute P is a character (char)
    // And I is an intger.. so P is converted unto it decirmal intger ASCII value (80)  and 80 assigned to i of data_two
    // 6.2 will be assigned to f of data_two
    //12.3433.35 will be assigned to d of data_two
    // 68 will be assigned to c of data_two ... 68 is an intger (int) and c is a char ... 68 is considered as decimal ASCII value annd its
    // correspoding character('D) is assigned to c data_two
    struct MyData data_two = {'P',6.2f,12.19999,68};// Inline Initializations

    // 36 will be assigned to i of data_three
    //G us char but d of data_three is float ..hence G s converted to ites decimal intger ASCII value (71) and thus
    //this is turn is converted to float (71.0000) and then it will be assigned to f of dat_three
    // 0.00000 will be assigned to 'data_three.
    struct MyData data_three = {36,'G'};//Inline Initialiations

    //79 will be assigned to i of data_four
    // 0.00000 will be assigned to f of data_four
    // 0.0000 will be assigned to d of data_four 
    // No character will be assigned to c of data_four 
    struct MyData data_four = {79};// Inline Initizalition

    //code    
    //Displaying values of the data Members of struct Mydat
    printf("\n\n");
    printf("Data memebrs of struct MDat dat_one are :\n\n");
    printf("i = %d\n",data_one.ganii_i);
    printf("f = %f\n",data_one.ganii_f);
    printf("d = %lf\n",data_one.ganii_d);
    printf("c= %c\n",data_one.ganii_c);

    printf("\n\n");
    printf("Data memebrs os struct MyDat dat_two Are :\n\n");
    printf("i = %d\n",data_two.ganii_i);
    printf("f = %f\n",data_two.ganii_f);
    printf("d = %lf\n",data_two.ganii_d);
    printf("c = %c\n",data_two.ganii_c);

    printf("\n\n");
    printf("Data memebers os struct Mydata data_threee Are :\n\n");
    printf("i = %d\n",data_three.ganii_i);
    printf("f = %f\n",data_three.ganii_f);
    printf("d = %lf\n",data_three.ganii_f);
    printf("c = %c\n",data_three.ganii_c);

    printf("\n\n");
    printf("Data members of struct MyData dat_four are : \n\n");
    printf("i = %d\n",data_four.ganii_i);
    printf("f = %f\n",data_four.ganii_f);
    printf("d = %ld\n",data_four.ganii_d);
    printf("c = %c\n",data_four.ganii_c);

    return 0;

}