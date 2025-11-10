#include <chrono>
#include <string>
#include <cstdio>

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

int main() {
    // Get current steady_clock time point
    auto now = std::chrono::steady_clock::now();
    
    // Format to human-readable string
    std::string time_str = format_steady_clock_to_string(now);
    std::printf("Current time: %s\n", time_str.c_str());

    // Example with a time point 5 seconds in the future
    auto future = now + std::chrono::seconds(5);
    time_str = format_steady_clock_to_string(future);
    std::printf("Future time (+5s): %s\n", time_str.c_str());

    return 0;
}