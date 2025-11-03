#pragma once
#include <cstdint>
#include <cstddef>
#include <atomic>

// -----------------------------------------------------------------------------
//  Minimal HAL (interfaces only)  — implement these on the target MCU
// -----------------------------------------------------------------------------
namespace re {

enum class GpioMode : uint8_t { In, Out, Alt, Analog };
enum class GpioPull : uint8_t { None, Up, Down };

struct IGpio {
    virtual ~IGpio() = default;
    virtual void config(uint32_t pin, GpioMode m, GpioPull p) = 0;
    virtual void write(uint32_t pin, bool hi) = 0;
    virtual bool read(uint32_t pin) const = 0;
};

struct IUartDma {
    virtual ~IUartDma() = default;
    virtual bool busy() const = 0;
    virtual void start_tx(const void* buf, std::size_t n) = 0;
    virtual void ack_complete_irq() = 0;   // call in ISR after completion
};

struct INvStorage {
    virtual ~INvStorage() = default;
    virtual bool erase_bank() = 0;
    virtual bool write(std::uint32_t offset, const void* data, std::size_t n) = 0;
    virtual bool mark_bank_ready(std::uint16_t crc16, std::uint32_t size) = 0; // bootloader reads this
    virtual bool read_counters(void* out, std::size_t n) = 0;
    virtual bool write_counters(const void* in, std::size_t n) = 0;
};

} // namespace re

// -----------------------------------------------------------------------------
//  Utilities
// -----------------------------------------------------------------------------
namespace re {

constexpr std::uint16_t crc16_ccitt(const void* data, std::size_t len, std::uint16_t crc = 0xFFFF) {
    const auto* p = static_cast<const std::uint8_t*>(data);
    while (len--) {
        crc ^= static_cast<std::uint16_t>(*p++) << 8;
        for (int i = 0; i < 8; ++i) {
            crc = (crc & 0x8000) ? static_cast<std::uint16_t>((crc << 1) ^ 0x1021)
                                 : static_cast<std::uint16_t>(crc << 1);
        }
    }
    return crc;
}

} // namespace re

// -----------------------------------------------------------------------------
//  LED driver (HAL + logic separation)
// -----------------------------------------------------------------------------
namespace re {

class LedDriver {
    IGpio& gpio_;
    std::uint32_t pin_;
public:
    LedDriver(IGpio& g, std::uint32_t pin) : gpio_(g), pin_(pin) {
        gpio_.config(pin_, GpioMode::Out, GpioPull::None);
    }
    void on()  { gpio_.write(pin_, true); }
    void off() { gpio_.write(pin_, false); }
};

} // namespace re

// -----------------------------------------------------------------------------
//  Single-Producer/Single-Consumer ring buffer for ISR->task (bytes)
//  - ISR pushes; task pops. Bounded, lock-free.
// -----------------------------------------------------------------------------
namespace re {

class SpscRing {
    std::uint8_t* buf_;
    std::size_t   cap_;                 // must be power-of-two
    std::atomic<std::size_t> head_{0};  // producer (ISR)
    std::atomic<std::size_t> tail_{0};  // consumer (task)
public:
    SpscRing(std::uint8_t* storage, std::size_t cap_pow2) : buf_(storage), cap_(cap_pow2) {}
    std::size_t capacity() const { return cap_; }

    // returns bytes actually written
    std::size_t push(const std::uint8_t* src, std::size_t n) {
        auto h = head_.load(std::memory_order_relaxed);
        auto t = tail_.load(std::memory_order_acquire);
        std::size_t free = cap_ - (h - t);
        if (n > free) n = free;
        for (std::size_t i = 0; i < n; ++i) buf_[(h + i) & (cap_ - 1)] = src[i];
        head_.store(h + n, std::memory_order_release);
        return n;
    }
    // returns bytes actually read
    std::size_t pop(std::uint8_t* dst, std::size_t n) {
        auto t = tail_.load(std::memory_order_relaxed);
        auto h = head_.load(std::memory_order_acquire);
        std::size_t avail = h - t;
        if (n > avail) n = avail;
        for (std::size_t i = 0; i < n; ++i) dst[i] = buf_[(t + i) & (cap_ - 1)];
        tail_.store(t + n, std::memory_order_release);
        return n;
    }
};

} // namespace re

// -----------------------------------------------------------------------------
//  UART DMA double-buffered TX (zero-copy across ISR boundary)
// -----------------------------------------------------------------------------
namespace re {

class UartDmaTx {
    IUartDma& hw_;
    static constexpr std::size_t CHUNK = 256;
    alignas(4) std::uint8_t bufA_[CHUNK]{}, bufB_[CHUNK]{};
    std::atomic<bool> a_in_use_{false}, b_in_use_{false};

    bool try_kick_(std::uint8_t* buf, std::size_t n) {
        if (n == 0 || hw_.busy()) return false;
        hw_.start_tx(buf, n);
        return true;
    }

public:
    explicit UartDmaTx(IUartDma& hw) : hw_(hw) {}

    // Copy into whichever buffer is free and start DMA if idle.
    // Returns bytes accepted for TX (<= CHUNK).
    std::size_t write(const std::uint8_t* data, std::size_t n);

    // Call from ISR on DMA completion:
    void on_dma_complete_irq() {
        hw_.ack_complete_irq();
        a_in_use_.store(false, std::memory_order_release);
        b_in_use_.store(false, std::memory_order_release);
    }
};

} // namespace re

// -----------------------------------------------------------------------------
//  Fixed-point PID (Q15 gains; bounded, anti-windup)
// -----------------------------------------------------------------------------
namespace re {

struct PidQ15 {
    std::int32_t kp{0}, ki{0}, kd{0}; // Q15 gains
    std::int32_t iacc{0};             // Q30 accumulator
    std::int16_t prev_err{0};
    std::int16_t out_min{-32768}, out_max{32767};

    static inline std::int16_t clamp16(std::int32_t x) {
        if (x > 32767) return 32767;
        if (x < -32768) return -32768;
        return static_cast<std::int16_t>(x);
    }

    std::int16_t step(std::int16_t setpoint, std::int16_t meas) {
        std::int16_t err = static_cast<std::int16_t>(setpoint - meas);
        std::int32_t pterm = (kp * err) >> 15;       // Q15
        iacc += (ki * err);                          // Q30
        std::int16_t i_q15 = static_cast<std::int16_t>(iacc >> 15);
        if (i_q15 > out_max) iacc = static_cast<std::int32_t>(out_max) << 15;
        if (i_q15 < out_min) iacc = static_cast<std::int32_t>(out_min) << 15;
        std::int16_t derr = static_cast<std::int16_t>(err - prev_err);
        prev_err = err;
        std::int32_t dterm = (kd * derr) >> 15;      // Q15
        std::int32_t out = pterm + (iacc >> 15) + dterm;
        if (out > out_max) out = out_max;
        if (out < out_min) out = out_min;
        return static_cast<std::int16_t>(out);
    }
};

} // namespace re

// -----------------------------------------------------------------------------
//  OTA updater (CRC-gated, resumable)
// -----------------------------------------------------------------------------
namespace re {

enum class OtaState : uint8_t { Idle, Recv, Validating, Ready, Applied, Error };

struct OtaCtx {
    OtaState      st{OtaState::Idle};
    std::uint32_t img_size{0};
    std::uint32_t received{0};
    std::uint16_t rolling_crc{0xFFFF};
};

class Ota {
    INvStorage& nv_;
    OtaCtx      ctx_;
public:
    explicit Ota(INvStorage& nv) : nv_(nv) {}
    bool begin(std::uint32_t expected_size) {
        ctx_ = {};
        ctx_.st = OtaState::Recv;
        ctx_.img_size = expected_size;
        ctx_.rolling_crc = 0xFFFF;
        ctx_.received = 0;
        return nv_.erase_bank();
    }
    bool write_chunk(const void* data, std::uint32_t n) {
        if (ctx_.st != OtaState::Recv || ctx_.received + n > ctx_.img_size) return false;
        if (!nv_.write(ctx_.received, data, n)) return false;
        ctx_.rolling_crc = crc16_ccitt(data, n, ctx_.rolling_crc);
        ctx_.received += n;
        if (ctx_.received == ctx_.img_size) ctx_.st = OtaState::Validating;
        return true;
    }
    bool finalize(std::uint16_t expected_crc) {
        if (ctx_.st != OtaState::Validating) return false;
        if (ctx_.rolling_crc != expected_crc) { ctx_.st = OtaState::Error; return false; }
        if (!nv_.mark_bank_ready(ctx_.rolling_crc, ctx_.img_size)) { ctx_.st = OtaState::Error; return false; }
        ctx_.st = OtaState::Ready; return true;
    }
    const OtaCtx& state() const { return ctx_; }
};

} // namespace re

// -----------------------------------------------------------------------------
//  Safety interlock (latched fault mirrored to HW e-stop line)
// -----------------------------------------------------------------------------
namespace re {

struct ISafetyHw {
    virtual ~ISafetyHw() = default;
    virtual void set_estop_line(bool asserted) = 0;
    virtual bool physical_reset_ok() const = 0;
};

class SafetyInterlock {
    ISafetyHw& hw_;
    std::atomic<int> latched_{0};
public:
    explicit SafetyInterlock(ISafetyHw& hw) : hw_(hw) {}
    void trip() {
        latched_.store(1, std::memory_order_release);
        hw_.set_estop_line(true);
    }
    void clear_if_ok() {
        if (hw_.physical_reset_ok()) {
            latched_.store(0, std::memory_order_release);
            hw_.set_estop_line(false);
        }
    }
    bool fault() const { return latched_.load(std::memory_order_acquire) != 0; }
};

} // namespace re

// -----------------------------------------------------------------------------
//  Boot breadcrumbs (reset reasons persisted for ops debugging)
// -----------------------------------------------------------------------------
namespace re {

enum ResetBits : std::uint32_t { RESET_UNKNOWN=0, RESET_WATCHDOG=1, RESET_BROWNOUT=2, RESET_SOFTWARE=4 };

struct BootCounters {
    std::uint32_t boots{0};
    std::uint32_t wd_resets{0};
    std::uint32_t brownouts{0};
    std::uint32_t last_reason{RESET_UNKNOWN};
};

class BootBreadcrumbs {
    INvStorage& nv_;
public:
    explicit BootBreadcrumbs(INvStorage& nv) : nv_(nv) {}
    void record_boot(std::uint32_t reason) {
        BootCounters c{};
        nv_.read_counters(&c, sizeof(c));
        c.boots++;
        c.last_reason = reason;
        if (reason & RESET_WATCHDOG) c.wd_resets++;
        if (reason & RESET_BROWNOUT) c.brownouts++;
        nv_.write_counters(&c, sizeof(c));
    }
};

} // namespace re
