/**
* Write a C++ program to increment a global counter using two threads. 
* Thread A will increment the counter only if the counter is odd, and thread B will increment the counter if it's even.
* The maximum count should be 1000000 and the initial counter value is 0. 
* Use unsigned 64 bit integer for the counter and use std::mutex to lock and unlock the counter while updating it.
*/

#include <cstdint>
#include <iostream>
#include <thread>
#include <mutex>

std::mutex counterMutex;
uint64_t counter = 0;

static const uint64_t MAX_COUNT = (1024 * 1024);

void threadA() {
    while (counter < MAX_COUNT)   {
        if (counter % 2 == 1) {
            counterMutex.lock();
            if (counter < MAX_COUNT) {
                counter++;
            }
            else {
                std::cout << "Thread A: Counter = " << counter << "\n";
            }
            counterMutex.unlock();
        }
    }
}

void threadB() {
    while (counter < MAX_COUNT) {
        if (counter % 2 == 0)  {
            counterMutex.lock();
            if (counter < MAX_COUNT) {
                counter++;
            }
            else {
                std::cout << "Thread B: Counter = " << counter << "\n";
            }
            counterMutex.unlock();
        }
    }
}

int main() {
    uint64_t Count = 0;
    auto Increment = [&](int d) {
        while (Count < MAX_COUNT) {
            if (Count % 2 == d) {
                counterMutex.lock();
                if (Count < MAX_COUNT) {
                    Count++;
                }
                counterMutex.unlock();
            }
        }
    };
    std::cout << "Start global counter at: " << counter << "\n";
    
    std::thread tA(threadA);
    std::thread tB(threadB);
    
    tA.join();
    tB.join();
    std::cout << "Global Coutner now is: " << counter << "\n\n";

    std::cout << "\n\nStart local counter at: " << Count << "\n";

    std::thread thA(Increment, 1);
    std::thread thB(Increment, 0);

    thA.join();
    thB.join();
    std::cout << "Local Coutner now is: " << Count << "\n\n";
    
    return 0;
}
