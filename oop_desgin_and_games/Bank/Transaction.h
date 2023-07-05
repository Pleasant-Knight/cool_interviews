#pragma once

#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include <cstdint>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctime>

#include <map>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <limits>
#include <vector>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <mutex>
#include <utility>

using namespace std;

struct account_balance {
    uint64_t account_id; ///< the name of the account
    uint64_t balance; ///< the balance of the account
};

struct transfer {
    uint64_t  from;    ///< the account to transfer from
    uint64_t  to;      ///< the account to transfer to
    uint64_t  amount;  ///< the amount to transfer
};

using transaction = vector<transfer>;

/** This is going to be timestamp data type for our db. */
using dbTimeStamp = size_t;

/** Transaction type with a timestamp for ordering. */
struct TRX {
    transaction t;
    dbTimeStamp ts;
};
/**
 *
 * The transcational DB class (type).
 */
class TransXDB {
 private:
    /** The pending transaction queue. */
    deque<TRX> trx;

    /** DB schema: Key = Account ID, Val = Balance. */
    unordered_map<int /*acct_id*/, int/*balance*/> accounts;

    /** Commited transactions so far. */
    vector<dbTimeStamp> cmTrx;

    /** Read and write mutex for the entire DB table. */
    std::mutex mxDB;
    /** Read and write mutex for the transaction table. */
    std::mutex mxTrx;

    /** Last attempted (commited or rolled-back) transaction timestamp. */
    dbTimeStamp lastTrxTs;

public:
    // Copy constructor: more like intilization list constructor
    TransXDB(const vector<account_balance>& initial_balances);
    
    // The move constructor.
    TransXDB(TransXDB&& other)  noexcept;

    /** Static timestamp to keep track of each transaction order. */
    static dbTimeStamp ts;

    void push_transaction(const transaction& t);
    vector<account_balance> get_balances() const;
    
    /**
     * Return the 0-based indices of the commited (successful)
     * transactions to the database so far.
     */
    vector<size_t> get_applied_transactions() const {
        return this->cmTrx;
    }
    /** Attempt to do all the current pending transactions. */
    void settle();
};

#endif // _TRANSACTION_H_
