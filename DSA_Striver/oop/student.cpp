#include <iostream>
#include <string.h>

using namespace std;

class Student

{
    public:
        // Data Members
        int rollNumber = 0;
        string name = "";
        char grade;

        // Member Functions
        void getRollNumber()
        {
            cout << "Roll No: " << rollNumber << endl;
        }

        void setRollNumber(int newRollNumber)
        {
            rollNumber = newRollNumber;
        }

        void getName()
        {
            cout << "Name: " << name << endl;
        }

        void setName(string newName)
        {
            name = newName;
        }

        void getGrade()
        {
            cout << "Grade: " << grade << endl;
        }

        void setGrade(char newGrade)
        {
            grade = newGrade;
        }
};

class  Teacher
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
    // --------------STUDENT 1------------
    Student s1;
    s1.rollNumber = 20; // set attribute value directly
    s1.getRollNumber();
    s1.name = "Radhika"; // set attribute value directly
    s1.getName();
    s1.grade = 'A'; // set attribute value directly
    s1.getGrade();

    // // --------------STUDENT 2------------
    // Student s2;
    // s2.setRollNumber(50); // set attribute value via setter function
    // s2.getRollNumber();
    // s2.setName("Lokesh"); // set attribute value via setter function
    // s2.getName();
    // s2.setGrade('B'); // set attribute value via setter function
    // s2.getGrade();

    // // -------------- Teacher1 --------------
    // Teacher t1;
    // t1.name = "Radhika";
    // t1.getName();
    // t1.id = 20;
    // t1.getId();
    // t1.sub = "English";
    // t1.getSub();

    // // -------------- Teacher2 --------------
    // Teacher t2;
    // t2.setName("Lokesh");
    // t2.getName();
    // t2.setId(25);
    // t2.getId();
    // t2.setSub("Maths");
    // t2.getSub();

    return 0;
}