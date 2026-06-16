#include "copyright.h"
#include "system.h"
#include "Elevator.h"

// testnum is set in main.cc
int testnum = 1;

//Building *building;

// AlarmTest used to test alarm, in this function we called Pause 
void RiderTest(int dummy){
        
  /*  int id = 1;
    int srcFloor = 1;
    int dstFloor = 2;

    Elevator *e;
     
    if (srcFloor == dstFloor)
        return;
     
    DEBUG('t',"Rider %d travelling from %d to %d\n",id,srcFloor,dstFloor);
       do {
          if (srcFloor < dstFloor) {
             DEBUG('t', "Rider %d CallUp(%d)\n", id, srcFloor);
             building->CallUp(srcFloor);
             DEBUG('t', "Rider %d AwaitUp(%d)\n", id, srcFloor);
             e = building->AwaitUp(srcFloor);
          } else {
             DEBUG('t', "Rider %d CallDown(%d)\n", id, srcFloor);
             building->CallDown(srcFloor);
             DEBUG('t', "Rider %d AwaitDown(%d)\n", id, srcFloor);
             e = building->AwaitDown(srcFloor);
          }
          DEBUG('t', "Rider %d Enter()\n", id);
       } while (!e->Enter());  // elevator might be full!
     
       DEBUG('t', "Rider %d RequestFloor(%d)\n", id, dstFloor);
       e->RequestFloor(dstFloor);  // doesn't return until arrival
       DEBUG('t', "Rider %d Exit()\n", id);
       e->Exit();
       DEBUG('t', "Rider %d finished\n", id);

       currentThread->Yield();*/
}



void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    // create two thread, to make result cleaner.
    Thread *t1 = new Thread("Thread 1");
    // Thread *t2 = new Thread("Thread 2");

    t1->Fork(RiderTest, 1);
    //t2->Fork(loopFunction, 2);

    //building = new Building("danger buidling233", 5, 1);
}

//----------------------------------------------------------------------
// ThreadTest
//  Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
    ThreadTest1();
    break;

    default:
    printf("No test specified.\n");
    break;
    }
}