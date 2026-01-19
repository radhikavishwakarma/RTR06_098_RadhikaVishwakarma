#include <iostream>

using namespace std;

class BankAccount {
    private:
        string accountNumber;
        double balance;
    
    public:
        BankAccount(string accNum, double initialBalance){
            accountNumber = accNum;
            balance = initialBalance;
        }

    // Getter
    double getBalance() const{
        return balance;
    }

    // Method to deposit money
    void deposit(double amount){
        if (amount > 0)
        {
            balance += amount;
            cout << "Deposited: " << amount << endl;
        }
        else
        {
            cout << "Invalid deposit amount";
        } 
    }

    void withdraw(double amount){
        if (amount > 0 && amount <= balance)
        {
            balance -= amount;
            cout << "Withdraw: " << amount << endl;
        }
        else
        {
            cout << "Invalid withdraw amount";
        }  
    }
};

int main(){
    BankAccount myAccount("15466532", 2000);

    myAccount.getBalance();

    myAccount.deposit(400);
    myAccount.withdraw(200);

    return 0;
}