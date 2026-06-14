#include "Alarm.h"

char name1[3][30];

static void AlarmHandler(int arg)//周期性检查是否闹钟响了
{
	Alarm *p = (Alarm*) arg;
	p->Call();
}

Alarm::Alarm()
{
	pending = new List();
	num = 0;
}

Alarm::~Alarm()
{
	while (!pending->IsEmpty())
		delete (PendingInterrupt*)pending->Remove();
	delete pending;
}

void Find(int num)
{
	int *a;
	a = (int*)num;
	currentThread->Yield();
	while (*a != 0)
		currentThread->Yield();//生成的新线程一直执行切换
}

void Alarm::Pause(int howLong)
{
	void Find(int i);
	IntStatus oldLevel = interrupt->SetLevel(IntOff);//关中断
	num++;//睡眠线程数增加
	if (pending->IsEmpty() == TRUE)//阻塞队列中没有放入线程时
	{
		sprintf(name1[num-1],"newthread from %s",currentThread->getName());
		Thread *t = new Thread(name1[num-1]);//生成一个新线程执行切换，在不断的切换过程中去唤醒睡眠时间已到的线程，
                                            //当无睡眠线程时，则结束。
		t->Fork(Find,(int)&num);
	}
	int when = stats->totalTicks + howLong;// 根据stats->totalTicks所取得的系统运行的当前时间，加上howLong即为线程唤醒时间
	pending->SortedInsert((void*)currentThread,when);//队列根据醒来的时间对线程进行排序，然后让该线程阻塞。
	interrupt->Schedule(AlarmHandler,(int)this,when,TimerInt);// Schedule an interrupt to occur at time ``when''.  This is called by the hardware device simulators.
	currentThread->Sleep();//当前线程进入睡眠
	(void)interrupt->SetLevel(oldLevel);//开中断
}

void Alarm::Call()
{
	int when;
	IntStatus oldLevel = interrupt->SetLevel(IntOff);//关中断
	Thread *t = (Thread *)pending->SortedRemove(&when);//t为队首线程
	if (t == NULL) // no pending interrupts
		return;
	if (when <= stats->totalTicks && t != NULL)//当前时间超过线程的唤醒时间时，将线程唤醒，放入预备队列
	{
		//advance the clock;
		scheduler->ReadyToRun(t);
		num--;//睡眠线程数减1
	}
	else
	{
		pending->SortedInsert((void*)t,when);//如果没到唤醒时间，就把线程再放回阻塞队列中
		interrupt->Schedule(AlarmHandler,(int)this,when,TimerInt);//周期性检查闹钟是否响了
	}
	(void) interrupt->SetLevel(oldLevel);//开中断

//check if there is nothing more to do,and if so,quit
}

