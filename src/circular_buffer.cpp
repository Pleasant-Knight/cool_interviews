#include "circular_buffer.hpp"
#include <thread>
#include <cstdio>
#include <chrono>
/*
* In robitics applications, lock-free data structures are often used to ensure real-time performance and low latency.
* A common scenario is the communication between sensor data acquisition threads and processing threads.
* For example, a high-frequency IMU (Inertial Measurement Unit) sensor might produce data at a rapid rate, 
* which needs to be consumed by a processing thread for tasks such as state estimation or control.
* The SimpleSpscRingBuffer implemented here is ideal for such use cases, as it allows a single producer thread 
* (the sensor data acquisition) to push data into the buffer, while a single consumer thread (the processing unit) pops data from it.
* This design minimizes latency and maximizes throughput, as the producer and consumer can operate concurrently without blocking each other.
* The non-blocking nature of the buffer ensures that if the consumer is not ready to process data, the producer can continue to push new data,
* potentially overwriting the oldest data if the buffer is full. 
* This is particularly useful in scenarios where the most recent data is more relevant than older data.
* Overall, using a lock-free SPSC ring buffer in robotics applications helps maintain the responsiveness and efficiency of the system,
* which is crucial for real-time operations.
 */

 /*
 * An IMU is a common sensor in robotics that measures acceleration and angular velocity.
 * It stands for Inertial Measurement Unit and typically consists of accelerometers and gyroscopes.
 */
struct Imu {
    uint64_t t; float ax, ay, az, gx, gy, gz;
};

int main() {
    SimpleSpscRingBuffer<Imu, 1024> q;
    std::atomic<bool> run{true};

    std::thread prod([&]{
        uint64_t t = 0;
        while (run.load()) {
            Imu s{t,0.1f,0.2f,0.3f,1.0f,1.1f,1.2f};
            q.try_push(std::move(s));     // drop on full
            t += 1000000;
            std::this_thread::sleep_for(std::chrono::microseconds(200));
        }
    });

    std::thread cons([&]{
        for (int i=0;i<20;++i) {
            if (auto s = q.try_pop()) {
                std::printf("t=%llu ax=%.2f gx=%.2f\n",
                    (unsigned long long)s->t, s->ax, s->gx);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        run.store(false);
    });

    cons.join();
    prod.join();
}
