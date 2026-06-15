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
#include "synch-sem.h"
#include "thread.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)//0是条件变量，1是锁
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
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// 关中断
    while (value == 0) { 			// semaphore not available
	queue->Append((void *)currentThread);	// 把自己放入阻塞队列
	currentThread->Sleep();
    } 
    value--; 					// semaphore available, 
						
    
    (void) interrupt->SetLevel(oldLevel);	// 开中断
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
Lock::Lock(char* debugName)
{
	name = debugName;
	sem = new Semaphore(debugName, 1);
}

Lock::~Lock()
{
	delete sem;
}

//----------------------------------------------------------------------
// Lock::Acquire
// 	If isFree = true, then grab the lock and set isFree = false;
// 	If isFree = false, then put thread to queue and block thread;
//----------------------------------------------------------------------
void Lock::Acquire()
{	
	sem->P();
	owner = currentThread;
}

//----------------------------------------------------------------------
// Lock::Release
// 	Remove thread from queue and put thread to ready queue;
//	Set isFree = true, owner = NULL;	
//----------------------------------------------------------------------
void Lock::Release()
{
	ASSERT(isHeldByCurrentThread());

	sem->V();
	owner = NULL;
}

//----------------------------------------------------------------------
// Lock::isHeldByCurrentThread
// 	Test whether currentThread holding the lock
//----------------------------------------------------------------------
bool Lock::isHeldByCurrentThread()
{
	if (owner == currentThread)
		return true;
	return false;
}

Condition::Condition(char* debugName)
{
	name = debugName;
	sem = new Semaphore(debugName, 0);
	threadnum = 0;
}

Condition::~Condition()
{
	delete sem;
}

//----------------------------------------------------------------------
// Condition::Wait
//      For certain condition, Wait() put it to the monitor queue;
//	Thread first release lock and then go to sleep;
//	After thread is woke up, it grab lock and run.
//----------------------------------------------------------------------
void Condition::Wait(Lock* conditionLock)
{
	ASSERT(conditionLock->isHeldByCurrentThread());//执行等待的需要是有锁的线程
	IntStatus oldLevel = interrupt->SetLevel(IntOff);       // 关中断
	
	conditionLock->Release();//释放锁
	numWaiting++;
	sem->P();
	conditionLock->Acquire();//如果被唤醒，就再次请求锁
	
	(void) interrupt->SetLevel(oldLevel);   // 开中断
}

//----------------------------------------------------------------------
// Condition::Signal
//      For certain condition, Signal() wake up one  thread from
//      monitor queue; Then put it to the ready queue;
//----------------------------------------------------------------------
void Condition::Signal(Lock* conditionLock)
{
	ASSERT(conditionLock->isHeldByCurrentThread());//执行信号的必须是已有锁的线程
	IntStatus oldLevel = interrupt->SetLevel(IntOff);       // 关中断
	
	if (numWaiting > 0)
	{
		sem->V();
		numWaiting--;
	}
	
	(void) interrupt->SetLevel(oldLevel);   // 开中断
}

//----------------------------------------------------------------------
// Condition::Broadcast
//      For certain condition, broadcast() wake up all the thread in the
//	condition queue; Then put them to the ready queue;
//----------------------------------------------------------------------
void Condition::Broadcast(Lock* conditionLock)//广播
{
	ASSERT(conditionLock->isHeldByCurrentThread());
	IntStatus oldLevel = interrupt->SetLevel(IntOff);       // 关中断

	while (numWaiting > 0)
	{
		sem->V();
		numWaiting--;
	}

	(void) interrupt->SetLevel(oldLevel);   // 开中断
}
