// syndllist.cc
//	Routines for synchronized access to a list.
//
//	Implemented by ForenewHan
//  2018.5.9

#include "copyright.h"
#include "syndllist.h"

//----------------------------------------------------------------------
// Syndllist::Syndllist
//	Allocate and initialize the data structures needed for a 
//	synchronized list, empty to start with.
//	Elements can now be added to the dllist.
//----------------------------------------------------------------------

Syndllist::Syndllist()
{
    dllist = new DLList();
    lock = new Lock("list lock"); 
    listEmpty = new Condition("list empty cond");
}

//----------------------------------------------------------------------
// Syndllist::~Syndllist
//	De-allocate the data structures created for synchronizing a dllist. 
//----------------------------------------------------------------------

Syndllist::~Syndllist()
{ 
    delete dllist; 
    delete lock;
    delete listEmpty;
}

//----------------------------------------------------------------------
// Syndllist::Append
//      Append an "item" to the end of the dllist.  Wake up anyone
//	waiting for an element to be appended.
//
//	"item" is the thing to put on the dllist, it can be a pointer to 
//		anything.
//----------------------------------------------------------------------

void
Syndllist::Append(void *item, int key)
{
    lock->Acquire();		// enforce mutual exclusive access to the dllist 
    dllist->SortedInsert(item, key);
    listEmpty->Signal(lock);	// wake up a waiter, if any
    lock->Release();
}

//----------------------------------------------------------------------
// Syndllist::Remove
//      Remove an "item" from the beginning of the dllist.  Wait if
//	the dllist is empty.
// Returns:
//	The removed item. 
//----------------------------------------------------------------------

void *
Syndllist::Remove(int *keyPtr)
{
    void *item;

    lock->Acquire();			// enforce mutual exclusion

    //printf("enter interrupt\n");
    //currentThread->Yield();
    //printf("interrupt finished\n");

    while (dllist->isEmpty()){
        printf("list is empty now, enter condition wait\n");
        listEmpty->Wait(lock);       // wait until dllist isn't empty
    }


    item = dllist->Remove(keyPtr);
    ASSERT(item != NULL);
    lock->Release();
    return item;
}

//----------------------------------------------------------------------
// Syndllist::Mapcar
//      Apply function to every item on the dllist.  Obey mutual exclusion
//	constraints.
//
//	"func" is the procedure to be applied.
//----------------------------------------------------------------------

void
Syndllist::Mapcar(VoidFunctionPtr func)
{ 
    lock->Acquire(); 
    dllist->Mapcar(func);
    lock->Release(); 
}
