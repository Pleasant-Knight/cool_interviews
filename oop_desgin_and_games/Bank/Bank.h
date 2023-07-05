#pragma once

#ifndef _BANK_H_
#define _BANK_H_

#include "Account.h"


class Bank
{
private:
    map <long, Account> accounts;

public:
    Bank();
    ~Bank();

    Account OpenAccount(string fname, string lname, float balance);
    Account BalanceEnquiry(long accountNumber);
    Account Deposit(long accountNumber, float amount);
    Account Withdraw(long accountNumber, float amount);
    
    void CloseAccount(long accountNumber);
    void ShowAllAccounts();    
};

#endif // _BANK_H_
