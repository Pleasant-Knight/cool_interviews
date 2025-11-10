/*
* Word Break (Dynamic Programming):
*
* Problem: Given a string s and a dictionary of words wordDict, determine if s can be segmented into
* space-separated words from the dictionary.
* Return true if possible, false otherwise.
*
* Constraints: 1 ≤ s.length ≤ 300; 1 ≤ wordDict.length ≤ 1000; words are unique, length 1-20.
* C++ Focus: 
*   Use std::unordered_set for O(1) word lookups and std::vector<bool> for DP table. 
*   Optimize space by using a set to track reachable positions.
* Time to Solve: 20-30 minutes.
* Follow-up: If true, return all possible segmentations (backtracking).
* 
* Sample Input/Output:
*   string s = "leetcode";
*   vector<string> wordDict = {"leet", "code"};
*   Output: true ("leet code")
*/

include <string>
#include <vector>
#include <unordered_set>

// My solution from leetcode 2022.
using namespace std;
// DP solution:
/*
* Explaination: 
* We use a boolean vector `ans` where `ans[i]` indicates if the substring `s[0:i]` can be segmented into words from the dictionary.
* We initialize `ans[0]` to true since an empty string can always be segmented.
* For each position `i` in the string, we check all previous positions `j` (from 0 to i).
* If `ans[j]` is true (meaning `s[0:j]` can be segmented) and the substring `s[j:i]` exists in the dictionary, we set `ans[i]` to true.
* This means `s[0:i]` can also be segmented. We break out of the inner loop early for efficiency.
* Finally, we return `ans[s.size()]`, which tells us if the entire string can be segmented.
*/
bool wordBreak(string s, vector<string>& dict) {
        if (s.size() == 0)
            return true;
        
        unordered_set<string> ht(dict.begin(), dict.end());
        int siz_ = static_cast<int>(s.size());
        vector<bool> ans(siz_ + 1, false);
        ans[0] = true;
        for (int i = 1; i <= siz_; i++) {
            for (int j = 0; j < i; j++) {
                if (ans[j] && (ht.find(s.substr(j, (i - j))) != ht.end())) {
                    ans[i] = true;
                    break;
                }
            }
        }
        return ans[siz_];
}

// Example usage
int main() {
    string s = "leetcode";
    vector<string> wordDict = {"leet", "code"};
    bool result = wordBreak(s, wordDict);
    if (result) {
        printf("The string can be segmented into words from the dictionary.\n");
    } else {
        printf("The string cannot be segmented into words from the dictionary.\n");
    }
    return 0;
}