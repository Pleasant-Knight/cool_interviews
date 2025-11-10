/*
 * Merge Two Sorted Arrays to Find Median

* Problem: Given two sorted arrays nums1 and nums2, find the median of the merged sorted array without modifying the inputs. Return a single float if total length is odd, average of two middle elements if even.
* Constraints: 0 ≤ nums1.length, nums2.length ≤ 1000; -10^6 ≤ nums[i] ≤ 10^6.
* C++ Focus: 
*   Use binary search for O(log(min(m,n))) time.
*   Leverage std::vector for temporary storage if needed, but aim for in-place logic.
*   Handle floating-point precision with double.
* Time to Solve: 15-25 minutes.
* Follow-up: Extend to k-th smallest element (generalize to selection problem).
*
* Explanation:
* To find the median of two sorted arrays without merging them, we can use a binary search approach.
* The idea is to partition both arrays such that the left halves contain the smaller elements and the right halves contain the larger elements.
* We adjust the partitions using binary search until we find the correct partitioning that satisfies the median conditions.
* Once the correct partitions are found, the median can be calculated based on the maximum of the left halves and the minimum of the right halves.
* Sample Input/Output:
    vector<int> nums1 = {1, 3};
    vector<int> nums2 = {2};

    Output: 2.0 (median of [1,2,3])
 */

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <climits>


double findMedianSortedArrays(const std::vector<int>& nums1, const std::vector<int>& nums2) {
    const std::vector<int>& A = (nums1.size() < nums2.size()) ? nums1 : nums2;
    const std::vector<int>& B = (nums1.size() < nums2.size()) ? nums2 : nums1;
    int m = A.size();
    int n = B.size();
    
    int low = 0, high = m;
    while (low <= high) {
        int partitionA = (low + high) / 2;
        int partitionB = (m + n + 1) / 2 - partitionA;

        int maxLeftA = (partitionA == 0) ? INT_MIN : A[partitionA - 1];
        int minRightA = (partitionA == m) ? INT_MAX : A[partitionA];

        int maxLeftB = (partitionB == 0) ? INT_MIN : B[partitionB - 1];
        int minRightB = (partitionB == n) ? INT_MAX : B[partitionB];

        if (maxLeftA <= minRightB && maxLeftB <= minRightA) {
            if ((m + n) % 2 == 0) {
                return (std::max(maxLeftA, maxLeftB) + std::min(minRightA, minRightB)) / 2.0;
            } else {
                return static_cast<double>(std::max(maxLeftA, maxLeftB));
            }
        } else if (maxLeftA > minRightB) {
            high = partitionA - 1;
        } else {
            low = partitionA + 1;
        }
    }
    throw std::invalid_argument("Input arrays are not sorted or invalid.");
}

// Returns the 1-based k-th smallest element from the virtual merge of A and B.
// Throws std::out_of_range if k < 1 or k > A.size() + B.size().
int kthTwoSorted(const vector<int>& A, const vector<int>& B, int k) {
    const size_t nA = A.size();
    const size_t nB = B.size();
    const long long total = static_cast<long long>(nA) + static_cast<long long>(nB);
    if (k < 1 || static_cast<long long>(k) > total) {
        throw out_of_range("k is out of range for sizes of A and B");
    }

    size_t i = 0, j = 0; // current heads into A and B
    while (true) {
        // If A is exhausted, answer is in B.
        if (i == nA) {
            return B[j + k - 1];
        }
        // If B is exhausted, answer is in A.
        if (j == nB) {
            return A[i + k - 1];
        }
        // If we want the 1st, it's just the min of the heads.
        if (k == 1) {
            return min(A[i], B[j]);
        }

        // Choose steps to compare roughly half of k, bounded by remaining lengths.
        int half = k / 2;
        size_t stepA = min(static_cast<size_t>(half), nA - i);
        size_t stepB = min(static_cast<size_t>(half), nB - j);

        // Compare the step-th elements from current heads.
        int valA = A[i + stepA - 1];
        int valB = B[j + stepB - 1];

        if (valA <= valB) {
            // Discard stepA elements from A's front segment
            i += stepA;
            k -= static_cast<int>(stepA);
        } else {
            // Discard stepB elements from B's front segment
            j += stepB;
            k -= static_cast<int>(stepB);
        }
    }
}
// My solution from leetcode 2022.
double findMedianSortedArrays_michoo(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size();
        bool isOdd = (((n + m) % 2) == 1) ? true : false;
        int i = 0, j = 0, k = 0, MED1 = 0, MED2 = 0, N = ((n + m) / 2) + 1;
        bool fin1 = false, fin2 = false; // Two flags to denote when nums1 and nums2 each is done.
        
        // Check to see if any of the arrays is empty
        if (m == 0)
            fin1 = true;
        if (n == 0)
            fin2 = true;

        while (k < N) {
            ++k;
            MED2 = MED1;
            if (fin1) {
                MED1 = nums2[j++];
            } else if (fin2) {
                MED1 = nums1[i++];
            } else if (nums1[i] < nums2[j]) {
                MED1 = nums1[i++];
            } else {
                MED1 = nums2[j++];
            }

            if (i >= m)
                fin1 = true;
            if (j >= n)
                fin2 = true;
        }
        
        if (isOdd)
            return ((double) 1.00 * MED1);
        else
            return ((double) ((MED1 + MED2) / 2.00));
    }