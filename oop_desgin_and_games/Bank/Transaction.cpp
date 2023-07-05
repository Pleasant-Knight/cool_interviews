#include "Transaction.h"

// Intializing the database timestamp.
dbTimeStamp TransXDB::ts = 0;


TransXDB::TransXDB(const vector<account_balance>& initial_balances) {
    // Lock the DB while intializing it.
    lock_guard<std::mutex> lock(this->mxDB);
    unordered_map<int, int>& accts = this->accounts;

    // Liquidate the balances into the DB (hash) table.
    for (const auto& b : initial_balances) {
        accts[b.account_id] = b.balance;
    }
}

TransXDB::TransXDB(TransXDB&& other)  noexcept :
    trx(std::move(other.trx)),
    accounts(std::move(other.accounts)),
    cmTrx(std::move(other.cmTrx)),
    /** Move of a member of non-class type */
    lastTrxTs(std::exchange(other.lastTrxTs, 0)) {
}


/** Add transaction 't' to our trx queue. */
void TransXDB::push_transaction(const transaction& t) {
    // Lock the transaction queue.
    lock_guard<std::mutex> lock(this->mxTrx);
    // Add t to the transactions queue:
    this->trx.push_back({ t, this->ts++ });
}

/**
 *  Return a listing of all the balances in any order.
 */

vector<account_balance> TransXDB::get_balances() const {
    vector<account_balance> balances;

    if (this->accounts.empty())
        return balances;

    for (const auto it : this->accounts) {
        balances.push_back({ it.first, it.second });
    }

    return balances;
}


/**
 * Try to execute all the transactions in the pending trx queue.
 * Note: we do not need to have undo/redo logs to keep our database
 * consistent. We use a small hash table as a cache for all the accounts
 * involoved in each transaction (assuming multiple transfers per
 * transaction), then execute the transaction as a whole unit on the
 * cache. If one part (transfer) lead to negative balance, we abort
 * the current transaction, recording only its timestamp.
 * Successfull transaction will be commited back to the database
 * main (hash) table (i.e., this->accounts). Also we add its timestamp
 * to the committed transaction vector.
 */

void TransXDB::settle() {
    // Lock the DB while executing the transactions. 
    lock_guard<std::mutex> dbLock(this->mxDB);
    lock_guard<std::mutex> trxLock(this->mxTrx);
    /**
     * For each transaction, populate a small hashtable of balances
     * from the DB main table (balances).
     */
    for (auto const& tr : this->trx) {
        // Small hash table to execute each transaction:
        // key --> acct_id, val --> balance
        unordered_map<int, int> ht;

        //TODO: check to see if the current transaction is in order or not
        /*if (tr.ts != (this->lastTrxTs + 1))
            assert(); */

            // Record this transaction as the last one
        this->lastTrxTs = tr.ts;
        // Copy each from, to pair balances to HT:
        for (auto const& v : tr.t) {
            ht[v.from] = this->accounts[v.from];
            ht[v.to] = this->accounts[v.to];
        }
        // Try to eexecute each transfer:
        for (auto const& v : tr.t) {
            ht[v.from] -= v.amount;
            // If the from account does not have enough, we skip this transaction
            if (ht[v.from] <= 0)
                break;
            ht[v.to] += v.amount;
        }
        // Now record all the transfers back to the DB:
        for (const auto& it : ht) {
            this->accounts[it.first] = it.second;
        }

        // Add this transaction to successful ones
        this->cmTrx.push_back(tr.ts);
    }

    return;
}

/**
 *
 * @param initial_balances - the initial balances in the database, see the above datastructures.
 * @return an instantiated database type of your creation which has the following member methods:
 *
 *    // push a transaction to the database
 *    //
 *    void push_transaction( const transaction&t )
 *
 *    // settle the database such that the invariant is maintained and the best state of the database is present
 *    //
 *    void settle()
 *
 *    // return a listing of all the balances in any order
 *    //
 *    vector<account_balance> get_balances() const
 *
 *    // Given the logical sequence of transactions constructed by the ordered sequence of calls
 *    // to push_transaction AND having called settle()
 *    //
 *    // return the 0-based indices of the surviving transactions in that sequence which, when
 *    // applied to the initial balances produce a state where the invariant is maintained
 *    //
 *    vector<size_t> get_applied_transactions() const
 *
 * The automated test cases will call create_database with a set of initial balances
 * They will then feed a number of transactions to your class
 * They will then call settle
 * They will then call get_applied_transactions and inspect the results
 * They will then call get_balances and inspect the results
 */
TransXDB&& create_database(const vector<account_balance>& initial_balances) {
    /** The instance of the DB that will be returned back to the main(). */
    //TransXDB myDB(initial_balances);
    /** Return the DB handle back to the caller. */
    return std::move(TransXDB(initial_balances));
}

template<typename DB>
static void print_transactions(const DB& db, ofstream& fout) {
    auto trxs = db.get_applied_transactions();
    sort(trxs.begin(), trxs.end());
    fout << trxs.size() << endl;
    for (size_t cur : trxs) {
        fout << cur << endl;
    }
}

template<typename DB>
static void print_database(const DB& db, ofstream& fout) {
    auto balances = db.get_balances();
    sort(balances.begin(), balances.end(), [](const auto& a, const auto& b) {
        return a.account_id < b.account_id;
        });

    fout << balances.size() << endl;
    for (const auto& cur : balances) {
        fout << cur.account_id << " " << cur.balance << endl;
    }
}

int main() {
    ofstream fout(getenv("OUTPUT_PATH"));

    try {
        ifstream fin;
        auto input_path = getenv("INPUT_PATH");
        if (input_path != nullptr) {
            fin.open(input_path);
        }

        istream& in = input_path ? fin : cin;

        int remaining_accounts = 0;
        in >> remaining_accounts;
        in.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<account_balance> initial_balances;
        while (remaining_accounts-- > 0) {
            int account = 0;
            int balance = 0;

            in >> account >> balance;
            in.ignore(numeric_limits<streamsize>::max(), '\n');

            initial_balances.push_back({ account, balance });
        }

        auto db = create_database(initial_balances);

        int remaining_transactions = 0;
        in >> remaining_transactions;
        in.ignore(numeric_limits<streamsize>::max(), '\n');
        while (remaining_transactions-- > 0) {

            int remaining_transfers = 0;
            in >> remaining_transfers;
            in.ignore(numeric_limits<streamsize>::max(), '\n');

            transaction tx;

            while (remaining_transfers-- > 0) {
                int from = 0;
                int to = 0;
                int amount = 0;

                in >> from >> to >> amount;
                in.ignore(numeric_limits<streamsize>::max(), '\n');
                tx.push_back({ from, to, amount });
            }

            db.push_transaction(tx);
        }

        db.settle();

        print_transactions(db, fout);

        print_database(db, fout);
    }
    catch (...) {
        return -1;
    }

    return 0;
}

