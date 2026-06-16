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

#include "copyright.h"
#include "system.h"
#include "dllist.h"

// testnum is set in main.cc
int testnum = 2;

// dl is doubly linked list, N is items we store in dl
DLList dl;
const int N = 10;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

// my own threadtest, used for doubly linke list test, called by ThreadTest2()
// arguemnt t is used to indicate current thread number
void threadDelTest(int t){
    int key;

    for(int i=0;i<N/2;i++){
        dl.Remove(&key);
        printf("thread %d remove at %d\n", t, key);
        currentThread->Yield();
    }
}


//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

// threadtest2, used to test doubly linkd list, structure declared in dllist.h
void
ThreadTest2()
{
    DEBUG('t', "Entering ThreadTest2");

    Thread *t = new Thread("forked thread");

    printf("getItems");
    // generate N random keys for dl
    genItems(&dl, N);

    printf("\n---------------------------\n");
    // fork a new thread to do threadDelTest, allocate thread number as 0
    t->Fork(threadDelTest, 0);
    // do threadDelTest, allocate thread number as 1
    threadDelTest(1);
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
    case 2:
    ThreadTest2();
    break;
    default:
	printf("No test specified.\n");
	break;
    }
}

