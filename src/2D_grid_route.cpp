/*
* 2D Grid Route with “Soft” Obstacles (Min-Cost A*)
* Theme: warehouse routing with penalties vs. hard blocks (ties to Cobot’s material movement).
* Task: 
*   Given an N×M grid:
*    0 = free (cost 1 per move),
*    -1 = hard obstacle (impassable),
*   k > 0 = soft obstacle adds k extra cost when entering the cell.
* Move in 4 directions.
*
*Implement:
* int minRouteCost(const std::vector<std::vector<int>>& grid,
*                  std::pair<int,int> start,
*                  std::pair<int,int> goal);
*
* Return -1 if unreachable.
*
* Constraints:
*  - N,M ≤ 200.
*
* Use a heuristic that keeps it fast (≤ O(NM log NM) typical).
*
Be careful with overflow and tie-breaking.
*
Time budget: 35–40 min.
*/
#include <vector>
#include <queue>
#include <utility>
#include <limits>
#include <cmath>
#include <functional>

using namespace std;

using i64 = long long;

struct Node_1 {
  int r, c;
  i64 g;   // cost-so-far
  i64 f;   // g + h
};

// Min-heap by (f, then g)
struct Cmp {
  bool operator()(const Node_1& a, const Node_1& b) const {
    if (a.f != b.f) return a.f > b.f;     // smaller f first
    return a.g > b.g;                     // tie-break: smaller g first
  }
};

static inline i64 manhattan_164(int r, int c, int gr, int gc) {
  return std::llabs((long long)r - gr) + std::llabs((long long)c - gc);
}

// Returns -1 if unreachable
int minRouteCost_CHTGPT(const vector<vector<int>>& grid,
                 pair<int,int> start,
                 pair<int,int> goal) {
  const int n = (int)grid.size();
  if (n == 0) return -1;
  const int m = (int)grid[0].size();
  if (m == 0) return -1;

  auto [sr, sc] = start;
  auto [tr, tc] = goal;

  auto inb = [&](int r, int c){ return r>=0 && r<n && c>=0 && c<m; };

  if (!inb(sr,sc) || !inb(tr,tc)) return -1;
  if (grid[sr][sc] == -1 || grid[tr][tc] == -1) return -1;
  if (sr == tr && sc == tc) return 0;

  const i64 INF = (i64)9e18;
  vector<vector<i64>> best(n, vector<i64>(m, INF));

  priority_queue<Node_1, vector<Node_1>, Cmp> pq;

  // Start node: entering start does not incur a cost; cost starts at 0
  best[sr][sc] = 0;
  pq.push(Node_1{sr, sc, 0, manhattan_164(sr, sc, tr, tc)});

  const int dr[4] = {-1, 1, 0, 0};
  const int dc[4] = {0, 0, -1, 1};

  while (!pq.empty()) {
    Node_1 cur = pq.top(); pq.pop();

    if (cur.r == tr && cur.c == tc) {
      // First time we pop goal with consistent heuristic => optimal
      return (int)cur.g; // safe to cast if problem expects int
    }
    if (cur.g != best[cur.r][cur.c]) {
      // Outdated entry
      continue;
    }

    for (int k = 0; k < 4; ++k) {
      int nr = cur.r + dr[k], nc = cur.c + dc[k];
      if (!inb(nr, nc)) continue;
      int cell = grid[nr][nc];
      if (cell == -1) continue; // blocked
      i64 step = 1 + (cell > 0 ? (i64)cell : 0LL); // entering neighbor cost
      i64 ng = cur.g + step;
      if (ng < best[nr][nc]) {
        best[nr][nc] = ng;
        i64 h = manhattan_164(nr, nc, tr, tc); // admissible lower bound
        pq.push(Node_1{nr, nc, ng, ng + h});
      }
    }
  }

  return -1; // unreachable
}


using Pos = std::pair<int, int>;

struct Node_2 {
    Pos pos;
    int g;                     // cost from start to this node
    int f;                     // g + h(pos)

    // priority_queue is max-heap → store negative f
    bool operator>(const Node_2& other) const { return f > other.f; }
};

inline int manhattan_int(const Pos& a, const Pos& b) {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

int minRouteCost(const std::vector<std::vector<int>>& grid,
                 Pos start, Pos goal)
{
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid.empty() ? 0 : grid[0].size());
    if (rows == 0 || cols == 0) return -1;

    // ------------------------------------------------------------------
    // 1. sanity checks
    // ------------------------------------------------------------------
    const auto valid = [&](int r, int c) {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    };
    if (!valid(start.first, start.second) || !valid(goal.first, goal.second))
        return -1;
    if (grid[start.first][start.second] == -1 ||
        grid[goal.first][goal.second] == -1)
        return -1;

    // ------------------------------------------------------------------
    // 2. data structures
    // ------------------------------------------------------------------
    const int INF = std::numeric_limits<int>::max() / 2;   // avoid overflow
    std::vector<std::vector<int>> best(rows, std::vector<int>(cols, INF));
    std::priority_queue<Node_2, std::vector<Node_2>, std::greater<Node_2>> pq;

    int h0 = manhattan_int(start, goal);
    best[start.first][start.second] = 0;
    pq.push({start, 0, h0});

    const std::vector<Pos> dirs = {{0,1},{1,0},{0,-1},{-1,0}};

    // ------------------------------------------------------------------
    // 3. A* loop
    // ------------------------------------------------------------------
    while (!pq.empty()) {
        Node_2 cur = pq.top(); 
        pq.pop();
        int r = cur.pos.first;
        int c = cur.pos.second;

        if (cur.g > best[r][c])
            continue;               // older path

        if (cur.pos == goal)
            return cur.g;              // first time we dequeue goal

        for (const auto& d : dirs) {
            int nr = r + d.first, nc = c + d.second;
            if (!valid(nr, nc)) continue;
            if (grid[nr][nc] == -1) continue;           // hard block

            // cost to *enter* the neighbour
            int enter_cost = (grid[nr][nc] > 0 ? grid[nr][nc] : 0);
            int move_cost  = 1 + enter_cost;            // base move + soft penalty
            int tentative  = best[r][c] + move_cost;

            if (tentative < best[nr][nc]) {
                best[nr][nc] = tentative;
                int h = manhattan_int({nr, nc}, goal);
                pq.push({{nr,nc}, tentative, tentative + h});
            }
        }
    }

    return -1;                                          // unreachable
}


// --- tiny demo ---
int main() {
  // 0 = free (cost 1), -1 = wall, k>0 = extra entry cost
  // Example layout:
  // S . .  2  .
  // # # .  #  .
  // .  . .  .  G
  vector<vector<int>> grid = {
    { 0,  0,  0,  2,  0},
    {-1, -1,  0, -1,  0},
    { 0,  0,  0,  0,  0}
  };

  pair<int,int> start{0,0};
  pair<int,int> goal {2,4};

  int cost = minRouteCost(grid, start, goal);
  cout << "Min route cost = " << cost << "\n";

  // A rough expectation:
  // Path will likely avoid the 2 if it’s not worth it vs a detour around walls.
  // You can tweak the grid to see behavior with more soft obstacles.
  return 0;
}
