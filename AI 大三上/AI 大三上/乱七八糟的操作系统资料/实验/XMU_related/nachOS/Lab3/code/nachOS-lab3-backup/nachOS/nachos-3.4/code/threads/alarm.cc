// alarm.cc
// implementation for alarm.h
// 
// By ForenewHan at 2018.6.13

#include "alarm.h"
#include "stats.h"
#include "thread.h"
#include "interrupt.h"

// since these variables are all staic variables, 
// we cannot initialize them in constructor, so we 
// initialize them here
int Alarm::waitingTime = 0;
int Alarm::waitingNum = 0;
Lock* Alarm::lock = new Lock("alarm lock");
List* Alarm::waitQueue = new List;


Alarm::Alarm(){
	waitingTime = 0;		// re-initialize these variables as 0
	waitingNum = 0;
	timer = new Timer(Wake, 0, 0);		// register a timer
}

Alarm::~Alarm(){
	delete timer;
	delete lock;
	delete waitQueue;
}

// Pause function , pause for a timePeriod
void Alarm::Pause(int timePeriod){		
	waitingTime = timePeriod;		// set waiting time as timePeriod

	if(waitingTime >= 0){		// if waiting time is greater than 0, then keep thread state and put it into sleep
		waitingNum++;
		lock->Acquire();		// enforce mutual exclusive access to the list 
		waitQueue->Append((void *)currentThread);   // so go to sleep
	    lock->Release();

	    IntStatus oldLevel = interrupt->SetLevel(IntOff);   // disable interrupts
	    currentThread->Sleep();             // if we want to make thread sleep, we have to disable ints
	    (void) interrupt->SetLevel(oldLevel);   // re-enable interrupts
	}

	printf("Pause Function has finished\n");
}

// Static function wake, used in Timer, as IRQ service
void Alarm::Wake(int dummy){

	extern Interrupt *interrupt;			// interrupt status, decleared in system.cc

	// every time Wake is called, we would decrease waitingTime.
    waitingTime -= TimerTicks;
	// print information, tell that it's another TimerTicks passed
    printf("\n----time goes by %d-----\n\n", TimerTicks);
	// if thread still haven't finished waiting, then keep waiting
    if(waitingTime >= 0){
        printf("----keep waiting-----\n");
    }
    else{
		// if thread has finished waiting, take it from queue, put it on readyQueue
    	if(waitingNum>0){
    		Thread* threadtoRun = (Thread*)waitQueue->Remove();
        	scheduler->ReadyToRun(threadtoRun);
        	waitingTime = 0;
        	waitingNum--;
        	printf("\n-----sleeping thread is waken-------\n");
    	}
    }

    
}