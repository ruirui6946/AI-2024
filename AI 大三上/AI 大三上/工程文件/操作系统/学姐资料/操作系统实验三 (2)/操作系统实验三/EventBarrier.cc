#include "EventBarrier.h"

EventBarrier::EventBarrier()
{
	status = 0;//初始为unsignaled状态
	waitnum = 0;//等待线程数为0
	lock = new Lock("lock");
	signal = new Condition("signal");
	//complete = new Condition("complete");
	wait = new Condition("wait");
}

EventBarrier::~EventBarrier()
{
    delete lock;
    delete signal;
   // delete complete;
    delete wait;
}

void EventBarrier::Wait()
{
    lock->Acquire();
    waitnum++;
    if (status == 1)//处于signaled状态直接return
    {
        lock->Release();
        return;
    }
    wait->Wait(lock);//处于unsignaled状态就等到处于signaled状态为止
    lock->Release();

}

void EventBarrier::Signal()
{
    lock->Acquire();
    status = 1;//变为signaled状态
    wait->Broadcast(lock);//通知所有等待event的事件
    while (waitnum != 0)
        signal->Wait(lock);//当还有事件没回应时挂起
   // complete->Broadcast(lock);//通知所有等待别的线程回应的线程
    status = 0;//回到unsignaled状态
    lock->Release();
}

void EventBarrier::Complete()
{
    lock->Acquire();
    waitnum--;
    if (waitnum == 0)
        signal->Signal(lock);//通知Signal线程全部事件已处理完
   // complete->Wait(lock);//等待其它线程的回应
    lock->Release();
}

int EventBarrier::Waiters()
{
    return waitnum;
}
