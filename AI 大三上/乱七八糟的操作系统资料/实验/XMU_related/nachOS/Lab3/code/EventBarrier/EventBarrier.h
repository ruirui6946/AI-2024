// EventBarrier.h
// head file for eventbarrier
// use the fact that thread are called by route, there would be no thread to grasp resource preferentially. 
// So we don't need to remember thread details, just use one count to remember.
// By ForenewHan 
// 2018.5.30

#ifndef EVENTBARRIER_H
#define EVENTBARRIER_H

#include "synch.h"

class EventBarrier {
	public:
		EventBarrier(char* eventname);
		~EventBarrier();
		
		// wait used to wait for specific event
		void Wait();
		// signal any thread that is waiting for event, just for current waiting thread
		// then signal would be turned off immediatly, event can be reused immediatly
		void Signal();
		// used after wait, means having finished event
		void Complete();
	
	private:
		// eventname, used for debugging
		char* eventName;
		// two states for event
		enum STATE {signaled, unsignaled} state;
		// used to record waiting thread number, since thread are called one by one, 
		// so here didn't record thread name
		int waitingNum;
		// maxwait, used for signal, to record current waiting threads
		int maxWait;
		// used in Complete(), to show if we have completed all threads
		int completeNum;
		// lock used for safety
		Lock *lock;
		// used to put waiting thread to sleep
		Condition *pendThread;
		// used to put finished thread to sleep
		Condition *finishThread;
};

#endif