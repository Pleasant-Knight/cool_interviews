#ifndef TIME_FORMATTER_H
#define TIME_FORMATTER_H

#include <chrono>
#include <string>
#include <ctime>

namespace time_utils {

std::string format_steady_clock_to_string(const std::chrono::steady_clock::time_point& tp) {
    // Convert steady_clock time point to system_clock time point (approximation)
    auto now_system = std::chrono::system_clock::now();
    auto now_steady = std::chrono::steady_clock::now();
    auto steady_duration = tp - now_steady;
    auto system_tp = now_system + steady_duration;

    // Convert to time_t for formatting
    auto time_t_val = std::chrono::system_clock::to_time_t(system_tp);

    // Format into string
    char buffer[64];
    if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time_t_val))) {
        return std::string(buffer);
    } else {
        std::printerr("Error formatting time\n");
        return std::string("Invalid time");
    }
}

} // namespace time_utils

#endif // TIME_FORMATTER_H