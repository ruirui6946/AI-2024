# include <iostream>
# include "Table.h"

extern Thread *currentThread;

Table::Table(int size)
{
    TableSize = size;
    table = new void*[size];
    for(int i = 0; i < size; ++ i) table[i] = NULL;
    lock = new Semaphore("TableLock", 1);
}  
Table::~Table()
{
    delete table;
    delete lock;
}
int Table::Alloc (void* object)
{
    lock->P();
    for(int i = 0; i < TableSize; ++ i)
        if(table[i] == NULL)
        {
            table[i] = object;
            DEBUG('e', "%s: insert %p in %d.\n", currentThread->getName(), object, i);
            lock->V();
            return i;
        }
    DEBUG('e', "%s: insert failed because the tabel is full.\n", currentThread->getName());
    lock->V();
    return -1;
}
void* Table::Get (int index)
{
    lock->P();
    ASSERT(index >= 0 && index < TableSize);
    if(index >= 0 && index < TableSize && table[index] != NULL)
    {
        DEBUG('e', "%s: get %p in %d.\n", currentThread->getName(), table[index], index);
        lock->V();
        return table[index];
    }
    DEBUG('e', "%s: get NULL in %d because not allocated.\n", currentThread->getName(), index);
    lock->V();
    return NULL;
}
void Table::Release (int index)
{
    lock->P();
    ASSERT(index >= 0 && index < TableSize);
    if(index >= 0 && index < TableSize)
    {
        DEBUG('e', "%s: release %p in %d.\n", currentThread->getName(), table[index], index);
        table[index] = NULL;
    } 
    lock->V();
}
