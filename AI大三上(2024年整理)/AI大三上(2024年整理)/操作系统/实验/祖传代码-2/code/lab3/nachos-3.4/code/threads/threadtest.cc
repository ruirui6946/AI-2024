#include <stdlib.h>
#include <time.h>
#include "system.h"
#include "Alarm.h"
#include "Elevator.h"
#include "EventBarrier.h"

extern Alarm* alarm;
extern Building* building;
extern void ArriveBridge(int direc);
extern void CrossBridge(int direc);
extern void ExitBridge(int direc);
extern void rider(int srcFloor, int dstFloor);
extern void elevator(int id);

const int CrossBridgeTime = 100;

static EventBarrier* eventBarrier = new EventBarrier("Test EventBarrier");

static int *fargv;
static int fargc;
static int testNum;


void ThreadInit(int* argv, int argc)
{
   if(argc > 0)
   {
        testNum = argv[0];
        if(argc > 1)
        {
            fargv = argv + 1;
            fargc = argc - 1;
        } 
        else
        {
            fargv = NULL;
            fargc = 0;
        } 
   }
}

int getIntBit(int x)
{
    int n = 0;
    do ++ n, x /= 10; while(x != 0);
    return n;
}

void makeRandThread(void(*func)(int arg), char *kind, int id, int MinPriority)
{
    int n1 = strlen(kind), n2 = getIntBit(id);
    char* name = new char[n1 + n2 + 2];

    name[0] = '\0';
    strcat(name, kind);
    sprintf(name + n1, " %d", id);

    Thread *thread = new Thread(name, rand() % MinPriority);
    thread->Fork(func, id);

    DEBUG('s', "%s: make thread %s %d\n", currentThread->getName(), thread->getName(), thread->getPriority());
}

void makeThread(void(*func)(int arg), char *kind, int id, int priority)
{
    int n1 = strlen(kind), n2 = getIntBit(id);
    char* name = new char[n1 + n2 + 2];

    name[0] = '\0';
    strcat(name, kind);
    sprintf(name + n1, " %d", id);

    Thread *thread = new Thread(name, priority);
    thread->Fork(func, id);

    DEBUG('s', "%s: make thread %s %d\n", currentThread->getName(), thread->getName(), thread->getPriority());
}

void makeThread(void(*func)(int arg), char *kind, int id)
{
    int n1 = strlen(kind), n2 = getIntBit(id);
    char* name = new char[n1 + n2 + 2];

    name[0] = '\0';
    strcat(name, kind);
    sprintf(name + n1, " %d", id);

    Thread *thread = new Thread(name);
    thread->Fork(func, id);

    DEBUG('s', "%s: make thread %s\n", currentThread->getName(), thread->getName());
}

void test1_func1(int which)
{
    DEBUG('e', "%s: call Wait.\n", currentThread->getName());
    eventBarrier->Wait();
    DEBUG('e', "%s: call Complete.\n", currentThread->getName());
    eventBarrier->Complete();
    DEBUG('e', "%s: finish.\n", currentThread->getName());
}

void test1_func2(int which)
{
    DEBUG('e', "%s: call Signal.\n", currentThread->getName());
    eventBarrier->Signal();
    DEBUG('e', "%s: finish.\n", currentThread->getName());
}

void test1_func3(int which)
{
    DEBUG('e', "%s: call Wait.\n", currentThread->getName());
    eventBarrier->Wait();
    DEBUG('e', "%s: call Complete.\n", currentThread->getName());
    eventBarrier->Complete();
    DEBUG('e', "%s: call Wait.\n", currentThread->getName());
    eventBarrier->Wait();
    DEBUG('e', "%s: finish.\n", currentThread->getName());
}

void test1()
{
    makeThread(test1_func2, "Test", 0);
    makeThread(test1_func3, "Test", 1);
    makeThread(test1_func3, "Test", 2);
    makeThread(test1_func1, "Test", 3);
    makeThread(test1_func2, "Test", 4);
    makeThread(test1_func3, "Test", 5);
    makeThread(test1_func1, "Test", 6);
    makeThread(test1_func1, "Test", 7);
    makeThread(test1_func3, "Test", 8);
}

void callSignal(int which)
{
    int MaxWaitTime = fargv[2];
    int waitTime = rand() % MaxWaitTime + 1;
    alarm->Pause(waitTime);
    DEBUG('e', "%s: Signal.\n", currentThread->getName());
    eventBarrier->Signal();
    DEBUG('e', "%s: finish.\n", currentThread->getName());
}

void callWait(int which)
{
    int MaxWaitTime = fargv[2];
    int waitTime = rand() % MaxWaitTime + 1;
    alarm->Pause(waitTime);
    DEBUG('e', "%s: call Wait.\n", currentThread->getName());
    eventBarrier->Wait();
    DEBUG('e', "%s: call Complete.\n", currentThread->getName());
    eventBarrier->Complete();
    DEBUG('e', "%s: finish.\n", currentThread->getName());
}

void test2()
{
    ASSERT(fargc == 3);
    srand(time(0));
    int n1 = fargv[0], n2 = fargv[1];
    for(int i = 0; i < n1; ++ i)
        makeThread(callSignal, "Signal", i);
    for(int i = 0; i < n2; ++ i)
        makeThread(callWait, "Wait", i);
}

void alarmTest(int which)
{
    int maxTime = fargv[1];
    int howLong = rand() % maxTime + 1;
    alarm->Pause(howLong);
}

void test3()
{
    ASSERT(fargc == 2);
    int n = fargv[0];
    srand(time(0));
    for(int i = 0; i < n; ++ i)
        makeThread(alarmTest, "AlarmTest", i);
}

void OneVehicle(int direc)
{
    DEBUG('e', "%s: arrve %d.\n", currentThread->getName(), direc);
    ArriveBridge(direc);
    DEBUG('e', "%s: enter bridge.\n", currentThread->getName()); 
    alarm->Pause(CrossBridgeTime);
    CrossBridge(direc); 
    ExitBridge(direc);
}

void car0(int which)
{
    OneVehicle(0);
}

void car1(int which)
{
    OneVehicle(1);
}

void test4()
{
    makeThread(car0, "Car", 0);
    makeThread(car0, "Car", 1);
    makeThread(car1, "Car", 2);
    makeThread(car0, "Car", 3);
}

void car(int which)
{
    int MaxWaitTime = fargv[1];
    int waitTime = rand() % MaxWaitTime + 1;
    alarm->Pause(waitTime);
    OneVehicle(rand() % 2);
    DEBUG('e', "%s: \033[40;32mfinished\033[0m.\n", currentThread->getName());
}

void test5()
{
    ASSERT(fargc == 2);
    srand(time(0));
    int n = fargv[0];
    for(int i = 0; i < n; ++ i)
        makeThread(car, "Car", i);
}

void Rider(int which)
{
    int numFloor = fargv[0], OtherTime = fargv[4];
    alarm->Pause(rand() % OtherTime + 1);
    rider(rand() % numFloor, rand() % numFloor);
}

void test6()
{
    ASSERT(fargc == 5);
    srand(time(0));
    int numFloor = fargv[0], numElevator = fargv[1], capacity = fargv[2], numRider = fargv[3];
    building = new Building("Building", numFloor, numElevator, capacity);
    for(int i = 0; i < numElevator; ++ i)
        makeThread(elevator, "Elevator", i);
    for(int i = 0; i < numRider; ++ i)
        makeThread(Rider, "Rider", i);
}

void ThreadTest()
{
    switch(testNum)
    {
        case 1:
            test1();
            break;
        case 2:
            test2();
            break;
        case 3:
            test3();
            break;
        case 4:
            test4();
            break;
        case 5:
            test5();
            break;
        case 6:
            test6();
            break;
        default:
            ASSERT(false);
            break;   
    }
}
