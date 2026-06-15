#include "dllist.h"
#include "system.h"

struct Data
{
    int value;
};

// randomly create N nodes and insert into list
void InsertList(DLList *list, int N)
{
    for (int i = 0; i < N; i++)
    {
        void *item; // to get the address
        // create key randomly
        int key = Random() % 1000; 
        int *items = new int[1];
        items[0] = key;
        // get the addressitem = items;
        list->SortedInsert(item, key);
        printf("%s: the number: %d has entered into the queue \n",
               currentThread->getName(), key);
    }
}
// remove N nodes from list
void RemoveList(DLList *list, int N)
{
    for (int i = 0; i < N; i++)
    {
        int key;
        void *item;
        item = list->Remove(&key);
        printf("%s: the number: %d has left the queue \n", currentThread->getName(),
               *((int *)item));
    }
}
