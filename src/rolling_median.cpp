/**
* @file rolling_median.cpp
* @brief Implementation of rolling median calculation.
* @author Michel Hanna.
* @date October 2025.
* @license MIT License.
* @details This file contains the implementation of a rolling median algorithm
*          that efficiently computes the median of a sliding window over a data stream.
*
* Implement a RollingMedian for a window of the last `W` integers supporting:
* push(int x) inserts a new value.
* median() returns the median of the current window 
*   (odd ⇒ middle; even ⇒ lower-of-two or exact average — interviewer’s choice).
* If size < W, use all available values; when size > W, evict the oldest.
*
* Constraints/notes.
*   - Target O(log n) per push/evict and O(1) median().
*   - No 3rd-party libs; C++17/20 STL only.
*   - Be careful about duplicates and eviction of specific values.
*   - Consider edge cases: empty window, single element, all identical elements.
*   - Write clean, modular, well-documented code.
*   - (extra) Include unit tests demonstrating correctness and performance.
*   - Discuss time/space complexity:
* Design options: 
*   - Two-heap design + lazy deletion (hash map of “tombstones”), or
*   - indexed multiset with iterators that track the median.
*
* Edge talk-track:
*   Exception safety, integer overflow on averaging (use long long), compare performance of multiset vs two-heap.
*
* Examples:
* Basic odd window: W=3, push: 1, 5, 2 ⇒ median: 2.
* Basic even window: W=4, push: 1, 5, 2, 10 ⇒ median: (2+5)/2=3.5 or 2.
* Rolling median:
*    W=3, push: 1, 5, 2, 10, -1 ⇒ medians: 1,3,2,5,2.
* Many duplicates: 
*    W=4, push: 2,2,2,2,2 ⇒ always 2.
* Eviction correctness: 
*    W=2, push INT_MAX, INT_MIN, 0 ⇒ check no overflow if averaging.
*/

// Build: g++ -std=gnu++17 -O2 -Wall -Wextra -pedantic rolling_median.cpp -o rolling_median
//#include <bits/stdc++.h>
#include <queue>
#include <set>
#include <vector>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <limits>
#include <utility>
#include <cstdio>

using namespace std;

/*
  RollingMedian (sliding window)
  - Two heaps: max-heap for lower half, min-heap for upper half
  - Lazy deletion via index marking to handle O(log k) evictions
  - O(log k) per push/evict, O(1) median()
  - Duplicates fully supported
  - Uses 64-bit for safety on sums/averages
*/

class RollingMedian {
public:
    explicit RollingMedian(size_t window)
        : W(window), next_id(0), szMax(0), szMin(0) {}

    // Push a new value; automatically evicts oldest when size exceeds W
    void push(long long x) {
        const long long id = next_id++;
        order.emplace_back(x, id);
        insert_(x, id);

        // Evict if window exceeded
        if (size() > W) {
            auto [oldVal, oldId] = order.front();
            order.pop_front();
            erase_(oldVal, oldId);
        }
        // Ensure clean tops and balance
        pruneMax(); pruneMin();
        rebalance();
    }

    // Current number of elements considered (<= W)
    size_t size() const {
        return static_cast<size_t>(szMax + szMin);
    }

    // Returns true if at least one element is present
    bool has_value() const {
        return size() > 0;
    }

    // Median as double (lower-of-two vs average: we use the exact average)
    double median() {
        pruneMax(); pruneMin();
        if (!has_value()) throw runtime_error("median() on empty structure");

        if (szMax == szMin) {
            long long a = maxHeap.top().first;
            long long b = minHeap.top().first;
            // Avoid overflow by widening
            return (static_cast<long double>(a) + static_cast<long double>(b)) / 2.0L;
        } else {
            return static_cast<double>(maxHeap.top().first);
        }
    }

private:
    // Heaps store (value, id). id is strictly increasing (insertion order).
    // maxHeap: largest of the lower half on top.
    priority_queue<pair<long long,long long>> maxHeap;
    // minHeap: smallest of the upper half on top
    priority_queue<
        pair<long long,long long>,
        vector<pair<long long,long long>>,
        greater<pair<long long,long long>>
    > minHeap;

    // Window size
    size_t W;
    // Monotonic id to uniquely identify each pushed element.
    long long next_id;

    // Logical sizes (count only non-deleted elements assigned to each heap).
    int szMax, szMin;

    // Track which indices are lazily deleted.
    unordered_set<long long> deleted;

    // Track which heap an element was inserted into: 0 -> maxHeap, 1 -> minHeap
    unordered_map<long long, int> heapOf;

    // Maintain insertion order for eviction.
    deque<pair<long long,long long>> order;

    // Insert element into appropriate heap and record its heap side.
    void insert_(long long x, long long id) {
        if (maxHeap.empty() || x <= cleanTop(maxHeap).first) {
            maxHeap.emplace(x, id);
            heapOf[id] = 0;
            ++szMax;
        } else {
            minHeap.emplace(x, id);
            heapOf[id] = 1;
            ++szMin;
        }
        rebalance(); // keep heaps close as we go.
    }

    // Mark an element for deletion; adjust virtual sizes according to its heap.
    void erase_(long long /*val*/, long long id) {
        deleted.insert(id);
        auto it = heapOf.find(id);
        if (it != heapOf.end()) {
            if (it->second == 0) --szMax; else --szMin;
            heapOf.erase(it);
        }
        // We don't pop immediately; prune happens on demand at tops.
    }

    // Ensure size invariant: szMax == szMin or szMax == szMin + 1
    void rebalance() {
        // Clean tops before moving across heaps
        pruneMax(); pruneMin();

        while (szMax > szMin + 1) {
            auto p = cleanTop(maxHeap); popClean(maxHeap);
            // Move top from max -> min
            minHeap.push(p);
            heapOf[p.second] = 1;
            --szMax; ++szMin;
            pruneMax(); pruneMin();
        }
        while (szMin > szMax) {
            auto p = cleanTop(minHeap); popClean(minHeap);
            // Move top from min -> max
            maxHeap.push(p);
            heapOf[p.second] = 0;
            ++szMax; --szMin;
            pruneMax(); pruneMin();
        }
    }

    // Helpers to get a "clean" top (skip deleted)
    template <class Heap>
    pair<long long,long long> cleanTop(Heap& h) {
        prune(h);
        if (h.empty()) return {0, -1};
        return h.top();
    }

    template <class Heap>
    void popClean(Heap& h) {
        prune(h);
        if (!h.empty()) h.pop();
    }

    // Prune deleted ids from the top of a specific heap
    void pruneMax() { prune(maxHeap); }
    void pruneMin() { prune(minHeap); }

    template <class Heap>
    void prune(Heap& h) {
        while (!h.empty()) {
            auto id = h.top().second;
            if (deleted.count(id)) {
                h.pop();
                deleted.erase(id); // one-time cleanup
            } else {
                break;
            }
        }
    }
};

// ---------------------- Demo / quick tests ----------------------
int main() {

    {
        std::printf("Test 1 (W=3): push 1,5,2,10,-1\n");
        RollingMedian rm(3);
        vector<long long> seq = {1,5,2,10,-1};
        for (auto x : seq) {
            rm.push(x);
            std::printf(" push %lld -> median = %g\n", x, rm.median());
        }
        // Expected medians (with averaging): 1, 3, 2, 5, 2
        // (Depending on the spec, some interviews take the lower-of-two; here we average.)
    }

    {
        std::printf("\nTest 2 (W=4): duplicates\n");
        RollingMedian rm(4);
        for (int i = 0; i < 5; ++i) {
            rm.push(2);
            std::printf(" push 2 -> median = %g\n", rm.median());
        }
        // Always 2.0
    }

    {
        std::printf("\nTest 3 (W=2): extremes and overflow-safe average\n");
        RollingMedian rm(2);
        rm.push(std::numeric_limits<long long>::max());
        std::printf(" median = %g\n", rm.median());
        rm.push(std::numeric_limits<long long>::min());
        std::printf(" median = %g (should be around 0.5*(max+min) ~ -0.5)\n", rm.median());
        rm.push(0); // evicts the max
        std::printf(" median = %g (min & 0)\n", rm.median());
    }

    return 0;
}
