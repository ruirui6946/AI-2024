#include "thread.h"

static int PLACE = -1;

extern Thread *currentThread; 
extern void DEBUG(char flag, char *format, ...);

void YieldInit(int place)
{
    PLACE = place;
}

void YIELD(int place)
{
    if(place == PLACE || PLACE == 0)
    {
        DEBUG('e', "%s yield in %d.\n", currentThread->getName(), place);
        currentThread->Yield();
    }
}
