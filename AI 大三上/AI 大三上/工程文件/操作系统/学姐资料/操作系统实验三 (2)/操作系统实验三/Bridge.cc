#include "Bridge.h"

Bridge::Bridge()
{
    lock = new Lock("Bridge lock");
    status = FREE;
    BridgeBusyL = new Condition("Bridge busyl cond");
    BridgeBusyR = new Condition("Bridge busyr cond");
    count = 0;
	valuel = 0;
	valuer = 0;
	flag = true;
}

Bridge::~Bridge()
{
    delete lock;
    delete BridgeBusyL;
    delete BridgeBusyR;
}

void Bridge::ArriveBridge(char direc)
{
    lock->Acquire();
	printf("*Car %s arrive the bridge at *%c* side\n",currentThread->getName(),direc);
    if (direc == 'l')//左边有车到达
	{
	    if(status == FREE)//如果桥上无车
		    status = BUSYL;
		else if (status == BUSYL)  //有同方向的车在过桥
		{
		    if (count < 3 && flag == true) ; //小于三辆车在过桥并且前面没有相同方向的车在等待过桥
			else //已经有三辆车在过桥或者前面有相同方向的车在等待过桥
			{
			    valuel++;//等待在左边的车数量加1
				printf("*Car %s waiting\n",currentThread->getName());
			    BridgeBusyL->Wait(lock);//等待左边可以过桥的信号
			}
		}
		else   //有反方向的车在过桥
		{
		    valuel++;
			printf("*Car %s waiting\n",currentThread->getName());
		    BridgeBusyL->Wait(lock);//等待左边可以过桥的信号，Wait函数里会将线程放入阻塞队列
		}
	}
	else//右边有车到达
	{
	    if(status == FREE)//如果桥上无车
		    status = BUSYR;
		else if (status == BUSYR)
		{
		    if (count < 3 && flag == true);
			else
			{
			    valuer++;
				printf("*Car %s waiting\n",currentThread->getName());
			    BridgeBusyR->Wait(lock);//等待右边可以过桥的信号
			}
		}
		else
		{
		    valuer++;
			printf("*Car %s waiting\n",currentThread->getName());
		    BridgeBusyR->Wait(lock);//等待右边可以过桥的信号
		}
	}
	count++;//如果到达的车可以上桥，则桥上车数量增加，如果不能则要等待信号
	lock->Release();
}

void Bridge::CrossBridge(char direc)
{
    printf("*Car %s cross the bridge from *%c* side\n",currentThread->getName(),direc);
	currentThread->Yield();
}

void Bridge::ExitBridge(char direc)
{
    lock->Acquire();
	count--;
	flag = false;//此时等待的车不能过桥
    if (direc == 'l')//左边的车在过桥
	{
	    if (valuer > 0)//右边有车等待
		{
		    if (count == 0)//如果下桥后桥上无车
			{
			    status = FREE;
				flag = true;
	            for (int i = 0;i < 3 && valuer > 0;i++)//右边车可以过桥了
				{
					valuer--;
		            BridgeBusyR->Signal(lock);//发出信号，右边的车可以准备过桥了
				}
			}
		}
		else
		{
		    if (count == 0)//右边无车等待
			{
			    status = FREE;
				flag = true;
	            for (int i = 0;i < 3 && valuel > 0;i++)//左边车可以继续过桥
				{
				    valuel--;
		            BridgeBusyL->Signal(lock);//发出信号，左边的车可以准备过桥了
				}
			}
		}
	}
	else
	{
	    if (valuel > 0)
		{
	        if (count == 0)
			{
			    status = FREE;
	            for (int i = 0;i < 3 && valuel > 0;i++)
				{
				    valuel--;
		            BridgeBusyL->Signal(lock);
				}
			}
		}
		else
		{
		    if (count == 0)
			{
			    status = FREE;
	            for (int i = 0;i < 3 && valuer > 0;i++)
				{
					valuer--;
					BridgeBusyR->Signal(lock);
				}
			}
		}
	}
	printf("*Car %s exit the bridge\n",currentThread->getName());
	lock->Release();
}

