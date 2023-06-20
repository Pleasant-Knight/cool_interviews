#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

int countPairs(std::vector<int>& ary, int k) {
    if (ary.size() < 2)
        return 0;
    else
        cout << "Ary size = " << ary.size() << "\n\n";
    
    std::unordered_map<int, int> ht;
    int comp = 0;
    for (int i = 0; i < (int)ary.size(); i++) {
        if (ary[i] > k)
            continue;
        comp = std::abs(k - ary[i]);
        ht[comp]++;
    }
    int sol = 0;
    for (auto &it : ht) {
        if ((it.first * 2 == k) && (it.second >= 2)) {
            ++sol;
            it.second = 0;
            cout << "(" << it.first << ", " << it.first << ")\n";
        } else {
            auto idx = ht.find(std::abs(it.first - k));
            if ((idx != ht.end()) && (idx->second != 0)) {
                ++sol;
                cout << "(" << it.first << ", " << (abs(it.first - k)) << ")\n";
                idx->second = 0;
                it.second = 0;
            }
        }
    }

    return sol;
}


int main() {
    std::vector<int> mtx = { 1, 11, 5, 7, 12, 15, 0, 9, 5, 6, 6, 6, 8, 3, 1, 4, 10 };

    int k = 12, ans = countPairs(mtx, k);

    cout << "Ans = " << ans << std::endl;
}