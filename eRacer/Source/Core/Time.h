
#include <windows.h>

struct Time
{
	static const long long RESOLUTION = 1000000;
	long long current;
	long long delta;


	Time() : current(0), delta(0) {}
	long long Tick() 
	{
		LARGE_INTEGER clockSpeed;
		LARGE_INTEGER tick;
		QueryPerformanceFrequency(&clockSpeed);
		QueryPerformanceCounter(&tick);
		delta = (long long)(RESOLUTION * tick.QuadPart / clockSpeed.QuadPart);
		current += delta;
		return delta;
	}
};