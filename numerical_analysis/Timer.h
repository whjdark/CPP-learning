#pragma once

#include <chrono>
#include <iostream>

class  Timer
{
public:
	Timer(const char* _info) 
		: info(_info), TimeStartPoint(std::chrono::high_resolution_clock::now()) {}
	~Timer()
	{
		auto TimeEndPoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(TimeStartPoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(TimeEndPoint).time_since_epoch().count();

		auto dur = end - start;
		double ms  = 0.001 * dur;

		std::cout << info << " took " << dur << "us(" << ms << "ms)" << "\n";
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> TimeStartPoint;
	const char* info;
};
