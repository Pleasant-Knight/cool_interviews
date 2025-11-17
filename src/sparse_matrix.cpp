/*
 * Leetcode: 311. Sparse Matrix Multiplication
* Given two sparse matrices mat1 of size m × k and mat2 of size k × n, return the matrix product of mat1 and mat2.
* You should return a sparse matrix.
*
* Example 1:
Input:
mat1 = [[1,0,0],[-1,0,3]]
mat2 = [[7,0,0],[0,0,0],[0,0,1]]

Output:
[[7,0,0],[-7,0,3]]

* Example 2:
Input:
mat1 = [[0]]
mat2 = [[0]]

Output:
[[0]]

* Constraints:
* - m == mat1.length
* - k == mat1[i].length == mat2.length
* - n == mat2[i].length
* - (1 ≤ m, n, k ≤ 100)
* - (-100 ≤ mat1[i][j], mat2[i][j] ≤ 100)

* Approach:
 *  We’ll treat each matrix as “row → (col, value) list” and only ever touch non-zeros. That is the dict-of-keys / row adjacency-list style.
 * Given mat1 of size m × k and mat2 of size k × n:
 * For mat1: nonZero1[i] = list of (colIndex, value) for row i
 *   For each i in [0..m-1]
 *   For each c in [0..k-1]
 *   If mat1[i][c] != 0, push (c, mat1[i][c]) into nonZero1[i].
 *
 * For mat2: nonZero2[r] = list of (colIndex, value) for row r
 * Here the “row” index r is the same as the shared dimension index k in multiplication.
 *   For each r in [0..k-1]
 *   For each j in [0..n-1]
 *   If mat2[r][j] != 0, push (j, mat2[r][j]) into nonZero2[r].
 *
 * Space: 
  *   O(nnz(mat1) + nnz(mat2)) extra.
  *   Result mat3 is dense m × n, as required.
  *  
  * Multiplication:
  *   for each row i:
  *     for each (k, valA) in nonZero1[i]:
  *       for each (j, valB) in nonZero2[k]:
  *         C[i][j] += valA * valB
  *
 * Time complexity (informally):
*  Preprocessing: O(m*k + k*n) to scan inputs once.
 
  ////////////////////
 * To use hash table to store the valid entries.
 * mat[i][j] = f;
 * key = to_string(i:j);
 * value = f;
 */

#include <vector>
#include <utility> // for std::pair
#include <string>
#include <cstdio>

using namespace std;


class Solution {
public:
    vector<vector<int>> multiply(vector<vector<int>>& mat1,
                                 vector<vector<int>>& mat2) {
        int m = mat1.size();
        int k = mat1[0].size();
        int n = mat2[0].size();

        // Build row adjacency list for mat1: nonZero1[i] = {(col, val), ...}
        vector<vector<pair<int, int>>> nonZero1(m);
        for (int i = 0; i < m; ++i) {
            for (int c = 0; c < k; ++c) {
                int val = mat1[i][c];
                if (val != 0) {
                    nonZero1[i].push_back({c, val});
                }
            }
        }

        // Build row adjacency list for mat2: nonZero2[r] = {(col, val), ...}
        // Note r is the same as the shared dimension index k in multiplication
        vector<vector<pair<int, int>>> nonZero2(k);
        for (int r = 0; r < k; ++r) {
            for (int j = 0; j < n; ++j) {
                int val = mat2[r][j];
                if (val != 0) {
                    nonZero2[r].push_back({j, val});
                }
            }
        }

        // Result matrix
        vector<vector<int>> result(m, vector<int>(n, 0));

        // Multiply using only non-zero entries
        for (int i = 0; i < m; ++i) {
            // Skip rows of A that are entirely zero
            if (nonZero1[i].empty()) 
              continue;

            for (const auto& [colA, valA] : nonZero1[i]) {
                // Row colA in mat2 contributes to row i in result.
                const auto& rowB = nonZero2[colA];
                if (rowB.empty())
                  continue;

                for (const auto& [colB, valB] : rowB) {
                    result[i][colB] += valA * valB;
                }
            }
        }

        return result;
    }
};


class Matrix {
  public:
  Matrix();
  push_element(int i, int j, float v);
  Matrix* multiply(Matrix &&other);
  
  private:
  bool unordered_list <stding, float> ht;
  size_t n_;	// rows
  size_t m_;	//cols
};

Matrix::Matrix(vector<pair<int,int> K, vector<float> V, size_t n, size_t m) {
  if (K.empty() || V.empty() || K.size() != V.size()) {
    std::printf("Error, empty init vector...\n");
  }
  
  for (size_t i; i < K.size(); i++) {
    auto k = K[i];
    auto v = V[i];
    push_element(k.first, k.second, v);
  }
  
  n_ = n;
  m_ = m;
}

bool Matrix::push_element(int i, int j, float v) {
  if (i >= n_ || i < 0 || j > m_ || j < 0) {
    return false;
  }
  auto key = std::to_string(i);
  key += ":";
  key += std::to_string(j);
  
  ht.insert(key, v);
  
  return true;
}

Matrix* multiply(Matrix &&other) {
  //Check if we can multiply 
  if (n_ != other_n || other.m_ < 1)
    return nullptr;
  
  
  
  
}
