#ifndef TIMER_H
#define TIMER_H
#include <ctime>

class SimpleTimer
{
	clock_t startedAt;
	clock_t pausedAt;
	bool started;
	bool paused;
public:
	SimpleTimer();
	bool IsStarted();
	bool IsStopped();
	bool IsPaused();
	bool IsActive();
	void Pause();
	void Resume();
	void Stop();
	void Start();
	void Reset();
	clock_t GetTicks();
};

#endif