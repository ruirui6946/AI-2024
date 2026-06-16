#include "EventBarrier.h"

EventBarrier::EventBarrier()
{
	waiternum = 0;
	lock = new Lock("EventBarrier Lock");
	event = new Condition("Event come cond");
	comp = new Condition("Event complete cond");
	signaled = false;
}

EventBarrier::~EventBarrier()
{
	delete lock;
	delete event;
	delete comp;
}

// 设置事件栅栏的状态为SIGNALED，唤醒所有阻塞于Signal的线程
// 阻塞于Complete，恢复事件栅栏的状态为UNSIGNALED
void EventBarrier::Signal()
{
	lock->Acquire();
	signaled = true;
	event->Broadcast(lock);	
	comp->Wait(lock);
	signaled = false;
	//this->Wait();
	//comp->Wait(lock);	
	lock->Release();
}

// 如果事件栅栏状态是SIGNALED，则直接返回，否则阻塞于Singal
void EventBarrier::Wait()
{
	lock->Acquire();
	waiternum++;
	if(signaled)
	{
		lock->Release();
		return;
	}
	event->Wait(lock);	
	lock->Release();
}

// 如果是最后一个调用Complete者，则唤醒所有阻塞于Complete的线程
// 否则，阻塞于Complete
void EventBarrier::Complete()
{
	lock->Acquire();
	waiternum--;
	if(waiternum==0)
	{
		comp->Broadcast(lock);
	}
	else
	{
		comp->Wait(lock);
	}
	lock->Release();
}

// 返回阻塞于Complete的线程的个数
int EventBarrier::Waiters()
{
	return waiternum;
}
