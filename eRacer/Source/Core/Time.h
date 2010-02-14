
#ifndef TIME_H_
#define TIME_H_

#define NOMINMAX
#include <windows.h>

struct Time
{
	static const long long RESOLUTION = 1000000;
	long long start;
	long long current;
	long long game_total;
	long long wall_total;
	long long game_delta;
	long long wall_delta;
	
	Time() : 
		start(GetTime()), 
		current(GetTime()), 
		game_total(0),
		wall_total(0),
		game_delta(0),
		wall_delta(0)
		{  }

	static long long GetTime()
	{
		LARGE_INTEGER clockSpeed;
		LARGE_INTEGER tick;
		QueryPerformanceFrequency(&clockSpeed);
		QueryPerformanceCounter(&tick);
		return (long long)(RESOLUTION * tick.QuadPart / clockSpeed.QuadPart);
	}

	long long Tick(float speed=1.0)
	{
		long long time = GetTime();
		wall_delta = time - current;
		game_delta = (long long)(speed * wall_delta);
		wall_total += wall_delta;
		game_total += game_delta;
		current = time;
		return game_delta;
	}
};


#endif //TIME_H_