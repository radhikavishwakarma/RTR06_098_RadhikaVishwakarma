#include <iostream>
using namespace std;

int main()
{
    char ch;

    cout << "Enter a symbol" << endl;
    cin >> ch;

    if ((ch >= 65) && (ch <= 90))
    {
        cout << "This is upper case" << endl;
    }

    else if ((ch >= 97) && (ch <= 122))
    {
        cout << "This is lower case" << endl;
    }

    else if ((ch >= 48) && (ch <= 57))
    {
        cout << "This is numeric number" << endl;
    }

    else
    {
        cout << "Invalid input" << endl;
    }

    return 0;
}

