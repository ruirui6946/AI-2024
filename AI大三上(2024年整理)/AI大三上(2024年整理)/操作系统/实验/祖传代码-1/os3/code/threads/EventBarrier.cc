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
void EventBarrier::Wait()
{
    lock->Acquire();
    waiternum++;
    if (signaled)// 如果事件栅栏已经处于Signaled状态，直接返回
    {
        lock->Release();
        return;
    }
    event->Wait(lock);
    lock->Release();
}
void EventBarrier::Signal()
{
    lock->Acquire();
    signaled = true;
    event->Broadcast(lock);
    comp->Wait(lock);
    signaled = false;
    lock->Release();
}
void EventBarrier::Complete()
{
    lock->Acquire();
    waiternum--;
    if (waiternum == 0)
    {
        comp->Broadcast(lock);// 唤醒所有线程
    }
    else
    {
        comp->Wait(lock);// 当前线程休眠
    }
    lock->Release();
}
int EventBarrier::Waiters()
{
    return waiternum;
}