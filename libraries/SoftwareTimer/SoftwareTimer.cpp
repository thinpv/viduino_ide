#include "SoftwareTimer.h"
#include "SoftwareTimerManager.h"

SoftwareTimer::SoftwareTimer() {
	initial_time = 0;
	current_time = 0;
	interval = 0;
	last_interval_time = 0;
	repeat_count = -1;
	is_running = false;
	is_paused = false;
	interval_is_setted = false;
	function_callback = NULL;
	TimerManager::instance().add(this);
}

SoftwareTimer::~SoftwareTimer() {
	TimerManager::instance().remove(this);
}

void SoftwareTimer::start() {
	if(isPaused()) {
		int paused_time = millis() - current_time;
		current_time = millis();
		initial_time += paused_time;
		last_interval_time += paused_time;
	} else {
		reset();
		repeat_count = total_repeat_count;
	}

	is_running = true;
	is_paused = false;
}

void SoftwareTimer::stop() {
	is_running = false;
	is_paused = false;
}

void SoftwareTimer::pause() {
	is_running = false;
	is_paused = true;
}

void SoftwareTimer::reset() {
	initial_time = millis();
	current_time = initial_time;
	last_interval_time = initial_time;
}

bool SoftwareTimer::isPaused() {
	return is_paused;
}

bool SoftwareTimer::isStopped() {
	return !is_paused && !is_running;
}

bool SoftwareTimer::isRunning() {
	return is_running;
}

unsigned long SoftwareTimer::getElapsedTime() {
	return current_time - initial_time;
}

void SoftwareTimer::update() {
	if(is_running) {
		current_time = millis();

		if(interval_is_setted) {
			if(current_time - last_interval_time >= interval) {
				if(repeat_count == 0) {
					stop();
					return ;
				} else if(repeat_count > 0) {
					repeat_count -= 1;
				}
				call();
				last_interval_time = current_time;

			}
		}
	}
}

void SoftwareTimer::setInterval(unsigned long interval, int repeat_count){
	this->interval = interval;
	this->repeat_count = repeat_count;
	total_repeat_count = repeat_count;
	interval_is_setted = true;
}

void SoftwareTimer::setTimeout(unsigned long timeout){
	setInterval(timeout, 1);
}

void SoftwareTimer::clearInterval() {
	repeat_count = -1;
}

void SoftwareTimer::call() {
	if(function_callback != NULL) {
		function_callback();
	}
}
