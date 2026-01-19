#include <iostream>
#include <vector>

using namespace std;

class Chai
{
    public:
        string* teaName;
        int servings;
        vector<string> ingredients;

        // Parameter constructor
        Chai(string name, int serve, vector<string> ingr)
        {
            teaName = new string(name);
            servings = serve;
            ingredients = ingr;
            cout << " Param Constructor called" << endl;
        }

        Chai(Chai& other)
        {
            teaName = new string(*other.teaName);
            servings = other.servings;
            ingredients = other.ingredients;
            cout << "Copy constructor called" << endl;
        }

        ~Chai()
        {
            delete teaName;
            cout << "Destructor called" << endl;
        }
        
        void displayChaiDetails()
        {
            cout << "Tea Name: " << *teaName << endl;
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
        Chai lemonTea("Lemon Tea", 2, {"Water", "lemon", "Honey"});
        // lemonTea.displayChaiDetails();

        // Copy the object
        Chai copiedChai = lemonTea;
        // copiedChai.displayChaiDetails();

        *lemonTea.teaName = "Modified Lemon tea";

        cout << "Lemon Tea ----------" << endl;
        lemonTea.displayChaiDetails();
        cout << "Copied Tea ---------" << endl;
        copiedChai.displayChaiDetails();
        return 0;

    }

