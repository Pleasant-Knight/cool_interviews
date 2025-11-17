/*
* 
* Name: Streaming Percentiles with Bounded Memory
* Theme: infra for live robot metrics (“p95 latency”) during deployments.
* Task:
* - Design and implement a structure to estimate the 95th percentile of a stream of double values using sublinear space. 
* - Provide:

class P2Quantile {  // or GK sketch; your choice
public:
  explicit P2Quantile(double q = 0.95);
  void observe(double x);
  double estimate() const;
};

* Constraints:
- O(1) per update; O(1) space for P^2 (or O(log n) for GK if you prefer fixed-epsilon).
-  Discuss error bounds briefly.
* 
*/

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <vector>
#include <cmath>

class P2Quantile {
public:
  explicit P2Quantile(double q = 0.95) : q_(q) {
    if (!(q > 0.0 && q < 1.0)) throw std::invalid_argument("q must be in (0,1)");
  }

  void observe(double x) {
    if (count_ < 5) { init_.push_back(x); ++count_; if (count_ == 5) init(); return; }
    ++count_;

    // 1) Find cell k where x falls; update endpoints
    int k;
    if (x < q_[0]) { q_[0] = x; k = 0; }
    else if (x >= q_[4]) { q_[4] = x; k = 3; }
    else {
      for (k = 0; k < 4; ++k) {
        if (x < q_[k+1]) break;
      }
    }

    // 2) Increment positions of markers to the right
    for (int i = k + 1; i < 5; ++i) n_[i]++;

    // 3) Desired positions move by fixed deltas
    for (int i = 0; i < 5; ++i) np_[i] += dn_[i];

    // 4) Adjust inner markers 2..4 (indices 1..3)
    for (int i = 1; i <= 3; ++i) {
      double d = np_[i] - n_[i];
      int s = (d >= 1.0) ? +1 : (d <= -1.0 ? -1 : 0);
      if (s == 0) continue;
      if ((n_[i + s] - n_[i]) <= 0) continue; // guard

      // Parabolic prediction
      double n_im1 = n_[i-1], n_i = n_[i], n_ip1 = n_[i+1];
      double q_im1 = q_[i-1], q_i = q_[i], q_ip1 = q_[i+1];

      double a = (q_ip1 - q_i) / (n_ip1 - n_i);
      double b = (q_i - q_im1) / (n_i - n_im1);
      double q_par = q_i + s * ((n_i - n_im1 + s) * a + (n_ip1 - n_i - s) * b) / (n_ip1 - n_im1);

      // Accept parabolic if within bracket; else linear
      if (q_par > q_im1 && q_par < q_ip1) {
        q_[i] = q_par;
      } else {
        q_[i] = q_i + s * ((q_[i + s] - q_i) / (n_[i + s] - n_i));
      }
      n_[i] += s;
    }
  }

  double estimate() const {
    if (count_ == 0) return std::numeric_limits<double>::quiet_NaN();
    if (count_ <= 5) {
      // exact quantile of the small init set
      auto tmp = init_;
      std::sort(tmp.begin(), tmp.end());
      size_t idx = static_cast<size_t>(std::floor(q_ * (tmp.size() - 1) + 0.5));
      if (idx >= tmp.size()) idx = tmp.size() - 1;
      return tmp[idx];
    }
    return q_[2]; // center marker
  }

  std::size_t count() const { return count_; }
  void reset() {
    count_ = 0;
    init_.clear();
    // leave dn_ untouched (depends only on q_)
    inited_ = false;
  }

private:
  void init() {
    std::sort(init_.begin(), init_.end());
    for (int i = 0; i < 5; ++i) q_[i] = init_[i];
    // initial positions for 5 samples
    for (int i = 0; i < 5; ++i) n_[i] = i + 1;
    // desired positions for i=5 (Jain & Chlamtac 1985)
    np_[0] = 1.0;
    np_[1] = 1.0 + 2.0 * q_;
    np_[2] = 1.0 + 4.0 * q_;
    np_[3] = 3.0 + 2.0 * q_;
    np_[4] = 5.0;

    // per-sample increments
    dn_[0] = 0.0;
    dn_[1] = q_ / 2.0;
    dn_[2] = q_;
    dn_[3] = (1.0 + q_) / 2.0;
    dn_[4] = 1.0;

    inited_ = true;
  }

  double q_;                 // target quantile in (0,1)
  std::size_t count_ = 0;
  bool inited_ = false;

  // P² state
  std::array<double,5> q_ { };     // marker heights (q1..q5)
  std::array<double,5> n_ { };     // marker positions
  std::array<double,5> np_{ };     // desired positions
  std::array<double,5> dn_{ };     // desired increments per sample

  // init buffer
  std::vector<double> init_;
};

// --- Tiny demo ---
int main() {
  P2Quantile p95(0.95);

  // Build a test stream with known heavy-tail-ish behavior
  std::vector<double> data;
  data.reserve(1000);
  for (int i = 0; i < 900; ++i) data.push_back(i * 1.0);     // 0..899
  for (int i = 0; i < 100; ++i) data.push_back(1000 + i*10); // outliers 1000..1990

  // Shuffle to simulate streaming disorder
  std::mt19937 rng(123);
  std::shuffle(data.begin(), data.end(), rng);

  for (double x : data) p95.observe(x);

  // Compare to exact 95th percentile (nearest-rank style)
  auto exact = data; std::sort(exact.begin(), exact.end());
  std::size_t k = static_cast<std::size_t>(std::ceil(0.95 * exact.size())) - 1;
  if (k >= exact.size()) k = exact.size() - 1;

  std::cout << "P2 estimate p95: " << p95.estimate() << "\n";
  std::cout << "Exact p95 (nearest-rank): " << exact[k] << "\n";
  std::cout << "Count: " << p95.count() << "\n";
  return 0;
}

