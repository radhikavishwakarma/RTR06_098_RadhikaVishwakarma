#include <iostream>
#include <string>
#include <vector>

using namespace std;

// base class
class Tea{
    protected:
        string teaName;
        int servings;
    public:
        Tea(string name, int serve): teaName(name), servings(serve){
            cout << "Tea constructor called" << teaName << endl;
        }

        virtual void brew() const {
            cout << "Brewing" << teaName << "with generic method" << endl;

        }

        virtual void serve() const {
            cout << "Serve" << serve << " Cup of tea with generic method" << endl;
            
        }

        virtual ~Tea(){
            cout << "Tea destructor called for" << teaName << endl;
        }

};

class GreenTea: public Tea {
    public:
        GreenTea
}
