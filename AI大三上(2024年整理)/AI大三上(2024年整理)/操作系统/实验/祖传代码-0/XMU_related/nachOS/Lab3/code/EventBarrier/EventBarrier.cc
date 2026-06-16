// EventBarrier.cc
// implementation for eventbarrier
// By ForenewHan
// 2018.5.30

#include "EventBarrier.h"

EventBarrier::EventBarrier(char* eventname){
	eventName = eventname;
	state = unsignaled;
	waitingNum = 0;
	maxWait = 0;
	completeNum = 0;
	lock = new Lock("event barrier lock");
	pendThread = new Condition("pending Thread");
	finishThread = new Condition("finish Thread");
}

EventBarrier::~EventBarrier(){
	delete lock;
	delete pendThread;
	delete finishThread;
}

// if now we are in signaled state, just pass, or add to waitNum
void EventBarrier::Wait(){
	lock->Acquire();
	// actually state will never be signaled, because Signal() always 
	// turn off signal after wake up waiting threads
	if(state == signaled){
		lock->Release();
	}
	else{
		waitingNum++;
		pendThread->Wait(lock); 
		lock->Release();
	}
}

// mark waitingNum, wake up all waiting thread, then lock the signal again
void EventBarrier::Signal(){
	lock->Acquire();
	maxWait = waitingNum;
	state = signaled;
	while(waitingNum != 0){
		printf("signal waiting thread\n");
		pendThread->Signal(lock);
		waitingNum--;
	}
	state = unsignaled;
	lock->Release();
}

// used to make sure all threads have finished waiting
void EventBarrier::Complete(){
	lock->Acquire();
	// if this is the last thread to call this function
	while(completeNum != (maxWait-1)){
		completeNum++;
		finishThread->Wait(lock);
		return;
	}
	// then wake up all sleeping threads
	finishThread->Broadcast(lock);
	lock->Release();
}

