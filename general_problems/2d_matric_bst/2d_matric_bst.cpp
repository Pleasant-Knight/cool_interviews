/**
* Search a 2D Matrix Medium problem.
* You are given an m x n integer matrix matrix with the following two properties:
* Each row is sorted in non-decreasing order.
* The first integer of each row is greater than the last integer of the previous row.
* Given an integer target, return true if target is in matrix or false otherwise.
* You must write a solution in O(log(m * n)) time complexity.
*/
class Solution {
public:
    bool BinarySearch(vector<int>& nums, int target) {
        if (nums.empty() || target < nums[0] || target > nums[nums.size() - 1])
            return false;
        auto l = 0;
        auto r = nums.size();
        auto m = 0;

        while (l <= r) {
            m = (l + r) / 2;
            if (nums[m] == target)
                return true;
            else if (target > nums[m])
                l = m + 1;
            else
                r = m - 1;
        }
        return false;
    }

    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if (matrix.empty() || matrix[0][0] > target || target > matrix[m][n])
            return false;
        auto m = matrix.size() - 1;
        auto n = matrix[0].size() - 1;
        auto top = 0;   // top row.
        auto bot = m;   // bottom row.
        auto row = 0;   // middle row.

        while (top <= bot) {
            row = (top + bot) / 2;
            if (target > matrix[row][n]) {
                top = row + 1;
            }
            else if (target < matrix[row][0]) {
                bot = row - 1;
            }
            else {
                return BinarySearch(matrix[row], target);
            }
        }

        return false;
    }
};
