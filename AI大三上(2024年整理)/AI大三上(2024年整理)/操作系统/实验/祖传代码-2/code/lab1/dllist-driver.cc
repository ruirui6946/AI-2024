#include <stdlib.h>
#include <time.h>
#include "dllist.h"
#include "thread.h"

#define MAX_KEY 100

extern void YIELD(int place);
extern Thread *currentThread;
extern void DEBUG(char flag, char *format, ...);

void insert(int n, DLList &dllist)
{
    YIELD(50);
    for(int i = 0; i < n; ++ i)
    {
    	void *item = new int[1];
        int key = rand() % MAX_KEY;
        YIELD(51);
        dllist.SortedInsert(item, key);
        DEBUG('e', "%s: insert key:%2d, item:%p.\n", currentThread->getName(), key, item);
        YIELD(52);
    }
    YIELD(53);
}

void remove(int n, DLList &dllist)
{
    YIELD(54);
    for(int i = 0; i < n; ++ i)
    {
        int key = -1;
        YIELD(55);
        void *item = dllist.Remove(&key);
        DEBUG('e', "%s: remove key:%2d, item:%p.\n", currentThread->getName(), key, item);
        YIELD(56);
    }
    YIELD(57);
}
