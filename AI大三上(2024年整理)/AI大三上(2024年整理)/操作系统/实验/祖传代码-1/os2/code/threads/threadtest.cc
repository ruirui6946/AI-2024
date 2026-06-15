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

DLList *list;
BoundedBuffer *boundedbuffer;
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


//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void ThreadTest()
{
	switch (testnum)
	{
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
	default:
		printf("No test specified.\n");
		break;
	}
}
