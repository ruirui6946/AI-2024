// synch.cc 
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks 
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    while (value == 0) { 			// semaphore not available
	queue->Append((void *)currentThread);	// so go to sleep
	currentThread->Sleep();
    } 
    value--; 					// semaphore available, 
						// consume its value
    
    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}

// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!

// this is version 1 for lock and condition

Lock::Lock(char* debugName) {
    name = debugName;
    value = Free;
    waitQueue = new List;
    thread = NULL;
}
Lock::~Lock() {
    delete waitQueue;
}

void Lock::Acquire() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);   // disable interrupts
    if(value == Busy){
        // using this to indicate lock acquire failed, thread is put into waiting list
        currentThread->Print();
        printf("lock acquire failed\n");

        waitQueue->Append((void *)currentThread);   // append to waiting list and go sleep
        currentThread->Sleep();
    }
    else{
        value = Busy;
        thread = currentThread;             // thread tells who owns lock now
    }
    (void) interrupt->SetLevel(oldLevel);   // re-enable interrupts
}
void Lock::Release() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);   // disable interrupts
    if(!waitQueue->IsEmpty()){
        Thread* threadtoRun = (Thread*)waitQueue->Remove();
        scheduler->ReadyToRun(threadtoRun);

        // using this to indicate giving lock to another thread
        currentThread->Print();
        printf("released lock. ");
        threadtoRun->Print();
        printf("is ready to acquire lock\n");
    }
    else{
        value = Free;
        thread = NULL;
    }
    (void) interrupt->SetLevel(oldLevel);   // re-enable interrupts
}
bool Lock::isHeldByCurrentThread(){
    return thread == currentThread;
}


Condition::Condition(char* debugName) {
    name = debugName;
    waitNum = 0;
    waitQueue = new List;
}
Condition::~Condition() { 
    delete waitQueue;
}
void Condition::Wait(Lock* conditionLock) { 
    waitNum++;
    waitQueue->Append((void *)currentThread);   // so go to sleep
    conditionLock->Release();

    IntStatus oldLevel = interrupt->SetLevel(IntOff);   // disable interrupts
    currentThread->Sleep();             // if we want to make thread sleep, we have to disable ints
    (void) interrupt->SetLevel(oldLevel);   // re-enable interrupts

    conditionLock->Acquire();
}
void Condition::Signal(Lock* conditionLock) {
    if(waitNum > 0){
        waitNum--;
        Thread* threadtoRun = (Thread*)waitQueue->Remove();

        printf("signal next thread to run\n");

        scheduler->ReadyToRun(threadtoRun);
    }
}
void Condition::Broadcast(Lock* conditionLock) { 
    while(waitNum > 0){
        waitNum--;
        Thread* threadtoRun = (Thread*)waitQueue->Remove();
        scheduler->ReadyToRun(threadtoRun);
    }
}
