#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	float Mark();
	float Peek() const;

	void Reset() { last.time_since_epoch(); }
private:
	std::chrono::steady_clock::time_point last;
};

//credit ChiliTomatoNoodle
