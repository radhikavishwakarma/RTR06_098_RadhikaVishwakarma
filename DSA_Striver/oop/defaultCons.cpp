#include <iostream>
#include <vector>

using namespace std;

class Chai
{
    public:
        string teaName;
        int servings;
        vector<string> ingredients;

        // Default constructor
        Chai()
        {
            teaName = "Unknow Tea";
            servings = 1;
            ingredients = {"Water", "Tea leaves"};
            cout << "Constructor called" << endl;
        }
        
        void displayChaiDetails()
        {
            cout << "Tea Name: " << teaName << endl;
            cout << "servings: " << servings << endl;
            cout << "Ingredients: ";
            for (string ingredient : ingredients)
            {
                cout << ingredient << " ";
            }

            cout << endl;
            
        }

};

    int main()
    {
        Chai defaultChai;

        defaultChai.displayChaiDetails();
        return 0;

    }

