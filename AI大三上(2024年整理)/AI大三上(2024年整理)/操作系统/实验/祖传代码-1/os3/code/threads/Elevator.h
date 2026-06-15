#ifndef ELEVATOR_H
#define ELEVATOR_H
#include "EventBarrier.h"
#include "system.h"
#include "synch.h"

// #define SINGLE_ELEVATOR
#define MULTIPLE_ELEVATOR // 单个或多个电梯
#define BOUNDED_ELEVATOR
// #define UNBOUNDED_ELEVATOR //电梯容量是否有限
#define OPEN_AND_CLOSE_DOOR 20     // 电梯开关门的时间
#define RIDER_ENTER_OUT 10         // 一个乘客进出电梯的时间
#define ELEVATOR_MOVE_ONE_FLOOR 40 // 电梯移动一层楼的时间
#ifdef BOUNDED_ELEVATOR
#define ELEVATOR_CAPACITY 3 // 电梯容量
#endif
enum ElevatorState
{
  UP,
  DOWN,
  STOP
};
void rider(int id, int srcFloor, int dstFloor);
class Elevator
{
public:
  friend class Building;
  Elevator(char *debugName, int numFloors, int myID);
  ~Elevator();
  char *getName() { return name; }
  int getId() { return ElevatorID; }
  // elevator control interface: called by Elevator thread
  void OpenDoors();           // signal exiters and enterers to action
  void CloseDoors();          // after exiters are out and enterers are in
  void VisitFloor(int floor); // go to a particular floor
  // elevator rider interface (part 1): called by rider threads.
  bool Enter();                 // get in
  void Exit();                  // get out (iff destinationFloor)
  void RequestFloor(int floor); // tell the elevator our destinationFloor
  // insert your methods here, if needed
  void ElevatorControl(); // 控制电梯运行
  int GetLastRequestFloor();

private:
  char *name;
  int currentfloor; // floor where currently stopped
  int occupancy;    // how many riders currently onboard
  // insert your data structures here, if needed
  int numFloors, ElevatorID;
  ElevatorState state;
  EventBarrier **ElevatorOutBarrier; // 出电梯的事件栅栏
  Lock *ElevatorLock;
  Condition *HaveRequest;
#ifdef MULTIPLE_ELEVATOR
  bool canEnter;
#endif
#ifdef BOUNDED_ELEVATOR
  Condition *ElevatorNotFull;
#endif
};
class Building
{
public:
  friend class Elevator;
  Building(char *debugname, int numFloors, int numElevators);
  ~Building();
  char *getName() { return name; }
  // elevator rider interface (part 2): called by rider threads
  void CallUp(int fromFloor);         // signal an elevator we want to go up
  void CallDown(int fromFloor);       // ... down
  Elevator *AwaitUp(int fromFloor);   // wait for elevator arrival & going up
  Elevator *AwaitDown(int fromFloor); // ... down
#ifdef SINGLE_ELEVATOR
  Elevator *getElevator();
#endif
#ifdef MULTIPLE_ELEVATOR
  Elevator *getElevator(int id);
#endif
  static void new_instance(char *debugname, int numFloors, int numElevators);
  static Building *instance;

private:
  char *name;
#ifdef SINGLE_ELEVATOR
  Elevator *elevator; // the elevators in the building (array)
#endif
#ifdef MULTIPLE_ELEVATOR
  Elevator **elevator;
#endif
  EventBarrier **ElevatorUpBarrier;   // 上行栅栏
  EventBarrier **ElevatorDownBarrier; // 下行栅栏
  // insert your data structures here, if needed
  int numFloors;
  int numElevators;
  Lock *BuildingLock;
};
#endif // ELEVATOR
