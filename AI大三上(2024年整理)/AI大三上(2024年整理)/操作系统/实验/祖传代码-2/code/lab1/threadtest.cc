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

#include <stdlib.h>
#include <time.h>
#include "copyright.h"
#include "system.h"
#include "dllist.h"
//----------------------------------------------------------------------
// SimpleThread
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

extern void insert(int n, DLList &dllist);
extern void remove(int n, DLList &dllist);
extern void YIELD(int place);

static int N = 4, T = 2;
static DLList dllist;

void ThreadInit(int t, int n)
{
    T = t, N = n;
}

void SimpleThread(int which)
{
    DEBUG('e', "%s is running.\n", currentThread->getName());
    YIELD(60);
    insert(N, dllist);
    YIELD(61);
    remove(N, dllist);
    YIELD(62);
    DEBUG('e', "%s is finish.\n", currentThread->getName());
    dllist.show();
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void ThreadTest()
{
    srand(time(0));
    Thread *threads[T];
    for (int i = 0; i < T; ++ i)
    {
        char* name = new char[20];
        name[0] = '\0';
        strcat(name, "thread ");
        sprintf(name + 7, "%d", i);
        threads[i] = new Thread(name);
        threads[i]->Fork(SimpleThread, i);
    }
}
