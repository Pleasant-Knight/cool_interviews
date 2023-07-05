#pragma once

#ifndef _ACCOUNT_H
#define _ACCOUNT_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

#define MIN_BALANCE 500

class InsufficientFunds {
};

class Account
{
public:
    Account() { }
    ~Account() { };
    Account(string fname, string lname, float balance);

    long getAccNo() {
        return accountNumber;
    }

    string getFirstName() {
        return firstName;
    }
    
    string getLastName() {
        return lastName;
    }

    float getBalance() {
        return balance;
    }
    
    void Deposit(float amount);
    void Withdraw(float amount);
    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();
    
    // stream << operator functions.
    friend ofstream& operator << (ofstream& ofs, Account& acc);
    friend ifstream& operator >> (ifstream& ifs, Account& acc);
    friend ostream& operator << (ostream& os, Account& acc);

private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    static long NextAccountNumber;
};


#endif // _ACCOUNT_H
