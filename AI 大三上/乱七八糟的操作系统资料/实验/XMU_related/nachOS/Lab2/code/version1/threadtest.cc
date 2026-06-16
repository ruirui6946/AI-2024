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
#include "syndllist.h"

// testnum is set in main.cc
int testnum = 4;

// dl is doubly linked list, N is items we store in dl
DLList dl;
const int N = 8;
// sl is synchronized doubly linked list, used for threadtest 3
Syndllist sl;
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


void SynThreadBatchAdd(int t){
    int key[N] = {7,9,6,5,8,4,13,56};

    // print out the keys inserted
    for(int i=0;i<N;i++){
        printf("%d\n",key[i]);
    }

    int *item = key;
    for(int i=0;i<N;i++){
        sl.Append((void*)item, key[i]);
    }
}


void SynThreadTestDel(int t){
    int keyValue;
    for(int i=0;i<N/2;i++){
        printf("\n\n--------enter thread %d for remove test--------------\n",t);
        sl.Remove(&keyValue);
        printf("thread %d remove at %d\n", t, keyValue);
        printf("--------thread %d switched for another to use------------\n\n",t);
        currentThread->Yield(); 
    }
}

void SynThreadInsert(int t){
    const int N = 3;
    int keyValue[N] = {3,5,8};
    for(int i=0;i<N;i++){
        printf("\n\n--------enter thread %d for insert test--------------\n",t);
        int *item = &keyValue[i];
        sl.Append((void*)item, keyValue[i]);
        printf("list finsihed append %d with thread %d\n\n", keyValue[i], t);
        currentThread->Yield();
    }
}


void SynThreadDel(int t){
    const int N = 3;
    int keyValue;
    for(int i=0;i<N;i++){
        printf("\n\n--------enter thread %d for remove test--------------\n",t);
        sl.Remove(&keyValue);
        printf("thread %d remove at %d\n", t, keyValue);
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

    printf("getItems\n");
    // generate N random keys for dl
    genItems(&dl, N);

    printf("\n---------------------------\n");
    // fork a new thread to do threadDelTest, allocate thread number as 0
    t->Fork(threadDelTest, 0);
    // do threadDelTest, allocate thread number as 1
    threadDelTest(1);
}

// threadtest3, used to test thread with lock and condition implemented.
void ThreadTest3(){
    DEBUG('t', "Entering ThreadTest3");

    Thread *t = new Thread("thread 0");
    printf("getItems\n");

    printf("\n---------------------------\n");

    SynThreadBatchAdd(0);
    // fork a new thread to do threadDelTest, allocate thread number as 0
    t->Fork(SynThreadTestDel, 0);
    // do threadDelTest, allocate thread number as 1
    SynThreadTestDel(1);

}

// threadtest 4, used to test condition variables.
void ThreadTest4(){
    DEBUG('t', "Entering ThreadTest4");
    Thread *t = new Thread("thread to insert");
    printf("start with del\n");

    int key = 5;
    int *item = &key;
    t->Fork(SynThreadInsert, 1);

    SynThreadDel(0);

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
    case 3:
    ThreadTest3();
    case 4:
    ThreadTest4();

    default:
	printf("No test specified.\n");
	break;
    }
}

