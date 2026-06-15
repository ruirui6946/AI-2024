#include "Alarm.h"
#include "system.h"

Alarm* alarm = new Alarm();

static void AlarmHandler(int arg)
{ 
    Alarm *p = (Alarm *)arg; 
    p->Restart(); 
}

Alarm::Alarm()
{
    alarmNum = 0;
    queue = new List;
}

Alarm::~Alarm()
{
    delete queue;
}

static void hold(int alarmNum)
{
    int* p = (int *) alarmNum;
    currentThread->Yield();
    while(*p)
    {
        currentThread->Yield();
    }
}

void Alarm::Pause(int howLong)
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    if(alarmNum == 0)
    {
        Thread *thread = new Thread("Alarm");
        thread->Fork(hold, (int)&alarmNum);
    }

    interrupt->Schedule(AlarmHandler, (int)this, howLong, TimerInt);
    queue->SortedInsert(currentThread, stats->totalTicks + howLong);
    ++ alarmNum;

    DEBUG('s', "%s: sleep %d tick from Tick %d to Tick %d.\n", 
    currentThread->getName(), howLong, stats->totalTicks, stats->totalTicks + howLong);
 
    currentThread->Sleep();

    (void)interrupt->SetLevel(oldLevel);
}

void Alarm::Restart()
{
    int when;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    Thread* thread = (Thread*)queue->SortedRemove(&when);
    ASSERT(stats->totalTicks >= when);
    scheduler->ReadyToRun(thread);
    -- alarmNum;

    DEBUG('s', "%s: wakeup in Tick %d.\n", thread->getName(), stats->totalTicks);

    (void)interrupt->SetLevel(oldLevel);
}
