#include "copyright.h"
#include "system.h"
#include "EventBarrier.h"
#include "alarm.h"

// testnum is set in main.cc
int testnum = 1;

// AlarmTest used to test alarm, in this function we called Pause 
void AlarmTest(int arg){
	// alarm is declared in system.cc
    extern Alarm* alarm;
	// indicate that this thread has started
    printf("----AlarmTest with ID %d has started\n",arg);
	// ready to enter pause
    printf("----AlarmTest pause for 200 time units\n");
	// do pause
    alarm->Pause(200);
	// tell that pause has finished
    printf("----Alarmtest has finished pause 200 time units\n");
	// give up resource
    currentThread->Yield();
}

// dummy loop function, to make sure there is something running, or nachOS would halt
void loopFunction(int arg){
	// indicate that this thread has started
    printf("----LoopFunction with ID %d has started\n",arg);
	// we have to give up resource first, if we don't, then we would continue until this thread exit
    currentThread->Yield();
	// 15 times Yield 
    int time = 15;
    while(time > 0){
        time--;
        printf("thread exchange\n");
        currentThread->Yield();
    }

}


void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    // create two thread, to make result cleaner.
    Thread *t1 = new Thread("Thread 1");
    Thread *t2 = new Thread("Thread 2");

    t1->Fork(AlarmTest, 1);
    t2->Fork(loopFunction, 2);

    loopFunction(0);
}

//----------------------------------------------------------------------
// ThreadTest
//  Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
    ThreadTest1();
    break;

    default:
    printf("No test specified.\n");
    break;
    }
}