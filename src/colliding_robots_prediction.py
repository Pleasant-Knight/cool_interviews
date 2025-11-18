class Solution:
    def sumDistance(self, nums: list[int], s: str, d: int) -> int:
        MOD = 10**9 + 7
        n = len(nums)
        
        # Calculate final positions (treating robots as passing through each other)
        final_positions = []
        for i in range(n):
            if s[i] == 'R':
                final_positions.append(nums[i] + d)
            else:  # s[i] == 'L'
                final_positions.append(nums[i] - d)
        
        # Sort the final positions
        final_positions.sort()
        
        # Calculate sum of distances between all pairs
        # For sorted positions, we can use the formula:
        # For each position i, it contributes:
        # - positive: position[i] * i (distance to all positions on the left)
        # - negative: position[i] * (n - 1 - i) (distance to all positions on the right)
        
        total_sum = 0
        for i in range(n):
            # This position contributes to (i) pairs on the left
            # and (n - 1 - i) pairs on the right
            contribution = final_positions[i] * (2 * i - n + 1)
            total_sum = (total_sum + contribution) % MOD
        
        return total_sum


# Test cases
def test():
    sol = Solution()
    
    # Example 1
    nums1 = [-2, 0, 2]
    s1 = "RLL"
    d1 = 3
    print(f"Example 1: {sol.sumDistance(nums1, s1, d1)}")  # Expected: 8
    
    # Example 2
    nums2 = [1, 0]
    s2 = "RL"
    d2 = 2
    print(f"Example 2: {sol.sumDistance(nums2, s2, d2)}")  # Expected: 5
    
    # Manual verification for Example 1
    # Final positions: [-2+3, 0-3, 2-3] = [1, -3, -1]
    # Sorted: [-3, -1, 1]
    # Distances: |-3-(-1)| + |-3-1| + |-1-1| = 2 + 4 + 2 = 8 ✓

test()