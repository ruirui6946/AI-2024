# include "EventBarrier.h"

EventBarrier::EventBarrier(char* debugName)
{
    name = debugName;
    waiterNum = 0;
    signalStatus = false;
    lock = new Lock("EventBarrier Lock");
    signaled = new Condition("Signaled Condition");
    complete = new Condition("Complete Condition");
    allCompleted = new Condition("AllComplete Condition");
}

EventBarrier::~EventBarrier()
{
    delete lock;
    delete signaled;
    delete complete;
    delete allCompleted;
}

// 等待事件发生的信号，若处于有信号状态，直接返回
void EventBarrier::Wait()
{
    lock->Acquire();
    ++ waiterNum;
    if(signalStatus)
    {
        lock->Release();
        return ;
    }
    signaled->Wait(lock);
    lock->Release();
}

// 广播事件发生的信号，并等待所有线程都完成响应
// 当Signal()返回时，EventBarrier将恢复为条无信号状态
void EventBarrier::Signal()
{
    lock->Acquire();
    signalStatus = true;
    signaled->Broadcast(lock);
    while(waiterNum > 0)
    {
        allCompleted->Wait(lock);
    }
    complete->Broadcast(lock);
    signalStatus = false;
    lock->Release();
}

// 线程完成对事件信号的响应，并等待其他线程都完成响应
void EventBarrier::Complete()
{
    lock->Acquire();
    -- waiterNum;
    if(waiterNum == 0) allCompleted->Broadcast(lock);
    complete->Wait(lock);
    lock->Release();
}

// 返回正在等待事件或尚未完成响应的线程的数量
int EventBarrier::Waiters()
{
    return waiterNum;
}
