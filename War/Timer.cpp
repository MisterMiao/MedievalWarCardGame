#include "Engine.h"

Timer::Timer() {
	startTime = 0;
	pauseTime = 0;
	runningTime = 0;
	started = false;
	paused = false;
	reverse = false;
}

void Timer::Start(int runningTime) {
	started = true;
	paused = false;
	startTime = SDL_GetTicks();
	pauseTime = 0;
	this->runningTime = runningTime;
	if (runningTime != 0)
		reverse = true;
	else
		reverse = false;
}

void Timer::Stop() {
	started = false;
	paused = false;
}

void Timer::Pause() {
	if (started && !paused) {
		paused = true;
		pauseTime = SDL_GetTicks() - startTime;
	}
}

void Timer::UnPause() {
	if (paused) {
		paused = false;
		startTime = SDL_GetTicks() - pauseTime;
		pauseTime = 0;
	}
}

int Timer::Restart() {
	int elapsedTime = Elapsed();
	Start();
	return elapsedTime;
}

int Timer::Elapsed() const {
	if (started)
		return paused ? pauseTime : SDL_GetTicks() - startTime;

	return 0;
}

int Timer::Remaining() const {
	int timeRan = runningTime - Elapsed();

	if (timeRan >= 0)
		return timeRan;
	else
		return -1; // The timer is over
}

bool Timer::Started() const {
	return started;
}

bool Timer::Paused() const {
	return paused;
}

string Timer::ToString() const {
	if (reverse && Remaining() == -1) // If the timer was running in reverse, Remaining() would equal -1 when the time is up
		return "Time Up!";

	int millis = !reverse ? Elapsed() : Remaining(); // If reverse == false, display elapsed time, otherwise display remaining time
	int hours = millis / (1000*60*60); // Hours from milliseconds
	int mins = (millis % (1000*60*60)) / (1000*60); // Minutes in an hour from milliseconds
	int secs = ((millis % (1000*60*60)) % (1000*60)) / 1000; // Seconds in a minute from milliseconds

	stringstream ss;
	if (hours >= 1) // Only show hours if the timer has surpassed an hour
		ss << (hours < 10 ? "0" : "") << hours << ":"; // If hours is a single digit, display it as two digits; "01 hours"

	ss << (mins < 10 ? "0" : "") << mins << ":";
	ss << (secs < 10 ? "0" : "") << secs;

	return ss.str();
}