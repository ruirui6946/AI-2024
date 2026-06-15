#include "Elevator.h"
#include "Alarm.h"
Building *Building::instance;
#ifdef SINGLE_ELEVATOR
Elevator *Building::getElevator()
{
    return elevator;
}
void Building::new_instance(char *debugname, int numFloors, int numElevators)
{
    instance = new Building(debugname, numFloors, numElevators);
}
Building::Building(char *debugname, int numFloors, int numElevators)
{
    name = debugname;
    this->numFloors = numFloors;
    this->numElevators = numElevators;
    elevator = new Elevator("elevator", numFloors, 0);// 因为只有一个电梯，所以id是0
    BuildingLock = new Lock("Building lock");
    ElevatorUpBarrier = new EventBarrier *[numFloors];// 每层楼两个栅栏
    ElevatorDownBarrier = new EventBarrier *[numFloors];
    for (int i = 0; i < numFloors; ++i)
    {
        ElevatorUpBarrier[i] = new EventBarrier;
        ElevatorDownBarrier[i] = new EventBarrier;
    }
}
Building::~Building()
{
    delete elevator;
    delete BuildingLock;
    for (int i = 0; i < numFloors; ++i)
    {
        delete ElevatorUpBarrier[i];
        delete ElevatorDownBarrier[i];
    }
    delete[] ElevatorUpBarrier;
    delete[] ElevatorDownBarrier;
}
Elevator *Building::AwaitDown(int fromFloor)
{
    ElevatorDownBarrier[fromFloor]->Wait();// 休眠，并伺机而动
    return elevator;
}
Elevator *Building::AwaitUp(int fromFloor)
{
    ElevatorUpBarrier[fromFloor]->Wait();
    return elevator;
}
void Building::CallDown(int fromFloor)
{
    elevator->ElevatorLock->Acquire();
    elevator->HaveRequest->Signal(elevator->ElevatorLock);
    elevator->ElevatorLock->Release();
}
void Building::CallUp(int fromFloor)
{
    elevator->ElevatorLock->Acquire();
    elevator->HaveRequest->Signal(elevator->ElevatorLock);
    elevator->ElevatorLock->Release();
}
#endif // SINGLE_ELEVATOR   
#ifdef MULTIPLE_ELEVATOR
Elevator *
Building::getElevator(int id)
{
    return elevator[id];
}
void Building::new_instance(char *debugname, int numFloors, int numElevators)
{
    instance = new Building(debugname, numFloors, numElevators);
}
Building::Building(char *debugname, int numFloors, int numElevators)
{
    name = debugname;
    this->numFloors = numFloors;
    this->numElevators = numElevators;
    ElevatorUpBarrier = new EventBarrier *[numFloors];
    ElevatorDownBarrier = new EventBarrier *[numFloors];
    elevator = new Elevator *[numElevators];
    for (int i = 0; i < numElevators; ++i)
    {
        elevator[i] = new Elevator("elevator", numFloors, i);
    }
    for (int i = 0; i < numFloors; ++i)
    {
        ElevatorUpBarrier[i] = new EventBarrier;
        ElevatorDownBarrier[i] = new EventBarrier;
    }
    BuildingLock = new Lock("Building lock");
}
Building::~Building()
{
    for (int i = 0; i < numElevators; ++i)
    {
        delete elevator[i];
    }
    for (int i = 0; i < numFloors; ++i)
    {
        delete ElevatorUpBarrier[i];
        delete ElevatorDownBarrier[i];
    }
    delete[] elevator;
    delete[] ElevatorUpBarrier;
    delete[] ElevatorDownBarrier;
    delete BuildingLock;
}
Elevator *Building::AwaitDown(int fromFloor)
{
    ElevatorDownBarrier[fromFloor]->Wait();
    BuildingLock->Acquire();
    Elevator *re;
    for (int i = 0; i < numElevators; ++i)
    {
        if (elevator[i]->currentfloor == fromFloor && (elevator[i]->state == DOWN || elevator[i]->state == STOP) && elevator[i]->canEnter)
        {
            re = elevator[i];
            break;
        }
    }
    BuildingLock->Release();
    return re;
}
Elevator *Building::AwaitUp(int fromFloor)
{
    ElevatorUpBarrier[fromFloor]->Wait();
    BuildingLock->Acquire();
    Elevator *re;
    for (int i = 0; i < numElevators; ++i)
    {
        if (elevator[i]->currentfloor == fromFloor &&
         (elevator[i]->state == UP || elevator[i]->state == STOP) && 
         elevator[i]->canEnter)
        {
            re = elevator[i];
            break;
        }
    }
    BuildingLock->Release();
    return re;
}
void Building::CallDown(int fromFloor)
{
    BuildingLock->Acquire();
    int MinInterval = numFloors + 1;
    int eleNum = -1;
    for (int i = 0; i < numElevators; ++i)
    {
        if (elevator[i]->state == STOP && 
        abs(elevator[i]->currentfloor - fromFloor) < MinInterval)
        {
            MinInterval = abs(elevator[i]->currentfloor - fromFloor);
            eleNum = i;
        }
    }
    if (eleNum != -1)
    {
        elevator[eleNum]->HaveRequest->Signal(BuildingLock);
    }
    BuildingLock->Release();
}
void Building::CallUp(int fromFloor)
{
    BuildingLock->Acquire();
    int MinInterval = numFloors + 1;
    int eleNum = -1;
    for (int i = 0; i < numElevators; ++i)
    {
        if (elevator[i]->state == STOP && 
        abs(elevator[i]->currentfloor - fromFloor) < MinInterval)
        {
            MinInterval = abs(elevator[i]->currentfloor - fromFloor);
            eleNum = i;
        }
    }
    if (eleNum != -1)
    {
        elevator[eleNum]->HaveRequest->Signal(BuildingLock);
    }
    BuildingLock->Release();
}
#endif // MULTIPLE_ELEVATOR
Elevator::Elevator(char *debugName, int numFloors, int myID)
{
    name = debugName;
    this->numFloors = numFloors;
    ElevatorID = myID;
    ElevatorOutBarrier = new EventBarrier *[numFloors];
    for (int i = 0; i < numFloors; ++i)
    {
        ElevatorOutBarrier[i] = new EventBarrier;
    }
    ElevatorLock = new Lock("Elevator lock");
    state = STOP;
    currentfloor = 0;
    HaveRequest = new Condition("have a elevator requset");
    occupancy = 0;
#ifdef MULTIPLE_ELEVATOR
    canEnter = false;
#endif
#ifdef BOUNDED_ELEVATOR
    ElevatorNotFull = new Condition("elevator is not full");
#endif
}
Elevator::~Elevator()
{
    for (int i = 0; i < numFloors; ++i)
    {
        delete ElevatorOutBarrier[i];
    }
    delete[] ElevatorOutBarrier;
    delete[] ElevatorLock;
    delete HaveRequest;
#ifdef BOUNDED_ELEVATOR
    delete ElevatorNotFull;
#endif
}
void Elevator::OpenDoors()
{
    Alarm::instance->Pause(OPEN_AND_CLOSE_DOOR);
    printf("\033[1;32mElevator %d open door at %d floor with %d riders\n\033[0m", ElevatorID, currentfloor, occupancy);
    if (ElevatorOutBarrier[currentfloor]->Waiters() != 0)
    {
        ElevatorOutBarrier[currentfloor]->Signal();
    }
#ifdef MULTIPLE_ELEVATOR
    canEnter = true;
#endif
    if (state == UP)
    {
        if (Building::instance->ElevatorUpBarrier[currentfloor]->Waiters() != 0)
        {
            Building::instance->ElevatorUpBarrier[currentfloor]->Signal();
        }
    }
    else if (state == DOWN)
    {
        if (Building::instance->ElevatorDownBarrier[currentfloor]->Waiters() != 0)
        {
            Building::instance->ElevatorDownBarrier[currentfloor]->Signal();
        }
    }
}
void Elevator::CloseDoors()
{
#ifdef MULTIPLE_ELEVATOR
    canEnter = false;
#endif
    printf("\033[1;32mElevator %d close door at %d floor with %d riders\033[0m\n\n", ElevatorID, currentfloor, occupancy);
    Alarm::instance->Pause(OPEN_AND_CLOSE_DOOR);
}
void Elevator::VisitFloor(int floor)
{
#ifdef BOUNDED_ELEVATOR
    ElevatorLock->Acquire();
    ElevatorNotFull->Broadcast(ElevatorLock);
    ElevatorLock->Release();
#endif
    Alarm::instance->Pause(ELEVATOR_MOVE_ONE_FLOOR);
    currentfloor = floor;
}
bool Elevator::Enter()
{
    occupancy++;
#ifdef BOUNDED_ELEVATOR
    if (occupancy > ELEVATOR_CAPACITY)
    {
        occupancy--;
        if (state == UP)// 保证complete()的数目和Wait相同
        {
            Building::instance->ElevatorUpBarrier[currentfloor]->Complete();
        }
        else if (state == DOWN)
        {
            Building::instance->ElevatorDownBarrier[currentfloor]->Complete();
        }
        ElevatorLock->Acquire();
        ElevatorNotFull->Wait(ElevatorLock);// 阻塞超载
        ElevatorLock->Release();
        return false;// 用于换乘
    }
#endif
    Alarm::instance->Pause(RIDER_ENTER_OUT);
    if (state == UP)
    {
        Building::instance->ElevatorUpBarrier[currentfloor]->Complete();
    }
    else if (state == DOWN)
    {
        Building::instance->ElevatorDownBarrier[currentfloor]->Complete();
    }
    return true;
}
void Elevator::Exit()
{
    Alarm::instance->Pause(RIDER_ENTER_OUT);
    occupancy--;
    ElevatorOutBarrier[currentfloor]->Complete();
}
void Elevator::RequestFloor(int floor)
{
    ElevatorOutBarrier[floor]->Wait();
}
int Elevator::GetLastRequestFloor()// 确定目标
{
    ElevatorLock->Acquire();
    int return_value = -1;// 最开始是-1
    if (state == STOP)
    {
        for (int i = 0; i < numFloors; ++i)
        {
            if (Building::instance->ElevatorUpBarrier[i]->Waiters() != 0 || ElevatorOutBarrier[i]->Waiters() != 0 ||
                Building::instance->ElevatorDownBarrier[i]->Waiters() != 0)
            {
                return_value = i;
                break;
            }
        }
    }
    else if (state == UP)
    {
        for (int i = numFloors - 1; i >= currentfloor; --i)
        {
            if (Building::instance->ElevatorDownBarrier[i]->Waiters() !=
                    0 ||
                ElevatorOutBarrier[i]->Waiters() != 0 || Building::instance->ElevatorUpBarrier[i]->Waiters() != 0)
            {
                return_value = i;
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i <= currentfloor; ++i)
        {
            if (Building::instance->ElevatorDownBarrier[i]->Waiters() !=
                    0 ||
                ElevatorOutBarrier[i]->Waiters() != 0 || Building::instance->ElevatorUpBarrier[i]->Waiters() != 0)
            {
                return_value = i;
                break;
            }
        }
    }
    ElevatorLock->Release();
    return return_value;
}
void Elevator::ElevatorControl()// 最核心函数
{
    bool no_requset_flag = false;
    while (true)
    {
        int dest_floor = GetLastRequestFloor();
        if (state == STOP)
        {
            ElevatorLock->Acquire();
            printf("\033[1;32mElevator %d stop at %d floor with %d riders\n\033[0m", ElevatorID, currentfloor, occupancy);
            if (dest_floor == -1)
            {
                HaveRequest->Wait(ElevatorLock);// 等人来
            }
            ElevatorLock->Release();
            printf("\033[1;32mElevator %d at %d floor with %d riders\n\033[0m", ElevatorID, currentfloor, occupancy);
            dest_floor = GetLastRequestFloor();// 等待电梯目标，没有目标的话就会奔着-1层去，-1的情况会被特殊处理
            if (dest_floor > currentfloor)
                state = UP;
            else
                state = DOWN;
        }
        else if (state == UP)
        {
            while (currentfloor != dest_floor && dest_floor != -1)
            {
                no_requset_flag = false;
                if (Building::instance->ElevatorUpBarrier[currentfloor]->Waiters() != 0 
                || ElevatorOutBarrier[currentfloor]->Waiters() != 0)
                {// 我是上升的电梯，你是上升的乘客或者要出电梯的乘客
                // 根据常识，此时遇到下行的乘客，电梯不予理睬
                    OpenDoors();
                    CloseDoors();
                }
                VisitFloor(currentfloor + 1);
                dest_floor = GetLastRequestFloor();
                printf("\033[1;32mElevator %d at %d floor with %d riders\n\033[0m", 
                ElevatorID, currentfloor, occupancy);
            }
            if (dest_floor == -1) // 有多个电梯时，若请求被其他电梯满足，则会被取消，故需在此处判断
            {
                if (no_requset_flag) // 若在上一个循环中判断的另外一个方向也无请求
                {
                    state = STOP;
                    continue;
                }
                state = DOWN;// 不知道去哪，换个方向吧
                no_requset_flag = true;
                continue;
            }
            if (Building::instance->ElevatorUpBarrier[currentfloor]->Waiters() == 0)
            {// 到达目的地了，没乘客就调头，看另一个方向有无乘客
                state = DOWN; // 调头
                OpenDoors();
                CloseDoors();
                dest_floor = GetLastRequestFloor();
                if (dest_floor == -1) // 若 DOWN方向上也无新的请求则使电梯停下
                    state = STOP;
                else // 否则向 DOWN 方向运行
                    VisitFloor(currentfloor - 1);
            }
            else // 有想从当前层向上的
            {
                OpenDoors();
                CloseDoors();
                VisitFloor(currentfloor + 1);
            }
            printf("\033[1;32mElevator %d at %d floor with %d riders\n\033[0m", 
            ElevatorID, currentfloor, occupancy);
        }
        else
        {
            while (currentfloor != dest_floor && dest_floor != -1)
            {
                no_requset_flag = false;
                if (Building::instance->ElevatorDownBarrier[currentfloor]->Waiters() != 0 || ElevatorOutBarrier[currentfloor]->Waiters() !=
                                                                                                 0)
                {
                    OpenDoors();
                    CloseDoors();
                }
                VisitFloor(currentfloor - 1);
                dest_floor = GetLastRequestFloor();
                printf("\033[1;32mElevator %d at %d floor with %d riders\n\033[0m", ElevatorID, currentfloor, occupancy);
            }
            if (dest_floor == -1)
            {
                if (no_requset_flag)
                {
                    state = STOP;
                    continue;
                }
                state = UP;
                no_requset_flag = true;
                continue;
            }
            if (Building::instance->ElevatorDownBarrier[currentfloor]->Waiters() == 0)
            {
                state = UP;
                OpenDoors();
                CloseDoors();
                dest_floor = GetLastRequestFloor();
                if (dest_floor == -1)
                    state = STOP;
                else
                    VisitFloor(currentfloor + 1);
            }
            else
            {
                OpenDoors();
                CloseDoors();
                VisitFloor(currentfloor - 1);
            }
            printf("\033[1;32mElevator %d at %d floor with %d riders\n\033[0m", ElevatorID, currentfloor, occupancy);
        }
        currentThread->Yield();
    }
}
void rider(int id, int srcFloor, int dstFloor)
{
    Elevator *e;
    // printf("src:%d dst:%d\n",srcFloor,dstFloor);
    do
    {
        if (srcFloor < dstFloor)
        {
            printf("\033[1;35mRider %d CallUp from %dth floor\n\033[0m", id, srcFloor);
            Building::instance->CallUp(srcFloor);// 呼叫电梯
            printf("\033[1;35mRider %d wait to go up from %dth floor\n\n\033[0m", id, srcFloor);
            e = Building::instance->AwaitUp(srcFloor);// 电梯来了
        }
        else
        {
            printf("\033[1;35mRider %d CallDown from %dth floor\n\033[0m", id, srcFloor);
            Building::instance->CallDown(srcFloor);
            printf("\033[1;35mRider %d want to go down to %dth flooe\033[0m\n\n", id, srcFloor);
            e = Building::instance->AwaitDown(srcFloor);
        }
        printf("\033[1;35mRider %d Enter() elevator %d\n\033[0m", id, e->getId());
    } while (!e->Enter()); // elevator might be full!
    printf("\033[1;35mRider %d want to go to %dth floor\n\033[0m", id, dstFloor);
    e->RequestFloor(dstFloor);
    printf("\033[1;35mRider %d Exit\n\033[0m", id);
    e->Exit();
    printf("\033[1;35mRider %d finished\n\033[0m", id);
}