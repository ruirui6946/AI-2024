// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.


// deleted test used before, to make this file clean. Only keep ThreadTest1() 
// to test event barrier.
#include "copyright.h"
#include "system.h"
#include "EventBarrier.h"

// testnum is set in main.cc
int testnum = 1;
// test num, used to test if event barrier worked.
int test = 0;

// event barrier
EventBarrier event("test event");
//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

// event wait test, modify test, to see if event barrier worked.
void eventTest(int num){
    // enter thread.
    printf("%s gets into running\n", currentThread->getName());
    // modify test
    test++;

    // wait for event
    event.Wait();
    // finished event, print out message
    printf("%s gets into eventWaiting\n", currentThread->getName());
    // enter complete
    event.Complete();
    // finished complete, print out message
    printf("%s is completed\n", currentThread->getName());

    // after passing event barrier, print out current value for test
    printf("current test is %d\n", test);
    currentThread->Yield();
}

// eventWake used to signal
void eventWake(int num){
    // give up currnet resource, since this will be the first thread to run
    currentThread->Yield();
    event.Signal();
    //currentThread->Yield();
}

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    // create two thread, to make result cleaner.
    Thread *t1 = new Thread("Thread 1");
    Thread *t2 = new Thread("Thread 2");
    //Thread *t3 = new Thread("signal thread 3");

    t1->Fork(eventTest, 1);
    t2->Fork(eventTest, 2);
    //t3->Fork(eventWake, 3);

    eventWake(0);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
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

