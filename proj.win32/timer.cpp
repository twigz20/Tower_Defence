/*#include "timer.h"
/*
start();  starts the clock running from the current time
currtime();  returns the current time
stop();  stops the clock without reseting its value
reset();  stops the clock and resets the timer to 0
*/

/*Timer::Timer() {
	starttime_ = 0;
	isstop_ = 1;
	stoptime_ = 0;
}
void Timer::reset() {
	stoptime_ = 0;
	starttime_ = 0;
	isstop_ = 1;
}
void Timer::start() {
	clock_t curr = clock();
	if (isstop_) {
		starttime_ = (stoptime_ == 0) ? curr : starttime_ + (curr - stoptime_);
	}
	else {
		starttime_ = curr;   //its like hitting reset and start
	}
}
void Timer::stop() {
	stoptime_ = clock();
	isstop_ = 1;
}
double Timer::currtime() {
	clock_t curr = clock();
	clock_t elapse = (isstop_) ? stoptime_ - starttime_ : curr - starttime_;
	return double(elapse) / CLOCKS_PER_SEC;
}
double Timer::starttime() {
	return (double)starttime_;
}*/

#include "timer.h"

timer::timer()
{
	startedAt = 0;
	pausedAt = 0;
	paused = false;
	started = false;
}
bool timer::IsStarted()
{
	return started;
}
bool timer::IsStopped()
{
	return !started;
}
bool timer::IsPaused()
{
	return paused;
}
bool timer::IsActive()
{
	return !paused & started;
}
void timer::Pause()
{
	if (paused || !started)
		return;
	paused = true;
	pausedAt = clock();
}
void timer::Resume()
{
	if (!paused)
		return;
	paused = false;
	startedAt += clock() - pausedAt;
}
void timer::Stop()
{
	started = false;
}
void timer::Start()
{
	if (started)
		return;
	started = true;
	paused = false;
	startedAt = clock();
}
void timer::Reset()
{
	paused = false;
	startedAt = clock();
}
clock_t timer::GetTicks()
{
	if (!started)
		return 0;
	if (paused)
		return pausedAt - startedAt;
	return clock() - startedAt;
}