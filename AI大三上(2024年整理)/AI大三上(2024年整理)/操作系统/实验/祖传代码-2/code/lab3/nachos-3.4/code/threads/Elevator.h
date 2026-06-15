#ifndef ELEVATOR_H
#define ELEVATOR_H
#include "system.h"
#include "EventBarrier.h"
#include "copyright.h"
#include "synch.h"
#include "Alarm.h"
class Elevator
{
public:
    Elevator(char *debugName, int numFloors, int id, int maxCapacity);
    ~Elevator();
    char *GetName() { return name; }

    // 由电梯线程调用
    void OpenDoors();   // 开门
    void CloseDoors();  // 关门
    void Run();  // 电梯运行
    bool IsSatisfiable(int direc, int fromFloor); // 判断电梯是否可以响应这个请求

    // 由乘客线程调用
    bool Enter();   // 进电梯
    void Exit();    // 出电梯
    void RequestFloor(int dstFloor);        // 告诉电梯乘客的目标楼层
    void CallIn(int direc,int fromFloor);   // 呼叫并等待这个电梯的到来
private:
    int GetNextFloor(); // 获得最近的目标楼层

    char *name;         // 电梯名称
    int elevatorId;     // 电梯id
    int num_floor;      // 电梯楼层数
    int currentFloor;   // 电梯当前停靠的楼层
    int direction;      // 电梯正在运行方向
    int nextDirec;      // 暂存有乘客后的要运行的方向
    int capacity;       // 电梯的最大容量
    int number;         // 电梯当前乘客数量
    EventBarrier **exitElevator;    // 各层等待进电梯的事件栅栏
    EventBarrier **enterElevator;   // 各层等待出电梯的事件栅栏
    Lock* elevatorLock;             // 电梯锁
    Condition* request;             // 电梯存在请求的条件
};

class Building
{
public:
    Building(char *debugName, int numFloors, int numElevators, int maxCapacity);
    ~Building();
    char *getName() { return name; }
    Elevator* getElevator(int id); // 获得特定的电梯
    Elevator *Await(int direc, int fromFloor); // 呼叫并等待电梯的到来
    void Signal();  // 发出存在电梯可以响应的信号
private:
    Elevator *Call(int direc, int fromFloor);  // 呼叫电梯

    char *name;             // 建筑物名称
    int num_floor;          // 楼层数量
    int num_elevator;       // 电梯数量
    Elevator **elevator;    // 电梯列表
    Lock* buildingLock;     // 建筑锁
    Condition* responsible; // 存在可响应电梯条件
    int idx;
};
#endif
