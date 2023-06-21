/**
 * Generic timer class to measure performance (n# cycles)
 * of a code block.
 * Michel Hanna: 2020.
 *
 */
#include "timer.h"

// Calculates the execution time
void Timer::Stop() {
	auto endTimePoint = std::chrono::high_resolution_clock::now();
	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

	auto duration = end - start;
	double ms = duration * 0.001;

	std::cerr << "\n\nV1 Run in " << duration << " usec (" << ms << " msec).\n";
}
