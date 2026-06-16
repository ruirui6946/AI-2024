// synch-sem.cc 
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

/*
 * 该类定义了一个信号量值和一个等待队列，对信号量有两个操作：
 * P操作和V操作，这两个操作都是原子操作
 * (Nachos因为是一个运行在单一处理器上的操作系统，所以原子操作只需要进行开挂中断即可实现)。
 * 其中P操作：当value等于0时，将当前运行线程放入线程等待队列，当前进程进入睡眠状态，并切换到其他线程运行；
 * 当value大于0时，value–-。
 * V操作：如果线程等待队列中有等待该信号量的线程，取出其中一个将其设置成就绪态，准备运行，value++。
 */

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

//P操作：当value等于0时，将当前运行线程放入线程等待队列，当前进程进入睡眠状态，并切换到其他线程运行；
//当value大于0时，value–-。

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

//V操作：如果线程等待队列中有等待该信号量的线程，取出其中一个将其设置成就绪态，准备运行，value++。

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

//----------------------------------------------------------------------
// Lock::Lock
//  Initialize lock.
//----------------------------------------------------------------------

//锁有两个操作 Acquire 和 Release，它们都是原子操作。
// Acquire: 申请锁：当锁处于 BUSY 态，进入睡眠状态。
// 当锁处于 FREE 态，当前线程获得该锁，继续运行。
// Release: 释放锁（注意：只有拥有锁的线程才能释放锁）将锁的状态设置成 FREE 态，
// 如果有其它线程等待该锁，将其中的一个唤醒，进入就绪态。

Lock::Lock(char* debugName) 
{
    name = debugName;
    sem = new Semaphore("lock semaphore", 1);   // initial semaphore value is 1
    current = NULL;
}

//----------------------------------------------------------------------
// Lock::~Lock
//  Deallocate lock.
//----------------------------------------------------------------------

Lock::~Lock()
{
    delete sem;
}

//----------------------------------------------------------------------
// Lock::Acquire
//  Acquire a lock, just do P()'s op, since binary semaphore quite looks
//  like a lock.
//----------------------------------------------------------------------

// Acquire: 申请锁：当锁处于 BUSY 态，进入睡眠状态。
// 当锁处于 FREE 态，当前线程获得该锁，继续运行。

void Lock::Acquire()
{
    ASSERT (isHeldByCurrentThread() == FALSE);

    sem->P();
    current = currentThread;
}

//----------------------------------------------------------------------
// Lock::Release
//  Release a lock, do V()'s op.
//----------------------------------------------------------------------

// Release: 释放锁（注意：只有拥有锁的线程才能释放锁）将锁的状态设置成 FREE 态，
// 如果有其它线程等待该锁，将其中的一个唤醒，进入就绪态。

void Lock::Release()
{
    ASSERT (isHeldByCurrentThread() == TRUE);

    sem->V();
    current = NULL;
}

//----------------------------------------------------------------------
// Lock::isHeldByCurrentThread
//  Return true if the current thread holds this lock, false otherwise.
//----------------------------------------------------------------------

bool Lock::isHeldByCurrentThread()
{
    if (current == currentThread)
        return TRUE;
    else
        return FALSE;
}

//----------------------------------------------------------------------
// Condition::Condition
//  Initinalize condition variable.
//----------------------------------------------------------------------

//条件变量有三个操作 Wait、Signal 以及 BroadCast，
//所有的这些操作必须在当前线程获得一个锁的前提下，而且所有对一个条件变量进行的操作必须建立在同一个锁的前提下。
// void Wait (Lock *conditionLock) 把线程放入条件变量的等待队列上。
// void Signal (Lock *conditionLock) 唤醒一个等待该条件变量的线程（如果存在的话）。
// void BroadCast (Lock *conditionLock) 唤醒所有等待该条件变量的线程（如果存在的话）


Condition::Condition(char* debugName)
{
    name = debugName;
    waiters = 0;
    s = new Semaphore("sem s", 0);      // counting semaphore
    x = new Semaphore("sem x", 1);      // binary semaphore, used as lock
    h = new Semaphore("sem h", 0);      // counting semaphore
}

//----------------------------------------------------------------------
// Condition::Condition
//  Delete condition variable.
//----------------------------------------------------------------------

Condition::~Condition()
{
    delete s;
    delete x;
    delete h;
}

//----------------------------------------------------------------------
// Condition::Wait
//  It's quite difficult for us to 'create' this algorithm individually, 
//  see A. D. Birrell, 2003 please, we just *refer* their work, ^_^. 
//----------------------------------------------------------------------

// void Wait (Lock *conditionLock) 把线程放入条件变量的等待队列上。

void Condition::Wait(Lock* conditionLock) 
{
    ASSERT (conditionLock->isHeldByCurrentThread() == TRUE);

    // 临界区，记录阻塞队列元素的个数
    x->P();
    waiters++;
    x->V();
    conditionLock->Release();  // 释放锁
    s->P(); // 把当前线程加入阻塞队列
    h->V(); // 满足题目要求，不能多次Signal产生叠加效果
    conditionLock->Acquire(); // 获得锁
}

//----------------------------------------------------------------------
// Condition::Signal
//  See A. D. Birrell, 2003 too.
//----------------------------------------------------------------------

// void Signal (Lock *conditionLock) 唤醒一个等待该条件变量的线程（如果存在的话）。

void Condition::Signal(Lock* conditionLock)
{
    // ASSERT (conditionLock->isHeldByCurrentThread() == TRUE);

    // 临界区保护
    x->P();
    if (waiters > 0)
    {
        waiters--;
        s->V(); // 从阻塞队列里释放一个线程
        h->P(); // 不能多次Wait
    }
    x->V();
}

//----------------------------------------------------------------------
// Condition::Broadcast
//  See A. D. Birrell, 2003. Hmm, I think we can propose it if we know
//  how to implement Signal primitive.
//----------------------------------------------------------------------

// void BroadCast (Lock *conditionLock) 唤醒所有等待该条件变量的线程（如果存在的话）

void Condition::Broadcast(Lock* conditionLock)
{
    // ASSERT (conditionLock->isHeldByCurrentThread() == TRUE);

    x->P();
    for (int i = 0; i < waiters; ++i)
        s->V();
    while (waiters > 0)
    {
        waiters--;
        h->P();
    }
    x->V();
}
