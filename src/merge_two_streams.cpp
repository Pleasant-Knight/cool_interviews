/*
Task:
You receive two increasing timestamp streams A and B (ints). Implement an online merger:

class Merger {
public:
  // push maintains nondecreasing timestamps per source
  void pushA(int ts);
  void pushB(int ts);
  // pops the next global event if available
  std::optional<int> pop();
};


Requirements:
    pop() returns the smallest available timestamp, stable across equal times (A before B when ties).
    Amortized O(1) per push/pop; O(1) extra space beyond the queues holding unread data.
    Prove stability and discuss back-pressure handling.

  Questions to consider:
    What if one stream is much faster than the other?
    How to handle unbounded delays in one stream?
    Can you extend this to more than two streams?
*/

#include <deque>
#include <optional>
#include <stdexcept>
#include <cassert>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <iostream>
#include <chrono>
#include <atomic>
#include <algorithm>

class Merger_sngl { // single threaded version
public:
Merger_sngl::Merger_sngl() {}
  // push maintains nondecreasing timestamps per source
  void pushA(int ts);
  void pushB(int ts);
  // pops the next global event if available.
  std::optional<int> pop();

  bool empty() const noexcept { return A_queue.empty() && B_queue.empty(); }

  // Optional helpers for tests/metrics
  size_t sizeA() const noexcept { return A_queue.size(); }
  size_t sizeB() const noexcept { return B_queue.size(); }

private:
  std::deque<int> A_queue;
  std::deque<int> B_queue;

};


void Merger_sngl::pushA(int ts) {
  if (!A_queue.empty() && ts < A_queue.back()) {
    throw std::invalid_argument("Timestamps in stream A must be nondecreasing");
  }
  A_queue.push(ts);
}

void Merger_sngl::pushB(int ts) {
  if (!B_queue.empty() && ts < B_queue.back()) {
    throw std::invalid_argument("Timestamps in stream B must be nondecreasing");
  }
  B_queue.push(ts);
}

std::optional<int> Merger_sngl::pop() {
  if (A_queue.empty() && B_queue.empty()) {
    return std::nullopt; // No events available
  }
  
  if (A_queue.empty()) {
    int ts = B_queue.front();
    B_queue.pop();
    return ts;
  }
  
  if (B_queue.empty()) {
    int ts = A_queue.front();
    A_queue.pop();
    return ts;
  }
  
  // Both queues are non-empty, compare front elements
  if (A_queue.front() <= B_queue.front()) {
    int ts = A_queue.front();
    A_queue.pop();
    return ts;
  } else {
    int ts = B_queue.front();
    B_queue.pop();
    return ts;
  }
} 

// Multithreaded version with mutex and condition variable
class Merger {
public:
  // Non-blocking push: thread-safe
  void pushA(int ts) {
    std::unique_lock<std::mutex> lk(m_);
    // Enforce per-stream nondecreasing contract
    if (!a_.empty() && ts < a_.back()) {  
#ifndef NDEBUG
      assert(false && "A stream not nondecreasing");
#else
      throw std::invalid_argument("pushA: timestamps must be nondecreasing");
#endif
    }
    a_.push_back(ts);
    lk.unlock();
    cv_.notify_one();
  }

  void pushB(int ts) {
    std::unique_lock<std::mutex> lk(m_);
    if (!b_.empty() && ts < b_.back()) {
#ifndef NDEBUG
      assert(false && "B stream not nondecreasing");
#else
      throw std::invalid_argument("pushB: timestamps must be nondecreasing");
#endif
    }
    b_.push_back(ts);
    lk.unlock();
    cv_.notify_one();
  }

  // Non-blocking pop. Returns std::nullopt if no data.
  std::optional<int> pop() {
    std::lock_guard<std::mutex> lk(m_);
    if (a_.empty() && b_.empty()) return std::nullopt;

    if (b_.empty()) {
      int v = a_.front(); a_.pop_front(); return v;
    }
    if (a_.empty()) {
      int v = b_.front(); b_.pop_front(); return v;
    }

    // Both present: on tie take A to preserve stability
    if (a_.front() <= b_.front()) {
      int v = a_.front(); a_.pop_front(); return v;
    } else {
      int v = b_.front(); b_.pop_front(); return v;
    }
  }

  // Blocking pop with timeout. Returns true and sets `out` if an item was popped.
  template <class Rep, class Period>
  bool waitPop(int& out, const std::chrono::duration<Rep, Period>& timeout) {
    std::unique_lock<std::mutex> lk(m_);
    if (!cv_.wait_for(lk, timeout, [&]{ return !a_.empty() || !b_.empty(); }))
      return false; // timed out

    if (b_.empty()) {
      out = a_.front(); a_.pop_front(); return true;
    }
    if (a_.empty()) {
      out = b_.front(); b_.pop_front(); return true;
    }
    if (a_.front() <= b_.front()) {
      out = a_.front(); a_.pop_front(); return true;
    } else {
      out = b_.front(); b_.pop_front(); return true;
    }
  }

  bool empty() const {
    std::lock_guard<std::mutex> lk(m_);
    return a_.empty() && b_.empty();
  }

  // Optional helpers for metrics/testing
  size_t sizeA() const { std::lock_guard<std::mutex> lk(m_); return a_.size(); }
  size_t sizeB() const { std::lock_guard<std::mutex> lk(m_); return b_.size(); }

private:
  mutable std::mutex m_;
  std::condition_variable cv_;
  std::deque<int> a_;
  std::deque<int> b_;
};


// ---- Demo main ----
int main() {
  using namespace std::chrono_literals;

  Merger merger;
  std::atomic<bool> doneA{false}, doneB{false};
  std::vector<int> result;
  result.reserve(64);

  // Producer A: nondecreasing with a tie cluster
  std::thread prodA([&]{
    std::vector<int> A = {1, 3, 5, 5, 8, 12};
    for (int v : A) {
      std::this_thread::sleep_for(15ms);
      merger.pushA(v);
    }
    doneA = true;
  });

  // Producer B: interleaved with ties against A's 5s and 12
  std::thread prodB([&]{
    std::vector<int> B = {2, 4, 5, 9, 12, 13};
    for (int v : B) {
      std::this_thread::sleep_for(10ms);
      merger.pushB(v);
    }
    doneB = true;
  });

  // Consumer: block with timeout, stop when both producers done and queues drained
  std::thread consumer([&]{
    int x;
    while (true) {
      // Try to get next item; if timeout, check for termination
      if (merger.waitPop(x, 50ms)) {
        result.push_back(x);
      } else {
        if (doneA.load() && doneB.load() && merger.empty()) break;
      }
    }
  });

  prodA.join();
  prodB.join();
  consumer.join();

  // Print merged output
  std::cout << "Merged sequence (" << result.size() << " items): ";
  for (size_t i = 0; i < result.size(); ++i) {
    if (i) std::cout << ' ';
    std::cout << result[i];
  }
  std::cout << "\n";

  // Quick correctness checks: nondecreasing and stable tie (A before B on equal timestamps)
  bool nondecreasing = std::is_sorted(result.begin(), result.end());
  std::cout << "Nondecreasing: " << (nondecreasing ? "OK" : "FAIL") << "\n";

  // Stability spot-check: we know there were three 5s (A: two 5s; B: one 5).
  // The first two 5s should come from A before the B 5. We can check counts/order by replay:
  // For brevity, we just print the run of 5s:
  std::cout << "Run of 5s (expect A,A,B order): ";
  for (int v : result) if (v == 5) std::cout << "5 ";
  std::cout << "\n";

  return 0;
}