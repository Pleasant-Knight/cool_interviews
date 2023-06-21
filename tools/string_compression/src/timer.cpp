/**
 * Generic timer class to measure performance (n# cycles)
 * of a code block.
 * Michel Hanna: 2020.
 *
 */
#include "timer.h"

// Calculates the execution time
void Timer::Stop() {
	auto endTimePoint = chrono::high_resolution_clock::now();
	auto start = chrono::time_point_cast<chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
	auto end = chrono::time_point_cast<chrono::microseconds>(endTimePoint).time_since_epoch().count();

	auto duration = end - start;
	double ms = duration * 0.001;

	cerr << "\n\nV1 Run in " << duration << " usec (" << ms << " msec).\n";
}
