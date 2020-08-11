#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	float Reset();
	float getElapsed() const;

private:
	std::chrono::steady_clock::time_point last;
};

//credit ChiliTomatoNoodle
