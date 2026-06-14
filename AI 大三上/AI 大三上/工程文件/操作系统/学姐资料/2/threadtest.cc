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
#include "BoundedBuffer.h"
#include "synch.h"
#include "time.h"

extern void InsertElement1(int n, DLList *dll, int which);
extern void DeleteElement1(int n, DLList *dll, int which);
extern void InsertElement2(int n, DLList *dll, int which);
extern void DeleteElement3(int n, DLList *dll, int which);

//----------------------------------------------------------------------
// SimpleThread1
// 	Empty Error: thread 1 tries to test if DLList is empty
//SimpleThread2
//	Insert Error: thread 1 override dllist head
//SimpleThread3
//	Delete Error: both thread believe dllist is not empty	
//----------------------------------------------------------------------
DLList* dll = new DLList();
Lock* lock = new Lock("lock");

Condition* canYield = new Condition("can Yield cond");
Lock* llock = new Lock("list lock");
bool isFinish = false;

BoundedBuffer* boundedbuffer = new BoundedBuffer(5);

void SimpleThread1(int which)
{
	lock->Acquire();
        printf("*** thread %d acquire lock ***\n", which);
	if (which == 1)
	{
        	bool ans = dll->IsEmpty();
		if (ans)
			printf("thread 1 check list is empty: true\n");
		else
			printf("thread 1 check list is empty: false\n");
	}
	else if (which == 0)
	{
		InsertElement1(1, dll, which);
	}
	lock->Release();
	printf("*** thread %d release lock ***\n", which);
}


void
SimpleThread2(int which)
{
    //srand(which * 233333333);
    lock->Acquire();
    printf("*** thread %d acquire lock ***\n", which);
    InsertElement2(1, dll, which);
    lock->Release();
    printf("*** thread %d release lock ***\n", which);    
}

void
SimpleThread3(int which)
{
    lock->Acquire();
    printf("*** thread %d acquire lock ***\n", which);
    if (which == 1)
    {
    		InsertElement1(1, dll, which);
		currentThread->Yield();
    		DeleteElement3(1, dll, which);
    }
    else if (which == 0)
    {
    		InsertElement1(1, dll, which);
		DeleteElement3(1, dll, which);
    }
    lock->Release();
    printf("*** thread %d release lock ***\n", which);
}

void
SimpleThread4(int which)
{		 	
	if (which == 1)
	{
        	llock->Acquire();
		while(dll->IsEmpty())
			canYield->Wait(llock);
		bool ans = dll->IsEmpty();
		if (ans)
			printf("thread 1 check list is empty: true\n");
		else
			printf("thread 1 check list is empty: false\n");
		llock->Release();
	}
	else if (which == 0)
	{
		llock->Acquire();
		InsertElement1(1, dll, which);
		canYield->Signal(llock);
		llock->Release();
	}
}

void
SimpleThread5(int which)
{
    if (which == 1)
    {
    	llock->Acquire();
	while(dll->IsEmpty())
		canYield->Wait(llock);
	InsertElement2(1, dll, which);
	llock->Release();    
    }
    else if (which == 0)
    {
    	llock->Acquire();
		InsertElement2(1, dll, which);
		canYield->Signal(llock);
		llock->Release();
    }
      
}

void SimpleThread6(int which)
{
    if (which == 1)
    {
    		llock->Acquire();
		while(!isFinish)
			canYield->Wait(llock);
		InsertElement1(1, dll, which);
		currentThread->Yield();
    		DeleteElement3(1, dll, which);
		llock->Release();
    }
    else if (which == 0)
    {
    		llock->Acquire();
		InsertElement1(1, dll, which);
		DeleteElement3(1, dll, which);
		isFinish = true;
		canYield->Signal(llock);
		llock->Release();
    }
}

void
SimpleThread7(int which)
{
	boundedbuffer->BoundedBufferTest(which, 1);
}

/**************************************************/
void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    //Lock* lock = new Lock("lock1");
    Thread *t = new Thread("forked thread");
    t->Fork(SimpleThread1, 1);
    SimpleThread1(0);
}

void
ThreadTest2()
{
    DEBUG('t', "Entering ThreadTest2");

     Thread *t = new Thread("forked thread");
     t->Fork(SimpleThread2, 1);
     SimpleThread2(0);
}

void
ThreadTest3()
{
     DEBUG('t', "Entering ThreadTest3");
  
     Thread *t = new Thread("forked thread");
     t->Fork(SimpleThread3, 1);
     SimpleThread3(0);
}

void
ThreadTest4()
{
     DEBUG('t', "Entering ThreadTest4");
       
     Thread *t = new Thread("forked thread");
     t->Fork(SimpleThread4, 1);
     SimpleThread4(0);
}

void
ThreadTest5()
{
     DEBUG('t', "Entering ThreadTest5");
       
     Thread *t = new Thread("forked thread");
     t->Fork(SimpleThread5, 1);
     SimpleThread5(0);
}

void
ThreadTest6()
{
     DEBUG('t', "Entering ThreadTest6");
       
     Thread *t = new Thread("forked thread");
     t->Fork(SimpleThread6, 1);
     SimpleThread6(0);
}

void
ThreadTest7()
{
	DEBUG('t', "Entering ThreadTest7");
	
	Thread *t = new Thread("forked thread");
	t->Fork(SimpleThread7, 0);
        SimpleThread7(1);
	SimpleThread7(2);
	SimpleThread7(3);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//	1,2,3: apply lock
//	4,5,6: apply variable condition
//----------------------------------------------------------------------

void
ThreadTest(int testnum)
{
    switch (testnum) {
    case 1:
	ThreadTest1(); break;
    case 2:
	ThreadTest2(); break;
    case 3:
        ThreadTest3(); break;
    case 4:
    	ThreadTest4(); break;
    case 5:
    	ThreadTest5(); break;    	
    case 6:
	ThreadTest6(); break;
    case 7:
	ThreadTest7(); break;
    default:
	printf("No test specified.\n");break;
    }
}

