/*

Here are the method signatures for the Elevator and Building classes.
You should feel free to add to these classes, but do not change the
existing interfaces.

*/

// Elevator.h
// 
#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "EventBarrier.h"


const int MAX_ELE_VOLUMN = 3;

EventBarrier **waitEleUp;
EventBarrier **waitEleDown;


class Elevator {
   public:
     Elevator(char *debugName, int numFloors, int myID, int *FloorUpEleId, int *FloorDownEleId);
     ~Elevator();
     char *getName() { return name; }
   
     // elevator control interface: called by Elevator thread
     void OpenDoors();                //   signal exiters and enterers to action
     void CloseDoors();               //   after exiters are out and enterers are in
     void VisitFloor(int floor);      //   go to a particular floor
   
     int checkout();
     // elevator rider interface (part 1): called by rider threads.
     bool Enter();                    //   get in
     bool Exit();                     //   get out (iff destinationFloor)
     void RequestFloor(int floor);    //   tell the elevator our destinationFloor

     void initialize();
   
     // insert your methods here, if needed

     int currentfloor;           // floor where currently stopped
     int direction;              // tells the direction of elevator, 0 stands for still, 1 stands for up, -1 stands for down.
     int *FloorToGo;             // which floor our elevator is expected to visit

   private:
     char *name;
     int eleID;                  // Elevator ID

     int maxFloor;               // maxFloor for elevator, elevator from 0 to maxFloor
     
     int occupancy;              // how many riders currently onboard
     int maxVolumn;              // the most people an elevator can carry


     int *floorUpID;
     int *floorDownID;

     enum STATE {running, stop} state;

     EventBarrier *NoOneInOrOut;
     EventBarrier *doorsOpen;
     // insert your data structures here, if needed
};
   
class Building {
   public:
     Building(char *debugname, int numFloors, int numElevators);
     ~Building();
     char *getName() { return name; }
   
   				
     // elevator rider interface (part 2): called by rider threads
     void CallUp(int fromFloor);      //   signal an elevator we want to go up
     void CallDown(int fromFloor);    //   ... down
     Elevator *AwaitUp(int fromFloor); // wait for elevator arrival & going up
     Elevator *AwaitDown(int fromFloor); // ... down

     int chooseBestElevator(int fromFloor, int direction);
     void addToEleStop(int eleID, int fromFloor, int dir);
   
   private:
     char *name;
     int eleNum;                 // how many elevators do we have
     int floors;                 // how many floors do we have in this building
     Elevator **elevator;         // the elevators in the building (array)

    int *FloorUpEleId;            // indicate which up elevator stoped at certain floor
    int *FloorDownEleId;          // indicate which down elevator stoped at certain floor

   
     // insert your data structures here, if needed
};


#endif
   // here's a sample portion of a rider thread body showing how we
   // expect things to be called.
   //
   // void rider(int id, int srcFloor, int dstFloor) {
   //    Elevator *e;
   //  
   //    if (srcFloor == dstFloor)
   //       return;
   //  
   //    DEBUG('t',"Rider %d travelling from %d to %d\n",id,srcFloor,dstFloor);
   //    do {
   //       if (srcFloor < dstFloor) {
   //          DEBUG('t', "Rider %d CallUp(%d)\n", id, srcFloor);
   //          building->CallUp(srcFloor);
   //          DEBUG('t', "Rider %d AwaitUp(%d)\n", id, srcFloor);
   //          e = building->AwaitUp(srcFloor);
   //       } else {
   //          DEBUG('t', "Rider %d CallDown(%d)\n", id, srcFloor);
   //          building->CallDown(srcFloor);
   //          DEBUG('t', "Rider %d AwaitDown(%d)\n", id, srcFloor);
   //          e = building->AwaitDown(srcFloor);
   //       }
   //       DEBUG('t', "Rider %d Enter()\n", id);
   //    } while (!e->Enter()); // elevator might be full!
   //  
   //    DEBUG('t', "Rider %d RequestFloor(%d)\n", id, dstFloor);
   //    e->RequestFloor(dstFloor); // doesn't return until arrival
   //    DEBUG('t', "Rider %d Exit()\n", id);
   //    e->Exit();
   //    DEBUG('t', "Rider %d finished\n", id);
   // }

