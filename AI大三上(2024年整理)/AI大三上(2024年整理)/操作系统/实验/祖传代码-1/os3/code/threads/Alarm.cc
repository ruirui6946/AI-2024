#include "Alarm.h"
#include "interrupt.h"

Alarm *Alarm::instance;

void Alarm::new_instance()
{
    instance = new Alarm();
}

Alarm::Alarm()
{
    queue = new List;
    waiters = 0;
}
Alarm::~Alarm()
{
    delete queue;
}

void Alarm::Continue(int arg){// interupt要用得的handler
    Alarm* temp = (Alarm*)arg;
    temp->Wakeup();
}

void Alarm::Pause(int howLong) // howlong单位为中断次数
{
    
    waiters++;
    Thread *loop_t;
    if (waiters == 1)
    {
        loop_t = new Thread("loop thread");
        loop_t->Fork(check, 0); // 使至少有一个线程在运行，防止系统终止
    }
    IntStatus oldLevel = interrupt->SetLevel(IntOff); // 必须关中断
    int Ticks = TimerTicks * howLong;// timerticks默认100
    queue->SortedInsert((void *)currentThread,
                        stats->totalTicks + Ticks); // 按结束时间依序插入链表
    interrupt->Schedule(Continue, (int)this, Ticks, TimerInt);// 用这一行在howlong以后唤醒sleep的进程
    currentThread->Sleep();
    (void)interrupt->SetLevel(oldLevel);
}

void check(int which)
{
    while (Alarm::instance->waiters != 0)
    {
        currentThread->Yield();
    }
    currentThread->Finish();
}

void Alarm::Wakeup()
{
    int duetime = -1;
    Thread *thread = NULL;
    IntStatus oldLevel = interrupt->SetLevel(IntOff); // 必须关中断
    thread = (Thread *)queue->SortedRemove(&duetime);
    // 把duetime改成被删除的那个元素的key值，该key值即插入时间，将该值作为thread的名字
    waiters--;
    scheduler->ReadyToRun(thread);
    printf("%s wakeup!\n",thread->getName()); 
    (void)interrupt->SetLevel(oldLevel);
}

