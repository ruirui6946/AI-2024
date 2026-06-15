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

DLList *list;
extern void InsertList(DLList *list, int N);
extern void RemoveList(DLList *list, int N);
int nodenum;
int thread;
int testnum;
int error_type;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void SimpleThread(int which)
{
	int num;

	for (num = 0; num < 5; num++)
	{
		printf("*** thread %d looped %d times\n", which, num);
		currentThread->Yield();
	}
}

// test are threads all empty
void TestDllist(int which)
{
	InsertList(list, nodenum);
	RemoveList(list, nodenum);  
	if (!list->IsEmpty())
		printf("The thread %d :it has been empty ===\n\n", which);
	else
		printf("The thread %d :it hasn't been empty ===\n\n", which);
}
 
//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void ThreadTest1()
{
	DEBUG('t', "Entering ThreadTest1");

	Thread *t = new Thread("forked thread");

	t->Fork(SimpleThread, 1);
	SimpleThread(0);
}

//----------------------------------------------------------------------
// ThreadTest2
//----------------------------------------------------------------------

void ThreadTest2()
{
	DEBUG('t', "Entering Multi-thread-test ");
	list = new DLList();
	for (int i = 0; i < thread; i++)
	{
		char No[4] = "1";
		// create the info
		sprintf(No, "%d", i);
		char *name = new char[25];
		name[0] = '\0';
		strcat(name, "Thread ");
		strcat(name, No);
		Thread *t = new Thread(name);
		t->Fork(TestDllist, i);
	}
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void ThreadTest()
{
	switch (testnum)
	{
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
