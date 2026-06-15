# include "synch.h"
# include "system.h"
# include "Alarm.h"

static int direction = 0;
static int carInBridge = 0;
static int checkNum = 0;
static Lock* lock = new Lock("Bridge Lock"); 
static Condition* check = new Condition("Check Condition"); 
static Condition* passable = new Condition("Passable Condition");

bool IsPassable(int direc)
{
    if(carInBridge == 0 || (direc == direction && carInBridge < 3))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ArriveBridge(int direc)
{
    lock->Acquire();
    while(checkNum)
    {
        check->Wait(lock);
    }
    ++ checkNum;
    while(!IsPassable(direc))
    {
        passable->Wait(lock);
    }
    -- checkNum;
    check->Signal(lock);
    direction = direc;
    ++ carInBridge;
    lock->Release();
}

void CrossBridge(int direc)
{
    DEBUG('e', "%s cross the bridge, direction:%d.\n", currentThread->getName(), direc);
}

void ExitBridge(int direc)
{
    lock->Acquire();
    -- carInBridge;
    passable->Signal(lock);
    lock->Release();
}
