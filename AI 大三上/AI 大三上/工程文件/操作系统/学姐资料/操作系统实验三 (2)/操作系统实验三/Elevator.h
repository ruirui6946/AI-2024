/*

Here are the method signatures for the Elevator and Building classes.
You should feel free to add to these classes, but do not change the
existing interfaces.

*/
#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "system.h"
#include "synch.h"
#include "EventBarrier.h"

#define UP 1
#define SLEEP 0
#define DOWN -1

class Elevator
{
   public:
     Elevator(char *debugName,int numFloors, int myID);
     ~Elevator();
     char *getName() { return name; }

     // elevator control interface: called by Elevator thread
     void OpenDoors();                // 电梯开门，给离开者和进入者发信号表示要开始行动了
     void CloseDoors();               // 电梯在离开者下电梯、进入者上电梯后关门
     void VisitFloor(int floor);      // 电梯去指定楼层

     // elevator rider interface (part 1): called by rider threads.
     bool Enter();                    //  进入电梯
     void Exit();                     //  离开电梯
     void RequestFloor(int floor);    //  告诉电梯自己要去的楼层

     // insert your methods here, if needed
     void StartElevator(); // 电梯启动

   //private:
   public:
     char *name;
     int currentfloor;		// 电梯目前停留的楼层
     int occupancy;		// 电梯实载人数

     // insert your data structures here, if needed
     int NumFloors;		// 楼层数
     int ID;			// 电梯序号
     int status;		// 电梯状态，向上、向下、暂停
     int Capacity;		// 电梯可容纳人数
     bool overload;		// 电梯是否超载
     EventBarrier **exitBarrier;//确保要下电梯的乘客全部离开
     bool *exitRequest;//离开请求
};



class Building
{
   public:
     Building(char *debugname, int numFloors, int numElevators);
     ~Building();
     char *getName() { return name; }


     // elevator rider interface (part 2): called by rider threads
     void CallUp(int fromFloor);      // 发出信号要从当前楼层乘坐电梯向上
     void CallDown(int fromFloor);    // 发出信号要从当前楼层乘坐电梯向下
     Elevator *AwaitUp(int fromFloor); // 在当前楼层等待电梯到来并向上
     Elevator *AwaitDown(int fromFloor); // 在当前楼层等待电梯到来并向下

     void RecordCall(int fromFloor, int direction);	// 记录乘客的请求
     int HandleCall();	// 电梯处理第一个请求，根据先到先服务原则
     void HandleFloor(int fromFloor, int direction);// 处理队列中所有处于当前楼层且同方向的请求

   //private:
   public:
     char *name;
     char **elevatorname;  //电梯名
     Elevator **elevator;	// 电梯

     // insert your data structures here, if needed
     int NumFloors;		// 楼层数
     int NumElevators;  // 电梯数
     EventBarrier **upBarrier;	// 确保同楼层所有乘客乘电梯到上楼
     EventBarrier **downBarrier;// 确保同楼层所有乘客乘电梯到下楼
     bool *callUp, *callDown;	// 在某一楼层是否有请求叫电梯（向上、向下）
     int *elevatorUp, *elevatorDown;	// 记录向上向下的电梯的序号
     Condition *suspend_cond;	// 空的电梯被挂起在条件变量suspend_cond上
     int *callQueue;// 用于记录所有rider的请求，队列中每个元素的值为rider所在的楼层和方向的乘积，
                    //正值表示向上，负值表示向下。
     int end;		// 标记callQueue的结尾
};

/*当某一层楼有一个或者一个以上的乘客要乘坐电梯时，便有一个请求信号给电梯，电梯便响应，并设置相应的电梯状态。
电梯移动过程中乘客便阻塞在相应楼层，当电梯到达后，电梯开门，发出信号将乘客从事件栅栏中释放，电梯自己阻塞，
乘客进入电梯或者出电梯，若乘客是最后一个进电梯的或出电梯的则发出信号唤醒电梯。
然后电梯关门，乘客在电梯里请求要去的目的楼层，并阻塞，直到当电梯到达所要去的目的楼层时，
电梯开门，将乘客从条件变量释放。乘客出电梯，至此，一个乘客线程结束。*/


#endif
