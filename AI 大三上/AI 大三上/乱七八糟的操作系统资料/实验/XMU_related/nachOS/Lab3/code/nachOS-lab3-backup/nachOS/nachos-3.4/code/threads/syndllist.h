// syndllist.h 
//	Data structures for synchronized access to a dllist.
//
//	Implemented by ForenewHan
//  2018.5.9


#ifndef SYNDLLIST_H
#define SYNDLLIST_H

#include "dllist.h"
#include "synch.h"
#include "system.h"

class Syndllist {
  public:
    Syndllist();		// initialize a synchronized dllist
    ~Syndllist();		// de-allocate a synchronized dllist

    void Append(void *item, int key);	// append item to the end of the dllist,
				// and wake up any thread waiting in remove
    void *Remove(int *keyPtr);		// remove the first item from the front of
				// the dllist, waiting if the dllist is empty
				// apply functioH to every item in the dllist
    void Mapcar(VoidFunctionPtr func);

  private:
    DLList *dllist;			// the unsynchronized dllist
    Lock *lock;			// enforce mutual exclusive access to the dllist
    Condition *listEmpty;	// wait in Remove if the dllist is empty
};


#endif

