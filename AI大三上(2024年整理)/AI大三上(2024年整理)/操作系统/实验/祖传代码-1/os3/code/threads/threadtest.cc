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
#include "copyright.h"
#include "system.h"
#include "dllist.h"
#include "Table.h"
/*
table.h中包含表信息
*/
#include "BoundedBuffer.h"
#include "EventBarrier.h"
#include "Alarm.h"
#include "Bridge.h"
#include "Elevator.h"
// #define LAB_TWO
#define LAB_THREE
DLList *list;
BoundedBuffer *boundedbuffer;
EventBarrier *barrier;
extern void InsertList(DLList *list, int N);
extern void RemoveList(DLList *list, int N);
int nodenum;
int thread;
int testnum;
int error_type;
//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void SimpleThread(int which)
{
	int num;

	for (num = 0; num < 5; num++)
	{
		printf("*** thread %d looped %d times\n", which, num);
		currentThread->Yield();
	}
}

// test are threads all empty
void TestDllist(int which)
{
	InsertList(list, nodenum);
	RemoveList(list, nodenum);
	if (!list->IsEmpty())
		printf("The thread %d :it has been empty ===\n\n", which);
	else
		printf("The thread %d :it hasn't been empty ===\n\n", which);
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

//----------------------------------------------------------------------
// ThreadTest2
//----------------------------------------------------------------------

void ThreadTest2()
{
	DEBUG('t', "Entering Multi-thread-test ");
	list = new DLList();
	for (int i = 0; i < thread; i++)
	{
		char No[4] = "1";
		// create the info
		sprintf(No, "%d", i);
		char *name = new char[25];
		name[0] = '\0';
		strcat(name, "Thread ");
		strcat(name, No);
		Thread *t = new Thread(name);
		t->Fork(TestDllist, i);
	}
}

#ifdef LAB_TWO

/*
test3测试table
*/
void ThreadTest3(){
	DEBUG('t', "Entering Table-test");
	Table*t =new Table(3);//初始化
	for(int i=0;i<3;i++){//分配三个表项
		char No[4] = "1";
		// create the info
		sprintf(No, "%d", i);
		char *name = new char[25];
		name[0] = '\0';
		strcat(name, "Thread ");
		strcat(name, No);
		Thread *test_thread=new Thread(name);
		int x=t->Alloc(test_thread);//x是申请成功后的下标
		ASSERT(x==i);//验证是否申请成功
		printf("allocation %d ok!\n",i);
	}
	for(int i=0;i<3;i++){//查看三个表项
		void *item=t->Get(i);
		if(item){//查看成功
			printf("get :%p \n",item);
		}else{//查看失败
			printf("get failed!\n");
		}
	}
	for(int i=0;i<3;i++){
		t->Release(i);
	}
}
/*
测试缓冲区
*/

void Buffer_producer(int size)
{

	for(int i=0;i<size;i++){
		char *context = new char[size+1];
		for (int j = 0; i < size; ++i){
			int x=Random()%10;
			context[j] = 'a' + x;//随机初始化一个字母
		}
		context[size] = '\0';
		boundedbuffer->Write(context, size);//写入缓冲区
		printf("above all, thread: %s write %s into buffer successfully\n", currentThread->getName(), context);
	}
}

void Buffer_consumer(int size)
{
	for(int i=0;i<size;i++){
		char *context = new char[size+1];
		context[size] = '\0';
		boundedbuffer->Read(context, size);//从缓冲区读
		printf("above all, thread: %s read %s from buffer successfully\n", currentThread->getName(), context);
	}
}

void ThreadTest4(){//先写后读
	printf("Entering Test-Buffer-write-first-read-second\n");
	boundedbuffer = new BoundedBuffer(10);
	for (int i = 0; i < 2; i++)//先分别写入1，2个元素
	{
		char No[4] = "1";
		sprintf(No, "%d", i);
		char *name = new char[25];
		name[0] = '\0';
		strcat(name, "producer thread ");
		strcat(name, No);
		Thread *t = new Thread(name);
		t->Fork(Buffer_producer, i+1);
	}
	for (int i = 0; i < 2; i++)//后分别读出1，2个元素
	{
		char No[4] = "1";
		sprintf(No, "%d", i);
		char *name = new char[25];
		name[0] = '\0';
		strcat(name, "consumer thread ");
		strcat(name, No);
		Thread *t = new Thread(name);
		t->Fork(Buffer_consumer, i+1);
	}
}

void ThreadTest5(){//先读后写
	printf("Entering Test-Buffer-read-first-write-second\n");
	boundedbuffer = new BoundedBuffer(10);
	for (int i = 0; i < 2; i++)//分别读出1，2个元素
	{
		char No[4] = "1";
		sprintf(No, "%d", i);
		char *name = new char[25];
		name[0] = '\0';
		strcat(name, "consumer thread ");
		strcat(name, No);
		Thread *t = new Thread(name);
		t->Fork(Buffer_consumer, i+1);
	}
	for (int i = 0; i < 2; i++)//分别写入1，2个元素
	{
		char No[4] = "1";
		sprintf(No, "%d", i);
		char *name = new char[25];
		name[0] = '\0';
		strcat(name, "producer thread ");
		strcat(name, No);
		Thread *t = new Thread(name);
		t->Fork(Buffer_producer, i+1);
	}
}

void ThreadTest6(){//读写均衡
	DEBUG('t', "Entering Test-Buffer-read-write-balance\n");
	boundedbuffer = new BoundedBuffer(10);
	for(int i=0;i<4;i++){
		if(i%2){//奇数读出2个元素
			char No[4] = "1";
			sprintf(No, "%d", i);
			char *name = new char[25];
			name[0] = '\0';
			strcat(name, "consumer thread ");
			strcat(name, No);
			Thread *t = new Thread(name);
			t->Fork(Buffer_consumer, 2);
		}else{//偶数写入2个元素
			char No[4] = "1";
			sprintf(No, "%d", i);
			char *name = new char[25];
			name[0] = '\0';
			strcat(name, "producer thread ");
			strcat(name, No);
			Thread *t = new Thread(name);
			t->Fork(Buffer_producer, 2);
		}
	}
}

/*
两生产者情况
*/
void test7fun1(int x){
	char *name = new char[25];
	strcat(name, "t0");
	Thread *t = new Thread(name, x);//优先级高
	t->Fork(Buffer_producer, 1);
}

void test7fun2(int x){
	char *name = new char[25];
	strcat(name, "t1");
	Thread *t = new Thread(name, x);//优先级低
	t->Fork(Buffer_producer, 1);
}

void ThreadTest7(){//优先级
	printf("Entering Test-Priority1\n");
	boundedbuffer = new BoundedBuffer(10);
	test7fun1(0);
	test7fun2(1);
}

void ThreadTest8(){//优先级
	printf("Entering Test-Priority2\n");
	boundedbuffer = new BoundedBuffer(10);
	test7fun1(1);
	test7fun2(0);
}

void test7fun3(int x){
	char *name = new char[25];
	strcat(name, "prodeucer-0");
	Thread *t = new Thread(name, x);//优先级低
	t->Fork(Buffer_producer, 1);
}

void test7fun4(int x){
	char *name = new char[25];
	strcat(name, "consumer-0");
	Thread *t = new Thread(name, x);//优先级高
	t->Fork(Buffer_consumer, 2);
}

void ThreadTest9(){//优先级
	printf("Entering Test-Priority1\n");
	boundedbuffer = new BoundedBuffer(10);
	test7fun3(1);
	test7fun4(0);
}
/*
一生产者一消费者，但消费者优先级更高
*/
#endif // LAB_TWO

#ifdef LAB_THREE
void TestEventBarrier_runing(int which)
{
	printf("\033[1;31m%s complete\033[0m\n", currentThread->getName());
	printf("waiters:%d\n", barrier->Waiters());
	barrier->Wait();
	for (int i = 0; i < 1000; ++i)
	{
		for (int j = 0; j < 1000; ++j)
		{
			currentThread->Yield();
			// thread runing;
		}
	}
	barrier->Complete();
	printf("\033[1;32m%s finished\033[0m\n", currentThread->getName());
	printf("waiters:%d\n", barrier->Waiters());
}

void TestEventBarrier_signal(int which)
{
	printf("\033[1;32m%s signaled\033[0m\n", currentThread->getName());
	barrier->Signal();
}

void ThreadTest3()
{
	Thread *tthread[4];
	barrier = new EventBarrier();
	tthread[0] = new Thread("thread 0");
	tthread[1] = new Thread("thread 1");
	tthread[0]->Fork(TestEventBarrier_runing, 0);
	tthread[1]->Fork(TestEventBarrier_runing, 1);
	tthread[2] = new Thread("thread 2");
	tthread[2]->Fork(TestEventBarrier_signal, 2);
}

/*alarm*/

void TestAlarm(int time)
{
	printf("\033[1;34mset %s at %dth tick, timeslide is %d\033[0m\n", currentThread->getName(), stats->totalTicks, time); // 蓝色，set
	Alarm::instance->Pause(time);
	printf("\033[1;32m%s wake up at %dth tick\033[0m\n\n", currentThread->getName(), // 绿色,起床
		   stats->totalTicks);
}
void ThreadTest4()
{
	Thread *tthread[5];
	Alarm::new_instance();
	tthread[0] = new Thread("thread 0");
	tthread[1] = new Thread("thread 1");
	tthread[2] = new Thread("thread 2");
	tthread[3] = new Thread("thread 3");
	tthread[4] = new Thread("thread 4");
	for(int i=0;i<5;i++){
		tthread[i]->Fork(TestAlarm, i+1);
	}
}

/*测试bridge*/
int Total_num;	   // 总测试的车辆数
int direc_0_percent;   // 0 方向占的比例， x/10
int random_come_time = 10; // 随机时间间隔
int sumtime = 0;
int End_num = 0;// 成功过桥的数目
int should_start_time = 0; // 当前进程应当开始的时间
void TestBridge(int which)
{
	int rand_time = Random() % random_come_time; // 随机时间间隔
	should_start_time += rand_time;
	Alarm::instance->Pause(should_start_time);
	int start_time = stats->totalTicks;
	int rand = Random() % 10;
	int direc;
	if (rand < direc_0_percent)
		direc = 0;
	else
		direc = 1;
	printf("+++%s start at %d ticks in %ddirection\n\n", currentThread->getName(), start_time, direc);
	Bridge::instance->OneVehicle(direc);
	printf("---%s costs %d\n\n", currentThread->getName(), stats->totalTicks - start_time);
	sumtime += stats->totalTicks - start_time;
	End_num++;// 成功过桥的数目
	if (End_num == Total_num)
	{
		printf("\n------avg turnaround time %.2f------\n\n", (float)sumtime / Total_num);
#if (defined BASIC_TRAFFIC_LIGHT) || (defined ADV_TRAFFIC_LIGHT)
		Bridge::instance->finished = true;// 停止交通灯管理
#endif
	}
}
void ThreadTest5()
{
	Bridge::new_instance();
	Alarm::new_instance();
	for (int i = 0; i < Total_num; ++i)
	{
		char No[4] = "1";
		sprintf(No, "%d", i);
		char *name = new char[25];
		name[0] = '\0';
		strcat(name, "car ");
		strcat(name, No);
		Thread *t = new Thread(name);
		t->Fork(TestBridge, i);// 参数i实际上没做任何事
	}
}

/*elecator*/

int num_floors;			// 楼层数
int total_riders;		// 生成的总乘客数
int E_random_come_time = 2; // 随机时间间隔
int elevator_num;		// 电梯个数
long long int E_sumtime = 0;// 所有乘客一共用了多久
int E_End_num = 0;// 当前载了几个乘客
int E_should_start_time = 0; // 当前进程应当开始的时间
int max_turnaroundtime = 0;
int min_turnaroundtime = 2147483645;
void TestElevatorRider(int which)
{
	int srcfloor;// 用户起点
	int dstfloor;// 用户终点
	do
	{
		srcfloor = Random() % num_floors;
		dstfloor = Random() % num_floors;
	} while (srcfloor == dstfloor);
	int rand_time = Random() % E_random_come_time; // 随机时间间隔
	E_should_start_time += rand_time;// 随便一个开始时间
	Alarm::instance->Pause(E_should_start_time);
	int start_time = stats->totalTicks;
	printf("\033[1;35m\n%s start at %d ticks from %d floor to %d floor\n\033[0m", currentThread->getName(), start_time, srcfloor, dstfloor);
	rider(which, srcfloor, dstfloor);
	printf("\033[1;35m\n%s costs %d\n\033[0m", currentThread->getName(),stats->totalTicks - start_time);
	int turnaroundtime = stats->totalTicks - start_time;
	if (turnaroundtime > max_turnaroundtime)
		max_turnaroundtime = turnaroundtime;
	if (turnaroundtime < min_turnaroundtime)
		min_turnaroundtime = turnaroundtime;
	E_sumtime += turnaroundtime;
	E_End_num++;
	if (E_End_num == total_riders)
	{
		printf("\n-------avg turnaround time %.2f-------\n",
			   (float)E_sumtime / total_riders);
		printf("-------max turnaround time %d-------\n",
			   max_turnaroundtime);
		printf("-------min turnaround time %d-------\n\n",
			   min_turnaroundtime);
	}
}
void TestElevatorControl(int which)
{
#ifdef MULTIPLE_ELEVATOR
		Building::instance->getElevator(which)->ElevatorControl();
#endif
#ifdef SINGLE_ELEVATOR
	Building::instance->getElevator()->ElevatorControl();
#endif
}
void ThreadTest6()
{
	Building::new_instance("building", num_floors, elevator_num);
	Alarm::new_instance();
#ifdef MULTIPLE_ELEVATOR
	for (int i = 0; i < elevator_num; i++)
	{
		char No[4] = "1";
		sprintf(No, "%d", i);
		char *name = new char[25];
		name[0] = '\0';
		strcat(name, "elevator ");
		strcat(name, No);
		Thread *t = new Thread(name);
		t->Fork(TestElevatorControl, i);
	}
#endif
#ifdef SINGLE_ELEVATOR
	Thread *t = new Thread("elevator");
	t->Fork(TestElevatorControl, 0);
#endif
	for (int i = 0; i < total_riders; ++i)
	{
		char No[4] = "1";
		sprintf(No, "%d", i);
		char *name = new char[25];
		name[0] = '\0';
		strcat(name, "rider ");
		strcat(name, No);
		Thread *t = new Thread(name);
		t->Fork(TestElevatorRider, i);
	}
}
#endif // LAB_THREE

void ThreadTest()
{
	switch (testnum)
	{
#ifdef LAB_TWO
	case 1:
		ThreadTest1();
		break;
	case 2:
		ThreadTest2();
		break;
	case 3:
		ThreadTest3();
		break;
	case 4:
		ThreadTest4();
		break;
	case 5:
		ThreadTest5();
		break;
	case 6:
		ThreadTest6();
		break;
	case 7:
		ThreadTest7();
		break;
	case 8:
		ThreadTest8();
		break;
	case 9:
		ThreadTest9();
		break;
#endif // LAB_TWO
#ifdef LAB_THREE
	case 3:
		ThreadTest3();
		break;
	case 4:
		ThreadTest4();
		break;
	case 5:
		ThreadTest5();
		break;
	case 6:
		ThreadTest6();
		break;
#endif // LAB_THREE
	default:
		printf("No test specified.\n");
		break;
	}
}
