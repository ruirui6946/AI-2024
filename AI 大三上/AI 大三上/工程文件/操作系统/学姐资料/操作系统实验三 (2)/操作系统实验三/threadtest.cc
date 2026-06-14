// threadtest.cc
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield,
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include "copyright.h"
#include "system.h"
#include "dllist.h"
#include "Table.h"
#include "BoundedBuffer.h"
#include "EventBarrier.h"
#include "Alarm.h"
#include "Bridge.h"
#include "Elevator.h"

int testnum = 1,threadnum = 1,err_type = 0,n;
extern int consumernum;
extern int producernum;
int floornum = 10;  //The number of the floor(-f)
int elevatornum = 1;  //The number of the elevator(-l)
int capacity = 20;    //The number of the elevator'capacity(-c)

DLList *dllist;
Table table(5);
BoundedBuffer bbuff(3);
EventBarrier *barr = new EventBarrier();
Alarm *alarm = new Alarm();
Bridge *bridge = new Bridge();
Building *building;

extern void Insert(int t,int n,DLList *dllist);
extern void Remove(int t,int n,DLList *dllist);
extern void rider(int id, int srcFloor, int dstFloor);

void DLListThread(int t)
{
    Insert(t,n,dllist);
    Remove(t,n,dllist);
}

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;

    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

void TableThread(int which)
{
    int user[4],j;
    for (int j = 0;j < 4;j++)//向表中存放数据
    {
        void *obj = (void*)(rand()%100+1);
        printf("Thread %d save %d\n",which,(int)obj);
        user[j] = table.Alloc(obj);
        printf("Thread %d index = %d\n",which,user[j]);
        if (user[j] == -1)
		{
			printf("Alert!! index is out of range.\n");
			return;
		}
    }
    for (j = 0;j < 4;j++)//遍历表
        printf("Thread %d: table[%d] = %d\n",which,user[j],(int)table.Get(user[j]));
    for (j = 0;j < 4;j++)//删除表中数据
    {
        printf("Thread %d delete table[%d]\n",which,user[j]);
        table.Release(user[j]);
        printf("Thread %d: option succeed!\n",which);
    }
}

void bbuffThread(int x)
{
	char buf[2] = "A";//buf[0] = 'A',buf[1] = '\0'
	for (int i = 0;i < 20;i++)
	{

		bbuff.Write(buf,1);//每次向缓冲区写入一个字母,A到T
		printf("Thread %d Producer:write %s\n",x,buf);
		++buf[0];//A到B到……T
	}
}

void consume(int x)
{
	for (int i =0;i<10;i++)//一共二十个字符，读十次
	{
		char buf[3] = {0};
		bbuff.Read(buf,2);
		printf("Thread %d Consumer:read %s\n",x,buf);
	}
}

void EventBarrierThread(int Threadid)
{
	if(Threadid == 0)
	{
		currentThread->Yield();
		printf("thread %d is signaled\n",Threadid);
		barr->Signal();
		printf("thread %d finished\n",Threadid);
	}
	else
	{
		printf("Thread %d is waiting\n",Threadid);
		barr->Wait();
		printf("Thread %d is completed\n",Threadid);
		barr->Complete();
		printf("Thread %d is leaving\n",Threadid);
	}
}

void AlarmThread(int threadid)
{
	void *obj = (void*)(rand()%1000+1);//随机产生闹钟时间
	printf("Thread %d is waiting %d Tick now = %d \n",threadid,(int)obj,stats->totalTicks);//stats->totalTicks为当前时间
	alarm->Pause(int(obj));
	printf("thread %d is ok now = %d\n",threadid,stats->totalTicks);
}

void BridgeThread(int which)
{
    int i = rand()%2;//随机产生过桥方向
	char direct;
	if (i == 0)
        direct = 'l';//i为0则为从左边过桥
	else
        direct = 'r';//i为1则为从右边过桥
	bridge->ArriveBridge(direct);
    bridge->CrossBridge(direct);
	bridge->ExitBridge(direct);
}

void ElevatorThread(int which)
{
    int srcFloor, dstFloor;
    do
	{
    	srcFloor = rand()%floornum + 1;
    	dstFloor = rand()%floornum + 1;
    }while (srcFloor == dstFloor);
	rider(which,srcFloor,dstFloor);
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

void ThreadTest2()
{
    DEBUG('t', "Entering ThreadTest2");

    dllist = new DLList(err_type);
    for (int i = 1;i < threadnum;i++)
    {
        Thread *t = new Thread("forked thread");
        t->Fork(DLListThread, i);
    }
    DLListThread(threadnum);
}

void TableTest()
{
    //DEBUG('t', "Entering TableTest ");
    Thread *t = new Thread("forked thread");
    t->Fork(TableThread, 1);
    TableThread(0);
}

void BoundedBufferTest()
{
	Thread*t = new Thread("boundedbuffer Thread");
	t->Fork(bbuffThread,1);
	scheduler->Print();
	for (int i = 0;i < 10;i++)
	{
		char buf[3] = {0};
		bbuff.Read(buf,2);//每次从缓冲区读两个字符
		printf("Consumer:read %s\n",buf);
	}

}

void PriorityTest()
{
	int p;
	for (int i = 1;i <= consumernum;i++)
    {
        printf("please set the priority for this consumer thread:");
        scanf("%d",&p);
        Thread *t = new Thread("consumeThread",p);
        t->Fork(consume, i);
    }
    scheduler->Print();
    printf("\n");
	for (int i = consumernum+1;i <= consumernum+producernum;i++)
    {
        printf("please set the priority for this producer thread:");
        scanf("%d",&p);
        Thread *t = new Thread("produceThread",p);
        t->Fork(bbuffThread, i);
    }
	scheduler->Print();
    printf("\n");
    currentThread->Yield();

}

void EventBarrierTest()
{
	char name[25];
	int i;
	DEBUG('t',"Entering EventBarrierTest");
    for(i = 1;i <= 3;i++)
    {
        sprintf(name,"EventBarrier_Thread %d",i);
        Thread *t = new Thread(name);
        t->Fork(EventBarrierThread,i);
        currentThread->Yield();
    }
    EventBarrierThread(0);
}

void AlarmTest()
{
	char name[3][30];
	DEBUG('t',"Entering AlarmTest");
	for(int i = 1;i <= 3;i++)
	{
		sprintf(name[i-1],"Alarm_Thread %d",i);
		Thread *t = new Thread(name[i-1]);
		t->Fork(AlarmThread,i);
		//scheduler->Print();
		//printf("this is AlarmTest Print\n");
		currentThread->Yield();
	}
}

void BridgeTest()
{
    DEBUG('t', "Entering ThreadTest3");
    char **name;
    name = new char*[threadnum];
    for (int i = 1; i < threadnum; i++)
	{
	    name[i] = new char[5];
	    sprintf(name[i],"%d", i);
	    Thread *t = new Thread(name[i]);
	    t->Fork(BridgeThread, i);
    }
	BridgeThread(threadnum);
}

void ElevatorTest()
{
    DEBUG('t', "Entering ThreadTest4");
    char **name;
    building = new Building("building",floornum,elevatornum);
	printf("elevatornum = %d  ridernum = %d  floornum = %d capacity = %d \n\n",elevatornum,threadnum,floornum,capacity);
    name = new char*[threadnum];
    for (int i = 1; i <= threadnum; i++)
	{
	    name[i] = new char[5];
	    sprintf(name[i],"%d", i);
	    Thread *t = new Thread(name[i]);
	    t->Fork(ElevatorThread, i);
    }
	//ElevatorThread(0);
}
//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
	ThreadTest1();
	break;
	case 2:
	ThreadTest2();
	break;
	case 3:
    TableTest();
    break;
    case 4:
    BoundedBufferTest();
    break;
    case 5:
    PriorityTest();
    break;
    case 6:
    EventBarrierTest();
    break;
    case 7:
    AlarmTest();
    break;
    case 8:
    BridgeTest();
    break;
    case 9:
    ElevatorTest();
    break;
    default:
	printf("No test specified.\n");
	break;
    }
}

