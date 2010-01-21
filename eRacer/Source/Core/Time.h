
#ifndef TIME_H_
#define TIME_H_

#include <windows.h>

struct Time
{
	static const long long RESOLUTION = 1000000;
	long long start;
	long long current;
	long long elapsed;
	long long delta;
	
	Time() : start(GetTime()), current(GetTime()), delta(0) {  }

	static long long GetTime()
	{
		LARGE_INTEGER clockSpeed;
		LARGE_INTEGER tick;
		QueryPerformanceFrequency(&clockSpeed);
		QueryPerformanceCounter(&tick);
		return (long long)(RESOLUTION * tick.QuadPart / clockSpeed.QuadPart);
	}

	long long Tick() 
	{
		long long time = GetTime();
		delta = time - current;
		elapsed = time - start;
		current = time;
		return delta;
	}
};


#endif //TIME_H_