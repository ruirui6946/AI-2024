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

Lock::Lock(char* debugName)
{
 	name = debugName;//记录锁的名字
 	lockvalue = false;//初始化锁是unlock
 	lockqueue = new List;//初始化列表
 	lockhoder = NULL;

}

Lock::~Lock()
{
 	delete (lockqueue);
 	delete (lockhoder);
}

void Lock::Acquire()
{
	IntStatus oldLevel = interrupt->SetLevel(IntOff);
    //关中断，此操作要保证不被打断
 	ASSERT(!isHeldByCurrentThread());
 	//请求锁的线程不能已经拥有该锁，即不能二次请求
	while (lockvalue == true)//当锁已被占用
 	{
		//printf("lock is unavailable,sleep\n");
		lockqueue->Append((void *)currentThread);//将该线程放入队列中
		currentThread->Sleep();//等待唤醒
	}
	//printf("lock is available\n");//线程被唤醒了，锁可用时
	lockvalue = true;//将锁标记为“占有”状态
 	lockhoder = currentThread;//记录下所的新拥有者
    (void)interrupt->SetLevel(oldLevel);//开中断
}

void Lock::Release()
{
    Thread *thread;
 	IntStatus oldLevel = interrupt->SetLevel(IntOff);
 	ASSERT(isHeldByCurrentThread());
 	thread = (Thread *)lockqueue->Remove();//从队列中移除一个被阻塞的线程
 	if (thread != NULL)
 		scheduler->ReadyToRun(thread);//如果有被阻塞的线程，将它放到阻塞队列中
 	lockvalue = false;
 	lockhoder = NULL;//释放锁
 	(void)interrupt->SetLevel(oldLevel);

}


bool Lock::isHeldByCurrentThread()//判断线程是否拥有锁
{
 	return (currentThread == lockhoder);
}

Condition::Condition(char* debugName)//条件变量构造函数
{
 	name = debugName;
	ConditionQueue = new List;
}

Condition::~Condition()
{
 	delete (ConditionQueue);
}

void Condition::Wait(Lock* conditionLock)
{
	ASSERT(conditionLock->isHeldByCurrentThread());
	//执行等待的必须是拥有锁的线程，因为接下来它将释放锁
	IntStatus oldLevel = interrupt->SetLevel(IntOff);
	//关中断
	conditionLock->Release();//释放锁
 	ConditionQueue->Append((void *)currentThread);//把自己放入阻塞队列
 	currentThread->Sleep();//睡觉等待
 	(void)interrupt->SetLevel(oldLevel);//开中断
 	conditionLock->Acquire();//如果被唤醒，就再次请求锁

}

void Condition::Signal(Lock* conditionLock)
{
	Thread *thread;
	ASSERT(conditionLock->isHeldByCurrentThread());
 	IntStatus oldLevel = interrupt->SetLevel(IntOff);
 	thread = (Thread *)ConditionQueue->Remove();//从队列中移除一个被阻塞的线程
 	if (thread != NULL)
 		scheduler->ReadyToRun(thread);//如果阻塞队列中有被阻塞的线程，将它放到阻塞队列中
	(void)interrupt->SetLevel(oldLevel);
}


void Condition::Broadcast(Lock* conditionLock)//对所有被阻塞的线程发出通知
{
 	Thread *thread;
 	ASSERT(conditionLock->isHeldByCurrentThread());
 	IntStatus oldLevel = interrupt->SetLevel(IntOff);
 	while (!ConditionQueue->IsEmpty())//阻塞队列中还有线程时
 	{
 		thread = (Thread *)ConditionQueue->Remove();
 		if (thread != NULL)
 			scheduler->ReadyToRun(thread);
 	}
 	(void)interrupt->SetLevel(oldLevel);
}

