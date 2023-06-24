#pragma once

/**
 * Michel Hanna: 2020
 * Generic Timer class to measure the performance of the code.
 * Instantiate an object from Timer at the begining of you code, and at
 * the end it will print the executed time.
 * You can measure any code block by just put it inside a scope, i.e., "{ }".
 */

#include <iostream>
#include <ctime>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <numeric>
#include <cmath>
#include <atomic>
#include "timer.h"

class Timer {
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;

public:
	// the constructor will start the timer
	Timer() {
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}

	// The destructor will call Stop()
	~Timer() {
		Stop();
	}

	// Calculates the execution time
	void Stop();
};
