#pragma once

#include <chrono>
#include <iostream>

namespace Timer {

class Timer
{
public:
    Timer() : m_startPoint(std::chrono::high_resolution_clock::now())
    {
    }

    ~Timer()
    {
        auto endPoint = std::chrono::high_resolution_clock::now();

        auto start =
			std::chrono::time_point_cast<std::chrono::microseconds>
			(m_startPoint).time_since_epoch().count();
        auto end =
            std::chrono::time_point_cast<std::chrono::microseconds>
			(endPoint).time_since_epoch().count();

        auto duration = end - start;
        double ms     = duration * 0.001;

        std::cout << duration << "us (" << ms << "ms)\n";
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startPoint;
};

} // namespace Timer
