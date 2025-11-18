/*
LeetCode: 3481. Maximum Amount of Momey the Robot Can Earn.

You are given an m x n grid. A robot starts at the top-left corner of the grid (0, 0) and wants to reach
 the bottom-right corner (m - 1, n - 1). The robot can move either right or down at any point in time.

The grid contains a value coins[i][j] in each cell:

If coins[i][j] >= 0, the robot gains that many coins.
If coins[i][j] < 0, the robot encounters a robber, and the robber steals the absolute value of coins[i][j] coins.
The robot has a special ability to neutralize robbers in at most 2 cells on its path, preventing them from stealing coins in those cells.

Note: The robot's total coins can be negative.

Return the maximum profit the robot can gain on the route.
 

Example 1: 
Input: coins = [[0,1,-1],[1,-2,3],[2,-3,4]]

Output: 8

Explanation:
An optimal path for maximum coins is:
    Start at (0, 0) with 0 coins (total coins = 0).
    Move to (0, 1), gaining 1 coin (total coins = 0 + 1 = 1).
    Move to (1, 1), where there's a robber stealing 2 coins. The robot uses one neutralization here, avoiding the robbery (total coins = 1).
    Move to (1, 2), gaining 3 coins (total coins = 1 + 3 = 4).
    Move to (2, 2), gaining 4 coins (total coins = 4 + 4 = 8).

Example 2:
Input: coins = [[10,10,10],[10,10,10]]

Output: 40

Explanation:
An optimal path for maximum coins is:
    Start at (0, 0) with 10 coins (total coins = 10).
    Move to (0, 1), gaining 10 coins (total coins = 10 + 10 = 20).
    Move to (0, 2), gaining another 10 coins (total coins = 20 + 10 = 30).
    Move to (1, 2), gaining the final 10 coins (total coins = 30 + 10 = 40).
 

Constraints:
    m == coins.length
    n == coins[i].length
    1 <= m, n <= 500
    -1000 <= coins[i][j] <= 1000
*/

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxProfit(vector<vector<int>>& coins) {
        int m = coins.size();
        int n = coins.empty() ? 0 : coins[0].size();
        if (m == 0 || n == 0) return 0;

        const int INF = numeric_limits<int>::min() / 2;   // avoid overflow on addition

        // dp[k][j] = best coins arriving at column j having used exactly k neutralizations
        vector<vector<int>> prev(3, vector<int>(n, INF));
        vector<vector<int>> cur (3, vector<int>(n, INF));

        // start cell
        prev[0][0] = coins[0][0];
        if (coins[0][0] < 0) {               // we can neutralize the very first cell
            prev[1][0] = 0;
        }

        // fill first row (can only move right)
        for (int j = 1; j < n; ++j) {
            int val = coins[0][j];
            int penalty = val;
            int neutralized = (val >= 0 ? val : 0);

            for (int k = 0; k < 3; ++k) {
                if (prev[k][j-1] == INF) continue;
                // normal move
                prev[k][j] = max(prev[k][j], prev[k][j-1] + penalty);
                // neutralize this cell
                if (val < 0 && k + 1 < 3)
                    prev[k+1][j] = max(prev[k+1][j], prev[k][j-1] + neutralized);
            }
        }

        // fill remaining rows
        for (int i = 1; i < m; ++i) {
            fill(cur[0].begin(), cur[0].end(), INF);
            fill(cur[1].begin(), cur[1].end(), INF);
            fill(cur[2].begin(), cur[2].end(), INF);

            for (int j = 0; j < n; ++j) {
                int val = coins[i][j];
                int penalty = val;
                int neutralized = (val >= 0 ? val : 0);

                for (int k = 0; k < 3; ++k) {
                    if (prev[k][j] == INF && (j == 0 || cur[k][j-1] == INF)) continue;

                    // from top
                    if (prev[k][j] != INF) {
                        cur[k][j] = max(cur[k][j], prev[k][j] + penalty);
                        if (val < 0 && k + 1 < 3)
                            cur[k+1][j] = max(cur[k+1][j], prev[k][j] + neutralized);
                    }
                    // from left
                    if (j > 0 && cur[k][j-1] != INF) {
                        cur[k][j] = max(cur[k][j], cur[k][j-1] + penalty);
                        if (val < 0 && k + 1 < 3)
                            cur[k+1][j] = max(cur[k+1][j], cur[k][j-1] + neutralized);
                    }
                }
            }
            prev.swap(cur);
        }

        int ans = INF;
        for (int k = 0; k < 3; ++k)
            ans = max(ans, prev[k][n-1]);
        return ans;
    }
};

int main() {
    Solution solution;
    cout << "=== Maximum Amount of Money the Robot Can Earn ===\n";
    cout << "= Compile:  g++ -std=c++11 money_robot.cpp -o money_robot\n";
    // Example 1
    cout << "========== Example 1 ==========\n";
    vector<vector<int>> coins1 = {
        {0, 1, -1},
        {1, -2, 3},
        {2, -3, 4}
    };
    int result1 = solution.maxProfit(coins1);
    cout << "Maximum profit: " << result1 << "\n";
    cout << "Expected: 8\n";
    cout << endl;

    // Example 2
    cout << "========== Example 2 ==========\n";
    vector<vector<int>> coins2 = {
        {10, 10, 10},
        {10, 10, 10}
    };
    int result2 = solution.maxProfit(coins2);
    cout << "Maximum profit: " << result2 << "\n";
    cout << "Expected: 40\n\n";

    return 0;
}