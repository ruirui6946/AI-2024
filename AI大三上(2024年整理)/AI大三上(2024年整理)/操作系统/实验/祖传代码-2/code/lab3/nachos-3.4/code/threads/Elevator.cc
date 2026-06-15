#include "Elevator.h"

extern Alarm* alarm;

Building *building;

void rider(int srcFloor, int dstFloor)
{
    Elevator *e;
    DEBUG('e', "%s: want to go from %d to %d.\n", currentThread->getName(), srcFloor, dstFloor);

    if (srcFloor == dstFloor)
    {
        DEBUG('e', "%s: \033[40;32mfinished\033[0m.\n", currentThread->getName());
        return;
    } 
    
    do
    {
        if(srcFloor < dstFloor)
        {
            e = building->Await(1, srcFloor);
        }
        else if(srcFloor > dstFloor)
        {
            e = building->Await(-1, srcFloor);
        }
    } while (!e->Enter());
    DEBUG('e', "%s: request to go to %d.\n", currentThread->getName(), dstFloor);
    e->RequestFloor(dstFloor); 
    e->Exit();
    DEBUG('e', "%s: \033[40;32mfinished\033[0m.\n", currentThread->getName());
}

void elevator(int id)
{
    Elevator* elevator = building->getElevator(id);
    while(true)
    {
        elevator->Run();
        elevator->OpenDoors();
        elevator->CloseDoors();
    }
}

Elevator::Elevator(char *debugName, int numFloors, int id, int maxCapacity)
{
    name = debugName;       // 电梯名称
    elevatorId = id;        // 电梯id
    num_floor = numFloors;  // 电梯楼层数
    currentFloor = 0;       // 电梯当前停靠的楼层
    direction = 0;          // 电梯运行方向
    capacity = maxCapacity; // 电梯的最大容量
    number = 0;             // 电梯当前乘客数量
    exitElevator = new EventBarrier*[num_floor];    // 各层等待进电梯的事件栅栏
    enterElevator = new EventBarrier*[num_floor];   // 各层等待出电梯的事件栅栏
    elevatorLock = new Lock("Elevator Lock");       // 电梯锁
    request = new Condition("Request Condition");   // 电梯存在请求的条件
    for(int i = 0; i < num_floor; ++ i)
    {
        exitElevator[i] = new EventBarrier("ExitElevator EventBarrier");
        enterElevator[i] = new EventBarrier("EnterElevator EventBarrier"); 
    }
}

Elevator::~Elevator()
{
    delete elevatorLock;
    delete request;
    for(int i = 0; i < num_floor; ++ i)
    {
        delete exitElevator[i];
        delete enterElevator[i];
    }
    delete exitElevator;
    delete enterElevator;
}

// 开门
void Elevator::OpenDoors()
{
    DEBUG('e', "%s: open doors.\n", currentThread->getName());
    // 先下后上
    exitElevator[currentFloor]->Signal();  // 通知乘客出电梯
    DEBUG('s', "%s: signal enter Elevator.\n", currentThread->getName());
    enterElevator[currentFloor]->Signal(); // 通知乘客进电梯
}

// 关门
void Elevator::CloseDoors()
{
    DEBUG('e', "%s: close doors.\n", currentThread->getName());
}

// 电梯运行
void Elevator::Run()
{
    DEBUG('s', "%s: in Run.\n", currentThread->getName());
    if(number == 0) // 电梯无人，应处于空闲状态
    {
        direction = 0;
        building->Signal();
    }
    elevatorLock->Acquire();
    int dstFloor = GetNextFloor(); // 最近的目标楼层
    while(dstFloor == -1) // 判断是否存在请求
    {   
        request->Wait(elevatorLock); // 等待请求
        dstFloor = GetNextFloor();
    }
    elevatorLock->Release();
    while(currentFloor != dstFloor)
    {
        // 移动一层
        ASSERT(direction != 0);
        DEBUG('e', "%s: moving from %d to %d.\n", 
            currentThread->getName(), currentFloor, currentFloor + direction);
        alarm->Pause(50);
        currentFloor += direction;
        DEBUG('e', "%s: arrive %d.\n", currentThread->getName(), currentFloor);
        dstFloor = GetNextFloor(); // 寻找是否存在更近的目标楼层
    }
    if(number == 0) // 电梯接第一个人前，应该改变运行方向
    {
        direction = nextDirec;
    }
    DEBUG('s', "%s: out Run.\n", currentThread->getName());
}

// 进电梯
bool Elevator::Enter()
{
    if(number < capacity || capacity == 0)
    {
        DEBUG('e', "%s: enter Elevator %d in Floor %d.\n", 
            currentThread->getName(), elevatorId, currentFloor);
        ++ number;
        enterElevator[currentFloor]->Complete();
        return true;
    }
    else
    {
        // 该电梯满了，等下一个电梯
        enterElevator[currentFloor]->Complete();
        return false;
    }
}

// 出电梯
void Elevator::Exit()
{
    DEBUG('e', "%s: exit Elevator %d in Floor %d.\n", 
        currentThread->getName(), elevatorId, currentFloor);
    -- number;
    exitElevator[currentFloor]->Complete();
}

// 告诉电梯乘客的目标楼层
void Elevator::RequestFloor(int dstFloor)
{
    DEBUG('s', "%s: in RequestFloor.\n", currentThread->getName());
    elevatorLock->Acquire();
    request->Signal(elevatorLock);  // 发送存在请求的信号
    elevatorLock->Release();
    exitElevator[dstFloor]->Wait(); // 乘客等待电梯到达目标楼层
    DEBUG('s', "%s: out RequestFloor.\n", currentThread->getName());
}

// 判断电梯是否可以响应这个请求
// 可响应条件：电梯无请求或电梯的可能运行路径与请求相符
bool Elevator::IsSatisfiable(int direc, int fromFloor)
{
    if(direction == 0)
    {
        // 如果电梯无请求，那么肯定能响应请求
        return true;
    }
    else if((number == 0 && direc == nextDirec) || (number > 0 && number < capacity && direc == direction))
    {
        // 电梯内无人，说明电梯在去接客的路上，所以要和电梯接客后的方向一致
        // 电梯内有人，只有电梯未满时，才能响应请求，且应该与运行方向一致
        if(direc == 1) return fromFloor >= currentFloor;
        else return fromFloor <= currentFloor;
    }
    return false;
}

// 呼叫并等待这个电梯的到来
void Elevator::CallIn(int direc, int fromFloor)
{
    DEBUG('s', "%s: in CallIn.\n", currentThread->getName());
    elevatorLock->Acquire();
    if(direction == 0 && number == 0)
    {
        // 呼叫到的是一个无请求电梯
        // 为电梯设定运行方向
        if(currentFloor < fromFloor)
        {
            direction = 1;
        } 
        else if (currentFloor > fromFloor)
        {
            direction = -1;
        }
        nextDirec = direc;
        request->Signal(elevatorLock); // 发送存在请求的信号
    }
    elevatorLock->Release();
    enterElevator[fromFloor]->Wait(); // 乘客等待入电梯
    DEBUG('s', "%s: out CallIn.\n", currentThread->getName());
}

// 获得最近的目标楼层
int Elevator::GetNextFloor()
{
    if(direction == 0)
    {
        if(exitElevator[currentFloor]->Waiters() || enterElevator[currentFloor]->Waiters())
        {
            return currentFloor;
        }
        else 
        {
            return -1;
        }
    } 
    for(int i = currentFloor; i >= 0 && i < num_floor; i += direction)
        if(exitElevator[i]->Waiters() || enterElevator[i]->Waiters())
        {
            return i;
        }
    return -1;
}

Building::Building(char *debugName, int numFloors, int numElevators, int maxCapacity)
{
    name = debugName;
    num_floor = numFloors;
    num_elevator = numElevators;
    elevator = new Elevator*[num_elevator];
    for(int id = 0; id < num_elevator; ++ id)
        elevator[id] = new Elevator("Elevator", numFloors, id, maxCapacity);
    buildingLock = new Lock("Building Lock");
    responsible = new Condition("Responsible Condition");
    idx = 0;
}

Building::~Building()
{
    delete buildingLock;
    delete responsible;
    for(int i = 0; i < num_elevator; ++ i)
        delete elevator[i];
    delete elevator;
}

// 呼叫电梯
Elevator *Building::Call(int direc, int fromFloor)
{
    // 遍历每个电梯返回可满足请求的。若没有，返回NULL。
    for(int i = 0; i < num_elevator; ++ i)
    {
        int id = (idx + i) % num_elevator;
        if(elevator[id]->IsSatisfiable(direc, fromFloor))
        {
            idx = id;
            return elevator[id];
        }
    }
    return NULL;
}

// 呼叫并等待电梯的到来
Elevator *Building::Await(int direc, int fromFloor)
{
    DEBUG('s', "%s: in Await.\n", currentThread->getName());
    buildingLock->Acquire();
    // 重复呼叫电梯直到被响应
    Elevator* elev = Call(direc, fromFloor);
    while(elev == NULL)
    {
        responsible->Wait(buildingLock);
        elev = Call(direc, fromFloor);
    }

    buildingLock->Release();
    // 等待响应的电梯到来
    elev->CallIn(direc, fromFloor);
    DEBUG('s', "%s: out Await.\n", currentThread->getName());
    return elev;
}

// 发出存在电梯可以响应的信号
void Building::Signal()
{
    buildingLock->Acquire();
    responsible->Signal(buildingLock);
    buildingLock->Release();
}

Elevator *Building::getElevator(int id)
{
    return elevator[id];
}
