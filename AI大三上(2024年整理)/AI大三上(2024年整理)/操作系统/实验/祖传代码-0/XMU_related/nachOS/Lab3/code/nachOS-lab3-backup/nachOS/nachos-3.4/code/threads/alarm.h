// alarm.h
// 
// using alarm to achieve time just, we could put a thread to sleep 
// or wake it up at some certain point.
//
// contain two functions: 
// 1. alarm::Pause(int timePeriod), to sleep for a timePeriod
// 2. alarm::Wake(int dummy), used as IRQ inside timer object, to 
//    wake up those thread that have slept enough.
// 
// By ForenewHan at 2018.6.13

#ifndef ALARM_H
#define ALARM_H

#include "timer.h"
#include "synch.h"
#include "list.h"
#include "system.h"

class Alarm{
public:
	Alarm();
	~Alarm();

	// Pause function, called by thread, to sleep for a timePeriod
	void Pause(int timePeriod);

	// Wake function as VoidFunctionPtr to pass into timer
	// due to VoidFunctionPtr constraint, it has to be static function
	static void Wake(int dummy);

  private:
  	Timer *timer;
	
	// since static function can only access static data, so we declare 
	// these variables as static
  	static int waitingNum;		// used to record waiting numbers for pause threads
  	static int waitingTime;		// used to record waiting time for waiting thread
  	static Lock* lock;			// lock used to put pausing thread into queue
  	static List* waitQueue;		// waitingQueue, used for pausing threads storage
};



#endif

