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
int threadnum = 2;
int listsize = 5;
char item1[] = "insert_head_item";
char item2[]="insert_tail_item";
DLList *pointer;
//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------
extern void DLLGenerate(DLList *list,int n, int which);
extern void DLLRemove(DLList *list,int n, int which);

void SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
       	currentThread->Yield();
    }
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

void GenerateList(int which)
{
	DLLGenerate(pointer,listsize, which);
	if (testnum==3)
	{pointer->Prepend(item1);}
	if (testnum==4){pointer->Append(item2);}
	if (testnum==5){pointer->SortedRemove(50);}
	DLLRemove(pointer,listsize, which);
}

void ThreadTest2()
{
	DEBUG('t', "Entering ThreadTest2");
	for (int i = 1; i < threadnum; i++)
	{
		Thread *t = new Thread("forked thread");

		t->Fork(GenerateList, i);
	}
	GenerateList(0);
}
void ThreadTest3()
{
	DEBUG('t', "Entering ThreadTest2");
	for (int i = 1; i < threadnum; i++)
	{
		Thread *t = new Thread("forked thread");

		t->Fork(GenerateList, i);
	}
	GenerateList(0);
}
void ThreadTest4()
{
	DEBUG('t', "Entering ThreadTest2");
	for (int i = 1; i < threadnum; i++)
	{
		Thread *t = new Thread("forked thread");

		t->Fork(GenerateList, i);
	}
	GenerateList(0);
}
void ThreadTest5()
{
	DEBUG('t', "Entering ThreadTest2");
	for (int i = 1; i < threadnum; i++)
	{
		Thread *t = new Thread("forked thread");

		t->Fork(GenerateList, i);
	}
	GenerateList(0);
}


//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
	pointer=new DLList();
    switch (testnum) {
    case 1:
	ThreadTest1();
	break;
    case 2:
	ThreadTest2();
	break;
	case 3:
	ThreadTest3();
	break;
	case 4:
	ThreadTest4();
	break;
	case 5:
	ThreadTest5();
	break;
    default:
	printf("No test specified.\n");
	break;
    }
}

