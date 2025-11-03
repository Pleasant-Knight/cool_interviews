#include "robot_embedded.hpp"
#include <cstring>

namespace re {

std::size_t UartDmaTx::write(const std::uint8_t* data, std::size_t n) {
    if (!a_in_use_.load(std::memory_order_acquire)) {
        std::size_t w = n > CHUNK ? CHUNK : n;
        std::memcpy(bufA_, data, w);
        a_in_use_.store(true, std::memory_order_release);
        if (try_kick_(bufA_, w)) return w;
        a_in_use_.store(false, std::memory_order_release);
    }
    if (!b_in_use_.load(std::memory_order_acquire)) {
        std::size_t w = n > CHUNK ? CHUNK : n;
        std::memcpy(bufB_, data, w);
        b_in_use_.store(true, std::memory_order_release);
        if (try_kick_(bufB_, w)) return w;
        b_in_use_.store(false, std::memory_order_release);
    }
    return 0; // busy: caller can retry later
}

} // namespace re
