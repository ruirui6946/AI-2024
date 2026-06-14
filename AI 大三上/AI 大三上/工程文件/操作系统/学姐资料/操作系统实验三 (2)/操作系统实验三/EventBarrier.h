#ifndef EVENTBARRIER_H_INCLUDED
#define EVENTBARRIER_H_INCLUDED

#include "system.h"
#include "copyright.h"
#include "synch.h"

class EventBarrier
{
    public:
        EventBarrier();
        ~EventBarrier();
        void Wait(); //-- Wait until the event is signaled. Return immediately if already in the signaled state.
        void Signal();// -- Signal the event and block until all threads that wait for this event have responded. The
                    //EventBarrier reverts to the unsignaled state when Signal() returns.
        void Complete();// -- Indicate that the calling thread has finished responding to a signaled event, and block
                        //until all other threads that wait for this event have also responded.
        int Waiters();// -- Return a count of threads that are waiting for the event or that have not yet responded to it.
    private:
        int status;  // signaled时为1, unsignaled时为0
        int waitnum; // 等待线程数
        Lock *lock; //锁用来建立临界区
        Condition *signal;//用于 挂起和唤醒 调用Signal()函数的线程
      //  Condition *complete;//用于 挂起和唤醒 完成回应而处于等待别的事件回应的线程
        Condition *wait;//用于 挂起和唤醒 那些当事件处于unsignaled而调用Wait()的线程
};

#endif // EVENTBARRIER_H_INCLUDED
