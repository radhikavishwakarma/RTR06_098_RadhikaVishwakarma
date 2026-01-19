#include <iostream>
#include <string.h>

using namespace std;

class  teacher
{
    public:
        // Data members
        string name;
        int id = 0;
        string sub;

        // Members Functions
        void getName()
        {
            cout << "Name: " << name << endl;
        }

        void setName(string newName)
        {
            name = newName;
        }

        void getId()
        {
            cout << "Id: " << id << endl;
        }

        void setId(int newId)
        {
            id = newId;
        }

        void getSub()
        {
            cout << "Sub: " << sub << endl;
        }

        void setSub(string newSub)
        {
            sub = newSub;
        }


};

int main()
{
    // -------------- Teacher1 --------------
    teacher t1;
    t1.name = "Radhika";
    t1.getName();
    t1.id = 20;
    t1.getId();
    t1.sub = "English";
    t1.getSub();

    // -------------- Teacher2 --------------
    teacher t2;
    t2.setName("Lokesh");
    t2.getName();
    t2.setId(25);
    t2.getId();
    t2.setSub("Maths");
    t2.getSub();



    return 0;
}    