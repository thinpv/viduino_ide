/*
	timerManager.h - A singleton that holds all the SoftwareTimer instances. It's used to
  update all timers at once

  Link - https://github.com/brunocalou/SoftwareTimer

	Created by Bruno Calou Alves, Apr, 2017.
	Read LICENSE for more information.
*/

#ifndef SOFTWARE_TIMER_MANAGER_H
#define SOFTWARE_TIMER_MANAGER_H

#include "Arduino.h"

#ifndef NULL
#define NULL 0
#endif

#include "SoftwareTimer.h"

class TimerManager {
  friend class SoftwareTimer;

public:
  static TimerManager& instance();

  /**
     Updates all timers
   */
  void update();

  /*
		Starts all timers
	*/
	void start();

	/*
		Stops and resets all timers
	*/
	void stop();

	/*
		Pauses all timers
	*/
	void pause();

	/*
		Resets elapsed time of all timers. If any timer was running, it will continue to do so
	*/
	void reset();

private:
  TimerManager();
  TimerManager(TimerManager const &);
  void operator=(TimerManager const &);

  /**
     Adds a timer to the TimerManager
   */
  void add(SoftwareTimer *timer);

  /**
     Removes a timer from the TimerManager
     @return True if the timer was removed, false otherwise
   */
  bool remove(SoftwareTimer *timer);

  class TimerNode {
  public:
    TimerNode(SoftwareTimer *timer) {
      next = NULL;
      value = timer;
    }
    TimerNode *next;
    SoftwareTimer *value;
  };

  TimerNode *first;
  TimerNode *last;
};
#endif //TIMERMANAGER_H
