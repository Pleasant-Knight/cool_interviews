#include <bits/stdc++.h>
using namespace std;

/*
Problem: Maximum Unique Walls Destroyed by Robots

Algorithm Description:

1. Sort all positions:
   - Combine robots and walls into events with type (robot or wall)
   - Sort all unique positions in increasing order

2. For each robot, precompute the range it can cover in both directions:
   - Left range:  [robots[i] - distance[i], robots[i]]
   - Right range: [robots[i], robots[i] + distance[i]]

3. The key insight:
   - A robot can destroy a wall if:
     - The wall is within its firing range (left or right)
     - There is NO other robot strictly between the robot and the wall
       (because bullet stops at the first robot it hits)

4. To check this efficiently:
   - Sort robots by position
   - For each robot, find the nearest robot to its left and to its right
   - Then the actual reachable interval in each direction is limited by:
     - The firing distance
     - The nearest robot in that direction (exclusive)

   So:
   - Left reachable:  max( robot_pos - dist, left_neighbor_pos + 1 )
   - Right reachable: min( robot_pos + dist, right_neighbor_pos - 1 )

5. A robot firing left can destroy all walls in [L_reach, robot_pos]
   A robot firing right can destroy all walls in [robot_pos, R_reach]

6. Each robot chooses one direction (left or right). We want to assign directions
   such that the total number of unique walls covered is maximized.

7. Greedy strategy:
   - Process robots from left to right
   - For each robot, try to cover as many new walls as possible
   - Prefer extending to the right when possible (because future robots might be blocked)
   - But also allow left-firing when it covers walls not yet covered

8. Use a set to track destroyed walls (unique positions)

9. Final answer = size of the set of destroyed walls

Time Complexity: O(n log n + m log m) due to sorting and set operations
Space Complexity: O(n + m)
*/

int maxUniqueWallsDestroyed(vector<int> robots, vector<int> dist, vector<int> walls) {
    int n = robots.size();
    if (n == 0) return 0;

    vector<pair<int, int>> robs;
    for (int i = 0; i < n; ++i) {
        robs.emplace_back(robots[i], dist[i]);
    }
    sort(robs.begin(), robs.end());

    // Find nearest left and right robot for each
    vector<long long> leftLimit(n), rightLimit(n);
    for (int i = 0; i < n; ++i) {
        long long pos = robs[i].first;
        long long d = robs[i].second;

        // Left reachable: limited by distance and nearest left robot
        long long leftReach = pos - d;
        if (i > 0) {
            long long prevRobot = robs[i - 1].first;
            leftReach = max(leftReach, prevRobot + 1);
        }
        leftLimit[i] = leftReach;

        // Right reachable: limited by distance and nearest right robot
        long long rightReach = pos + d;
        if (i + 1 < n) {
            long long nextRobot = robs[i + 1].first;
            rightReach = min(rightReach, nextRobot - 1);
        }
        rightLimit[i] = rightReach;
    }

    // Put all walls into a sorted set for fast range queries
    set<long long> wallSet;
    for (int w : walls) {
        wallSet.insert(w);
    }

    set<long long> destroyed;

    // Greedy: each robot picks the direction that destroys more new walls
    for (int i = 0; i < n; ++i) {
        long long pos = robs[i].first;
        long long L = leftLimit[i];
        long long R = rightLimit[i];

        int leftCount = 0, rightCount = 0;

        // Count new walls on left [L, pos]
        auto it1 = wallSet.lower_bound(L);
        for (; it1 != wallSet.end() && *it1 <= pos; ++it1) {
            if (destroyed.find(*it1) == destroyed.end()) leftCount++;
        }

        // Count new walls on right [pos, R]
        auto it2 = wallSet.lower_bound(pos);
        for (; it2 != wallSet.end() && *it2 <= R; ++it2) {
            if (destroyed.find(*it2) == destroyed.end()) rightCount++;
        }

        // Choose the direction with more new walls
        if (leftCount >= rightCount) {
            // Fire left
            auto it = wallSet.lower_bound(L);
            for (; it != wallSet.end() && *it <= pos; ++it) {
                destroyed.insert(*it);
            }
        } else {
            // Fire right
            auto it = wallSet.lower_bound(pos);
            for (; it != wallSet.end() && *it <= R; ++it) {
                destroyed.insert(*it);
            }
        }
    }

    return destroyed.size();
}

int main() {
    // Example 1
    vector<int> r1 = {4};
    vector<int> d1 = {3};
    vector<int> w1 = {1, 10};
    assert(maxUniqueWallsDestroyed(r1, d1, w1) == 1);

    // Example 2
    vector<int> r2 = {10, 2};
    vector<int> d2 = {5, 1};
    vector<int> w2 = {5, 2, 7};
    assert(maxUniqueWallsDestroyed(r2, d2, w2) == 3);

    // Example 3
    vector<int> r3 = {1, 2};
    vector<int> d3 = {100, 1};
    vector<int> w3 = {10};
    assert(maxUniqueWallsDestroyed(r3, d3, w3) == 0);

    return 0;
}
