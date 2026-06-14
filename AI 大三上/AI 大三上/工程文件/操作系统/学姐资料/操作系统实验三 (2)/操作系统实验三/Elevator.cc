#include "Elevator.h"

extern Building *building;
Lock *lock1 = new Lock("elevator lock");//这里定义为lock1是为了不和dllist.cc里的lock名字重复
int riderTotal = 0;//乘客总数
extern int capacity;//电梯容量

//----------------------------------------------------------------------
// Elevator::Elevator
//----------------------------------------------------------------------

Elevator::Elevator(char *debugName, int numFloors, int myID)
{
    name = debugName;
    NumFloors = numFloors;
    ID = myID;
    currentfloor = 1;//初始楼层为1楼
    status = SLEEP;
    occupancy = 0;
    Capacity = capacity;
    overload = false;
    exitBarrier = new EventBarrier*[NumFloors+1];
    exitRequest = new bool[NumFloors+1];
    for (int i = 1; i <= NumFloors; i++)//在同一楼层离开电梯的乘客的事件栅栏
	{
	    exitBarrier[i] = new EventBarrier();
	    exitRequest[i] = false;//初始楼层有离开请求
    }
}

Elevator::~Elevator()
{
    delete[] exitRequest;
    for (int i = 1; i <= NumFloors; i++)
        delete exitBarrier[i];
    delete[] exitBarrier;
}

void Elevator::OpenDoors()
{
    printf("Elevator %d openDoors\n", ID);
    if (exitRequest[currentfloor])//当前楼层有乘客要离开电梯
	{
	    exitBarrier[currentfloor]->Signal();//用事件栅栏发出信号表示乘客可以一起离开
	    exitRequest[currentfloor] = false;
    }
    if (building->callUp[currentfloor] && (status == UP || status == SLEEP))
        //如果当前楼层有人叫向上电梯且电梯是向上或未使用的状态
	{
	    building->elevatorUp[currentfloor] = ID;	//记录要向上当前电梯的序号
                                                    //必须在signal前因为要先存入elevatorUp再在Awaitup中取出
	    building->upBarrier[currentfloor]->Signal();//给当前楼层等待乘电梯上楼的乘客发信号，等待乘客上完电梯继续执行
	    building->HandleFloor(currentfloor, UP);//处理乘客队列中所有处于当前楼层且上楼的请求
	    if (overload)
		{
	        overload = false;	// 如果电梯超载，剩下的等电梯的人就要继续等
	        building->callUp[currentfloor] = true;//当前楼层依旧有上楼的请求
	    }
	    else
	        building->callUp[currentfloor] = false;//当前楼层无上楼的请求
    }
    else if (building->callDown[currentfloor] && (status == DOWN || status == SLEEP))
         //如果当前楼层有人叫向下电梯且电梯是向下或未使用的状态
	{
	    building->elevatorDown[currentfloor] = ID;	//记录要向下的电梯序号 must set before Signal
	    building->downBarrier[currentfloor]->Signal();//给当前楼层等待乘电梯下楼的乘客发信号，等待乘客上完电梯继续执行
	    building->HandleFloor(currentfloor, DOWN);//处理乘客队列中所有处于当前楼层且下楼的请求
	    if (overload)
		{
	        overload = false;		// if over load, the rest riders need more elevators
	        building->callDown[currentfloor] = true;//当前楼层依旧有下楼的请求
	    }
	    else
	        building->callDown[currentfloor] = false;//当前楼层无下楼的请求
    }
}

void Elevator::CloseDoors()
{
    printf("Elevator %d closeDoors\n", ID);
    if (currentfloor == 1 && status == DOWN)//如果到达底层
	    status = UP;//电梯状态变为向上可用
    else if (currentfloor == NumFloors && status == UP)//如果到达顶层
	    status = DOWN;//电梯状态变为向下可用
}

void Elevator::VisitFloor(int floor)
{
    printf("Elevator %d visit Floor %d\n", ID, floor);
    currentfloor = floor;
    OpenDoors();
    currentThread->Yield();	//切换线程使乘客上下电梯
    CloseDoors();
}

bool Elevator::Enter()
{
    if (occupancy < Capacity)//当前实载量小于电梯容量
	{
	    occupancy++;
	    overload = false;	//电梯未超载
	    if (status == UP)//如果电梯向上
            building->upBarrier[currentfloor]->Complete();//要上楼的乘客上电梯
	    else//如果电梯向下
            building->downBarrier[currentfloor]->Complete();//要下楼的乘客上电梯
	    return true;
    }
    else
	{
	    occupancy = Capacity;
	    overload = true;
	    if (status == UP)//如果电梯向上
            building->upBarrier[currentfloor]->Complete();//要上楼的乘客上电梯，这里只是Complete没有真的上电梯，
                                                    //Signal让所有等待的线程都收到信号，
                                                    //只有所有等待的线程都Complete才能继续执行
	    else//如果电梯向下
            building->downBarrier[currentfloor]->Complete();//要下楼的乘客上电梯，这里只是Complete没有真的上电梯，
                                                    //Signal让所有等待的线程都收到信号，
                                                //只有所有等待的线程都Complete才能继续执行
	    return false;
    }
}

void Elevator::Exit()
{
    occupancy--;
    exitBarrier[currentfloor]->Complete();//离开者的事件栅栏完成一个
}

void Elevator::RequestFloor(int floor)
{
    exitRequest[floor] = true;//有请求去floor层
    exitBarrier[floor]->Wait();//进入电梯的乘客等待到达楼层可以离开的信号
}

void Elevator::StartElevator()
{
    while (true)
    {
    	lock1->Acquire();
	    if (occupancy == 0)			// 电梯无人使用
	    {
	        int next = building->HandleCall();//电梯处理第一个请求
	        if (next == 0)//如果没有请求
	        {

		        status = SLEEP;		// set elevator SLEEP
		        printf("Elevator %d is sleeping\n", ID);
		        building->suspend_cond->Wait(lock1);//空的电梯被挂起在条件变量suspend_cond上
		        printf("Elevator %d wakeup from sleeping\n", ID);
		        lock1->Release();
	        }
			else
			{
			    int fromFloor = (next > 0) ? next : (-next);//正值表示向上，负值表示向下
			    if (next > 0) //向上的请求
                    status = UP;
				else //向下的请求
                    status = DOWN;
				VisitFloor(fromFloor);//电梯去指定楼层
				lock1->Release();
				currentThread->Yield();//切电梯线程执行别的电梯
            }
	    }
		else//电梯有人使用
		{
		    if (status == UP)//电梯此时要向上
			{
		        for (int i = currentfloor+1; i <= NumFloors; i++)
				    if (exitRequest[i] || building->callUp[i])//在当前楼层的上层里是否有乘客要下电梯或有人叫电梯
		            {
			            VisitFloor(i);//去那个楼层
			            break;
		            }
	        }
	        else if (status == DOWN)//电梯此时要向下
	        {
		        for (int i = currentfloor-1; i >= 1; i--)
		            if (exitRequest[i] || building->callDown[i])
		            {
			            VisitFloor(i);
			            break;
		            }
	        }
	        else
		        printf("error status!\n");

	        lock1->Release();
	        currentThread->Yield();//切电梯线程
	    }
    }
}


void createElevator(int which)//创建要求数目的电梯
{
    building->elevator[which] = new Elevator(building->elevatorname[which], building->NumFloors, which);
    building->elevator[which]->StartElevator();
}


Building::Building (char *debugName, int numFloors, int numElevators)
{
    name = debugName;
    NumFloors = numFloors;
    NumElevators = numElevators;
    callUp = new bool[NumFloors+1];
    callDown = new bool[NumFloors+1];
    elevatorUp = new int[NumFloors+1];
    elevatorDown = new int[NumFloors+1];
    upBarrier = new EventBarrier*[NumFloors+1];
    downBarrier = new EventBarrier*[NumFloors+1];
    for (int i = 1; i <= NumFloors; i++)
	{
	    callUp[i] = false;//起始无楼层有请求
	    callDown[i] = false;
	    upBarrier[i] = new EventBarrier();
	    downBarrier[i] = new EventBarrier();
    }
    callQueue = new int[200];
    end = 0;
    suspend_cond = new Condition("elevator suspend cond");
    elevator = new Elevator*[NumElevators+1];
    elevatorname = new char*[NumElevators+1];
    for (int j = 1; j <= NumElevators; j++)//每个电梯都是一个线程
	{
    	elevatorname[j] = new char[5];
    	sprintf(elevatorname[j], "e%d", j);
	    Thread *t = new Thread(elevatorname[j]);
	    t->Fork(createElevator, j);
    }
}

Building::~Building()
{
    delete callUp;
    delete callDown;
    delete elevatorUp;
    delete elevatorDown;
    for (int i = 1; i <= NumFloors; i++)
	{
	   delete upBarrier[i];
	   delete downBarrier[i];
    }
    delete[] upBarrier;
    delete[] downBarrier;
    delete[] callQueue;
    delete suspend_cond;
}

void Building::CallUp(int fromFloor)
{
    callUp[fromFloor] = true;//当前楼层有向上的请求
    RecordCall(fromFloor, UP);//记录当前楼层和向上的请求
    lock1->Acquire();
    suspend_cond->Broadcast(lock1);	//当有请求的时候，在第一次CallUp里面Broadcast唤醒所有SLEEP的电梯
    lock1->Release();
}

void Building::CallDown(int fromFloor)
{
    callDown[fromFloor] = true;//当前楼层有向下的请求
    RecordCall(fromFloor, DOWN);//记录当前楼层和向下的请求
    lock1->Acquire();
    suspend_cond->Broadcast(lock1);	//当有请求的时候，在第一次CallDownCallDown里面Broadcast唤醒所有SLEEP的电梯
    lock1->Release();
}

Elevator *Building::AwaitUp(int fromFloor)
{
    upBarrier[fromFloor]->Wait();//在fromFloor的乘客等待OpenDoors中可以乘电梯的信号
    int id = elevatorUp[fromFloor];
    printf("Rider %s choose Elevator %d\n", currentThread->getName(), id);
    return elevator[id];
}

Elevator *Building::AwaitDown(int fromFloor)
{
    downBarrier[fromFloor]->Wait();
    int id = elevatorDown[fromFloor];
    printf("Rider %s choose Elevator %d\n", currentThread->getName(), id);
    return elevator[id];
}

void Building::RecordCall(int fromFloor, int direction)
{
    callQueue[end] = fromFloor * direction;//rider所在的楼层和方向的乘积,正向上，负向下
    end++;
}

int Building::HandleCall()
{
    int value;
    if (end > 0)//队列中有等待的乘客
	{
	    value = callQueue[0];//取出第一个请求
		end--;
	    for(int i = 0;i < end;i++)
	        callQueue[i] = callQueue[i+1];
    }
    else
        value = 0;
    return value;
}

void Building::HandleFloor(int fromFloor, int direction)
{
    int i,j;
    for (i = 0; i < end; i++)
	{
	    if (callQueue[i] == fromFloor * direction )//在当前楼层且和电梯同方向的乘客离开请求队列
	        callQueue[i] = 0;
    }
	for (i = 0,j = 0; i < end; i++)
	{
	    if (callQueue[i] == 0);
		else
		{
		    callQueue[j] = callQueue[i];
			j++;
		}
    }
    end = j;
}

void rider(int id, int srcFloor, int dstFloor)//电梯序号，乘客的起始楼层、目的楼层
{
    Elevator *e;
	if (srcFloor == dstFloor)
	    return;
    do
	{
	    if (srcFloor < dstFloor)
		{
	        printf("Rider %d CallUp       (%d)\n", id, srcFloor);
	        building->CallUp(srcFloor);//叫电梯
	        printf("Rider %d AwaitUp      (%d)\n", id, srcFloor);
	        e = building->AwaitUp(srcFloor);//等电梯到来并向上
	    }
	    else
		{
	        printf("Rider %d CallDown     (%d)\n", id, srcFloor);
	        building->CallDown(srcFloor);
	        printf("Rider %d AwaitDown    (%d)\n", id, srcFloor);
	        e = building->AwaitDown(srcFloor);//等电梯到来并向下
	    }
		printf("Rider %d Enter\n", id);
    } while (!e->Enter());	// 电梯要是满了剩下的人就一直叫电梯
    printf("Rider %d RequestFloor (%d)\n", id, dstFloor);
    e->RequestFloor(dstFloor);
    printf("Rider %d Exit         (%d)\n", id,dstFloor);
    e->Exit();
	printf("Rider %d finished\n", id);
}
