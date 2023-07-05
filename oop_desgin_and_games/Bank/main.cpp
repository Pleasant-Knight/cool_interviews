#include "Bank.h"

// Forward decleration for teh << operators:
ofstream& operator << (ofstream& ofs, Account& acc);
ifstream& operator >> (ifstream& ifs, Account& acc);
ostream& operator << (ostream& os, Account& acc);

int main() {
    Bank b;
    Account acc;
    int choice;
    string fname, lname;
    long accountNumber;
    float balance;
    float amount;

    cout << "*** Welcome to Our Banking System ***" << "\n";
    do     {
        cout << "\n\tSelect one option below:";
        cout << "\n\t1 Open an Account";
        cout << "\n\t2 Balance Enquiry";
        cout << "\n\t3 Deposit";
        cout << "\n\t4 Withdrawal";
        cout << "\n\t5 Close an Account";
        cout << "\n\t6 Show All Accounts";
        cout << "\n\t7 Quit";
        cout << "\nEnter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "Enter First Name: ";
            cin >> fname;
            cout << "Enter Last Name: ";
            cin >> lname;
            cout << "Enter initil Balance: ";
            cin >> balance;
            acc = b.OpenAccount(fname, lname, balance);
            cout << "\nCongradulation Account is Created" << "\n";
            cout << acc;
            break;

        case 2:
            cout << "Enter Account Number:";
            cin >> accountNumber;
            acc = b.BalanceEnquiry(accountNumber);
            cout << "\nYour Account Details:" << "\n";
            cout << acc;
            break;

        case 3:
            cout << "Enter Account Number:";
            cin >> accountNumber;
            cout << "Enter Balance:";
            cin >> amount;
            acc = b.Deposit(accountNumber, amount);
            cout << "\n" << "Amount is Deposited" << "\n";
            cout << acc;
            break;

        case 4:
            cout << "Enter Account Number:";
            cin >> accountNumber;
            cout << "Enter Balance:";
            cin >> amount;
            acc = b.Withdraw(accountNumber, amount);
            cout << "\n" << "Amount Withdrawn" << "\n";
            cout << acc;
            break;

        case 5:
            cout << "Enter Account Number:";
            cin >> accountNumber;
            b.CloseAccount(accountNumber);
            cout << "\n" << "Account is Closed" << "\n";
            cout << acc;

        case 6:
            b.ShowAllAccounts();
            break;

        case 7:
            break;

        default:
            cout << "\nEnter corret choice";
            exit(0);
        }
    } while (choice != 7);

    return 0;
}


ofstream& operator << (ofstream& ofs, Account& acc) {
    ofs << acc.accountNumber << "\n";
    ofs << acc.firstName << "\n";
    ofs << acc.lastName << "\n";
    ofs << acc.balance << "\n";
    return ofs;
}

ifstream& operator >> (ifstream& ifs, Account& acc) {
    ifs >> acc.accountNumber;
    ifs >> acc.firstName;
    ifs >> acc.lastName;
    ifs >> acc.balance;
    return ifs;
}

ostream& operator << (ostream& os, Account& acc)
{
    os << "First Name:" << acc.getFirstName() << "\n";
    os << "Last Name:" << acc.getLastName() << "\n";
    os << "Account Number:" << acc.getAccNo() << "\n";
    os << "Balance:" << acc.getBalance() << "\n";
    return os;
}
