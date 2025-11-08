/**
 * 1. LRU Cache Implementation:
 * Problem: Design and implement a Least Recently Used (LRU) cache that supports get(key) and put(key, value) 
 * operations in O(1) time.
 * Evict the least recently used item when the cache reaches capacity.
 * Constraints: Capacity up to 10^4, keys/values are integers (1 ≤ key, value ≤ 10^5).
 * C++ Focus: Use std::unordered_map for O(1) lookups and std::list for order tracking (doubly-linked list). Avoid arrays; handle iterator invalidation.
 * Time to Solve: 20-30 minutes.
 * Follow-up: Optimize for multi-threaded access (e.g., using std::mutex).
 * Sample Input/Output:
 *  textLRUCache cache(2);
 *  cache.put(1, 1);  // Cache: {1=1}
 *  cache.put(2, 2);  // Cache: {1=1, 2=2}
 *  cache.get(1);     // Returns 1, Cache: {2=2, 1=1}
 *  cache.put(3, 3);  // Evicts 2, Cache: {1=1, 3=3}
 *  cache.get(2);     // Returns -1
 *  cache.put(4, 4);  // Evicts 1, Cache: {3=3, 4=4}
 *  cache.get(1);     // Returns -1
 *
 *** Questions to Ask:
    * - What should happen if get() is called with a non-existent key?
    * - Should put() update the value if the key already exists?
    * - How should the cache behave when it reaches its capacity?
    * - Are there any constraints on the size of keys and values?
    * - Is there a thread-safety or a memory-saftey requirement for this implementation?
    * I will say: “Just to make sure I’m aligned:
    *   We need get(key) and put(key, value) in O(1) time.
    *   Capacity is fixed; when full, evict least recently used item.
    *   Keys and values are integers.
    *   Should support fast access and track usage order.
    *
    * Explanation:
    * The LRU Cache maintains a fixed capacity. When inserting a new key-value pair, if the cache is full,
    * it evicts the least recently used item. The get operation retrieves the value for a given key and marks it as recently used.
    *
    * Implementation uses an unordered_map for O(1) access to cache items and a doubly-linked list to track the order of usage.
    * The most recently used items are at the front of the list, and the least recently used items are at the back.
    * Both get and put operations update the order of items in the list to reflect recent usage.
    * Follow-up:
    * To make the cache thread-safe, we can use a std::mutex to protect access to the cache data structures.
    * This ensures that concurrent accesses do not lead to data races or inconsistent states.
    *
 */
#include <cstdio>
#include <cstddef>
#include <unordered_map>
#include <list>
#include <utility>


constexpr int INVALID_VALUE = -1;
constexpr int INVALID_KEY = -1;
constexpr size_t DEFAULT_CAPACITY = 128; //2^7

struct CacheNode {
    int key;
    int value;
    CacheNode(int k, int v) : key(k), value(v) {}
    std::pair<int, int> to_pair() const {
        return {key, value};
    }
};

class LRUCache {
public:
    LRUCache(int capacity) : capacity_(static_cast<size_t>(capacity)) {}

    int get(int key) {
        auto it = cache_map_.find(key);
        if (it == cache_map_.end()) {
            return -1; // Key not found
        }
        // Move the accessed item to the front of the list (most recently used)
        cache_list_.splice(cache_list_.begin(), cache_list_, it->second);
        return it->second->value; // Return the value
    }

    void put(int key, int value) {
        auto it = cache_map_.find(key);
        if (it != cache_map_.end()) {
            // Key exists, update value and move to front
            it->second->value = value;
            cache_list_.splice(cache_list_.begin(), cache_list_, it->second);
            return;
        }
        // Key does not exist, insert new key-value pair
        if (cache_list_.size() == capacity_) {
            // Cache is full, evict least recently used item
            int lru_key = cache_list_.back().key;
            cache_list_.pop_back();
            cache_map_.erase(lru_key);
        }
        // Insert new item at the front of the list
        cache_list_.emplace_front(key, value);
        cache_map_[key] = cache_list_.begin();
    }
private:
    size_t capacity_;
    std::list<CacheNode> cache_list_;
    std::unordered_map<int, std::list<CacheNode>::iterator> cache_map_;
};

// Example usage

int main() {
    LRUCache cache(2); // Capacity of 2

    cache.put(1, 1);  // Cache: {1=1}
    cache.put(2, 2);  // Cache: {1=1, 2=2}
    int val1 = cache.get(1);     // Returns 1, Cache: {2=2, 1=1}
    std::printf("Get(1) = %d\n", val1);
    cache.put(3, 3);  // Evicts 2, Cache: {1=1, 3=3}
    int val2 = cache.get(2);     // Returns -1 (not found)
    std::printf("Get(2) = %d\n", val2);
    cache.put(4, 4);  // Evicts 1, Cache: {3=3, 4=4}
    int val3 = cache.get(1);     // Returns -1 (not found)
    std::printf("Get(1) = %d\n", val3);

    std::printf("Get(3) = %d\n", cache.get(3)); // Returns 3
    std::printf("Get(4) = %d\n", cache.get(4)); // Returns 4
    return 0;
}
