/*
* Leetcode Problem: Number of Spaces Cleaning Robot Cleaned.
* Problem Description:
* A room is represented by a 0-indexed 2D binary matrix room where a 0 represents an empty space and
* a 1 represents a space with an object. The robot starts at position (row, col) in the room and is 
* facing in one of the four cardinal directions: "Up", "Right", "Down", or "Left".
* The robot will clean every empty space it visits. It follows these rules:

* The robot tries to move forward one cell in the direction it is facing
* If the cell it's moving to is out of bounds or contains an object, the robot stays in the current cell
* After attempting to move, the robot always turns 90 degrees clockwise
* The robot repeats steps 1-3 until it returns to its starting position and direction

* Return the number of unique empty spaces the robot cleaned.

* Constraints:
* - m == room.length
* - n == room[r].length
* - 1 <= m, n <= 300
* - room[r][c] is either 0 or 1
* - 0 <= row < m
* - 0 <= col < n
* - room[row][col] == 0

* Example
* - Input: room = [[0,0,0],[1,1,1],[0,0,0]], row = 0, col = 0, direction = "Right"
* Output: 7
* Explanation: The robot cleans spaces at (0,0), (0,1), (0,2), (1,2), (2,2), (2,1), (2,0).

* 
## How the Solution Works

### Key Insights
1. **Cycle Detection**: The robot will eventually return to a previous state (same position + same direction), creating a cycle. We need to detect this to know when to stop.
2. **State Tracking**: A "state" is defined as `(row, col, direction)`. When we revisit a state, we've completed the cycle.
3. **Unique Cleaning**: We track all unique positions visited, regardless of how many times we visit them.

### Algorithm Steps
1. **Setup**:
   - Map direction strings to integers (0-3)
   - Define direction vectors for movement
   - Create a set to track cleaned positions
   - Create a set to track visited states (for cycle detection)
2. **Simulation Loop**:
   - Clean the current cell (add to `cleaned` set)
   - Check if current state has been seen before → if yes, we've completed the cycle, break
   - Add current state to `states` set
   - Calculate next position based on current direction
   - If next position is valid (in bounds and not an obstacle), move to it
   - If not valid, stay in current position
   - **Always** turn 90° clockwise (increment direction mod 4)
3. **Return Result**:
   - Return the size of the `cleaned` set (number of unique positions)

### Example Trace
For `room = [[0,0,0],[1,1,1],[0,0,0]]`, starting at `(0,0)` facing `"Right"`:
```
(0,0) Right → move to (0,1), turn Down → cleaned: {(0,0), (0,1)}
(0,1) Down → blocked by (1,1), stay, turn Left → cleaned: {(0,0), (0,1)}
(0,1) Left → move to (0,0), turn Up → cleaned: {(0,0), (0,1)}
(0,0) Up → out of bounds, stay, turn Right → cleaned: {(0,0), (0,1)}
(0,0) Right → move to (0,1), turn Down → STATE REPEATED!

*/

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <tuple>
#include <string>

using namespace std;

class Solution {
public:
    int numberOfCleanRooms(vector<vector<int>>& room, int row, int col, string direction) {
        int m = room.size();
        int n = room[0].size();
        
        // Direction mappings: Up=0, Right=1, Down=2, Left=3
        map<string, int> dirMap = {{"Up", 0}, {"Right", 1}, {"Down", 2}, {"Left", 3}};
        int dir = dirMap[direction];
        
        // Direction vectors: Up, Right, Down, Left
        int dx[] = {-1, 0, 1, 0};
        int dy[] = {0, 1, 0, -1};
        
        // Track visited cells (to count unique cleaned spaces)
        set<pair<int, int>> cleaned;
        
        // Track states (position + direction) to detect cycle
        set<tuple<int, int, int>> states;
        
        int x = row, y = col;
        
        while (true) {
            // Clean current cell
            cleaned.insert({x, y});
            
            // Check if we've seen this state before (cycle detected)
            auto state = make_tuple(x, y, dir);
            if (states.count(state)) {
                break;
            }
            states.insert(state);
            
            // Try to move forward
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            
            // Check if next cell is valid and empty
            if (nx >= 0 && nx < m && ny >= 0 && ny < n && room[nx][ny] == 0) {
                x = nx;
                y = ny;
            }
            // If can't move (out of bounds or obstacle), stay in place
            
            // Always turn clockwise after attempting to move
            dir = (dir + 1) % 4;
        }
        
        return cleaned.size();
    }
};

// Helper function to print the room grid
void printRoom(const vector<vector<int>>& room, int startRow, int startCol) {
    cout << "Room layout (0=empty, 1=object, S=start):" << endl;
    for (int i = 0; i < room.size(); i++) {
        for (int j = 0; j < room[0].size(); j++) {
            if (i == startRow && j == startCol) {
                cout << "S ";
            } else {
                cout << room[i][j] << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    Solution solution;
    cout << "=== Number of Spaces Cleaning Robot Cleaned ===\n";
    cout << "= Compile:  g++ -std=c++11 cleaning_robot.cpp -o cleaning_robot\n" << endl;

    // Example 1: Robot moves around obstacles
    cout << "========== Example 1 ==========" << endl;
    vector<vector<int>> room1 = {
        {0, 0, 0},
        {1, 1, 1},
        {0, 0, 0}
    };
    int row1 = 0, col1 = 0;
    string direction1 = "Right";
    
    printRoom(room1, row1, col1);
    cout << "Starting position: (" << row1 << ", " << col1 << ")\n";
    cout << "Starting direction: " << direction1 << endl;
    
    int result1 = solution.numberOfCleanRooms(room1, row1, col1, direction1);
    cout << "Number of spaces cleaned: " << result1 << "\n";
    cout << "Expected: 7" << endl;
    cout << endl;
    
    // Example 2: Simple corridor
    cout << "========== Example 2 ==========\n";
    vector<vector<int>> room2 = {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    };
    int row2 = 0, col2 = 0;
    string direction2 = "Right";
    
    printRoom(room2, row2, col2);
    cout << "Starting position: (" << row2 << ", " << col2 << ")\n";
    cout << "Starting direction: " << direction2 << endl;
    
    int result2 = solution.numberOfCleanRooms(room2, row2, col2, direction2);
    cout << "Number of spaces cleaned: " << result2 << "\n";
    cout << "Expected: 10" << endl;
    cout << endl;
    
    // Example 3: Single cell
    cout << "========== Example 3 ==========\n";
    vector<vector<int>> room3 = {
        {0, 1},
        {1, 0}
    };
    int row3 = 0, col3 = 0;
    string direction3 = "Down";
    
    printRoom(room3, row3, col3);
    cout << "Starting position: (" << row3 << ", " << col3 << ")\n";
    cout << "Starting direction: " << direction3 << endl;
    
    int result3 = solution.numberOfCleanRooms(room3, row3, col3, direction3);
    cout << "Number of spaces cleaned: " << result3 << "\n";
    cout << "Expected: 1" << endl;
    cout << endl;
    
    // Example 4: Large open room
    cout << "========== Example 4 ==========\n";
    vector<vector<int>> room4 = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    int row4 = 1, col4 = 1;
    string direction4 = "Up";
    
    printRoom(room4, row4, col4);
    cout << "Starting position: (" << row4 << ", " << col4 << ")\n";
    cout << "Starting direction: " << direction4 << endl;
    
    int result4 = solution.numberOfCleanRooms(room4, row4, col4, direction4);
    cout << "Number of spaces cleaned: " << result4 << "\n";
    cout << "Expected: 9\n\n";
    
    return 0;
}