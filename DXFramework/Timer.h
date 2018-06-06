// timer class
// Calculates frame/delta time and FPS.

#ifndef _TIMER_H_
#define _TIMER_H_

#include <windows.h>

class Timer
{
public:
	Timer();
	~Timer();

	void frame();

	float getTime();
	float getFPS();

private:
	INT64 frequency;
	float ticksPerS;
	INT64 startTime;
	float frameTime;
	float fps;
	float frames;
	float elapsedTime;
};

#endif