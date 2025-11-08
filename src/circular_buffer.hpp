/*
* Real-Time Concurrency and Data Structures: 
* The Problem: Design and implement a thread-safe, fixed-size Circular Buffer (or Ring Buffer) in C++. 
* This buffer will be used to transfer high-rate sensor data (e.g., IMU readings) from an embedded driver thread 
* to a control loop thread that runs at a different frequency.
*
* Coding Challenge: Implement the push() and pop() methods, ensuring they are non-blocking and thread-safe using C++ 
* primitives (like std::mutex, std::condition_variable, or atomic operations).
*  The buffer should handle overflow by overwriting the oldest data when full.
*
* Follow-up (Systems): Explain the trade-offs between using a mutex-protected buffer versus 
a lock-free implementation (e.g., using std::atomic). Where in the robotics stack is a lock-free design mandatory?
*/


#pragma once
#include <array>
#include <atomic>
#include <cstddef>
#include <optional>
#include <type_traits>
#include <utility>

// Simple, fast SPSC (single-producer/single-consumer) ring buffer.
// - Single producer thread calls try_push.
// - Single consumer thread calls try_pop.
// - Non-blocking: returns false / std::nullopt instead of waiting.
// - Capacity must be a power of two (bitmask indexing).
template <typename T, std::size_t CapacityPow2>
class SimpleSpscRingBuffer {
    static_assert(CapacityPow2 >= 2, "Capacity must be >= 2");
    static_assert((CapacityPow2 & (CapacityPow2 - 1)) == 0,
                  "Capacity must be a power of two");
    // Mask for wrapping indices.
    static constexpr std::size_t kMask = CapacityPow2 - 1;

public:
    SimpleSpscRingBuffer() = default;
    // Non-copyable and non-movable.
    SimpleSpscRingBuffer(const SimpleSpscRingBuffer&) = delete;
    SimpleSpscRingBuffer& operator=(const SimpleSpscRingBuffer&) = delete;

    // Returns false if full; never blocks.
    bool try_push(const T& v) {
        return emplace_impl(v);
    }
    bool try_push(T&& v) {
        return emplace_impl(std::move(v));
    }

    // Returns empty optional if empty; never blocks.
    std::optional<T> try_pop() {
        std::size_t tail = tail_.load(std::memory_order_relaxed);
        const std::size_t head = head_.load(std::memory_order_acquire);
        if (tail == head) return std::nullopt; // empty

        std::size_t i = tail & kMask;
        T out = std::move(buf_[i]);           // read/move the item
        tail_.store(tail + 1, std::memory_order_release);
        return out;
    }

    bool empty() const {
        const auto h = head_.load(std::memory_order_acquire);
        const auto t = tail_.load(std::memory_order_acquire);
        return h == t;
    }
    bool full() const {
        const auto h = head_.load(std::memory_order_acquire);
        const auto t = tail_.load(std::memory_order_acquire);
        return (h - t) >= CapacityPow2;
    }
    std::size_t size() const {
        const auto h = head_.load(std::memory_order_acquire);
        const auto t = tail_.load(std::memory_order_acquire);
        return h - t;
    }

private:
    template <class U>
    bool emplace_impl(U&& v) {
        std::size_t head = head_.load(std::memory_order_relaxed);
        const std::size_t tail = tail_.load(std::memory_order_acquire);
        if ((head - tail) >= CapacityPow2) return false; // full

        std::size_t i = head & kMask;
        buf_[i] = std::forward<U>(v);                    // write
        head_.store(head + 1, std::memory_order_release);
        return true;
    }

    // Padding on indices helps avoid false sharing (optional but nice).
    std::atomic<std::size_t> head_{0};
    std::atomic<std::size_t> tail_{0};
    std::array<T, CapacityPow2> buf_{};
};
