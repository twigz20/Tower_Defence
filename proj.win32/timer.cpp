#include "timer.h"

SimpleTimer::SimpleTimer()
{
	startedAt = 0;
	pausedAt = 0;
	paused = false;
	started = false;
}
bool SimpleTimer::IsStarted()
{
	return started;
}
bool SimpleTimer::IsStopped()
{
	return !started;
}
bool SimpleTimer::IsPaused()
{
	return paused;
}
bool SimpleTimer::IsActive()
{
	return !paused & started;
}
void SimpleTimer::Pause()
{
	if (paused || !started)
		return;
	paused = true;
	pausedAt = clock();
}
void SimpleTimer::Resume()
{
	if (!paused)
		return;
	paused = false;
	startedAt += clock() - pausedAt;
}
void SimpleTimer::Stop()
{
	started = false;
}
void SimpleTimer::Start()
{
	if (started)
		return;
	started = true;
	paused = false;
	startedAt = clock();
}
void SimpleTimer::Reset()
{
	paused = false;
	startedAt = clock();
}
clock_t SimpleTimer::GetTicks()
{
	if (!started)
		return 0;
	if (paused)
		return pausedAt - startedAt;
	return clock() - startedAt;
}