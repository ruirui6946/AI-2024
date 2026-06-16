// BoundedBuffer.h

#ifndef BOUNDBUFFER_H
#define BOUNDBUFFER_H

#include "synch.h"
#include "Table.h"
#include "utility.h"

class BoundedBuffer {
   public:
     // create a bounded buffer with a limit of 'maxsize' bytes
     BoundedBuffer(int maxsize);
     ~BoundedBuffer();
	 
     // read 'size' bytes from the bounded buffer, storing into 'data'.
     // ('size' may be greater than 'maxsize')
     void Read(void *data, int size);
     
     // write 'size' bytes from 'data' into the bounded buffer.
     // ('size' may be greater than 'maxsize')
     void Write(void *data, int size);
   private:
		Table *tb;
		int sz;
		int ar, wr, aw, ww;
		Lock *lock;
		Condition *OktoWrite;
		Condition *OktoRead;
   // ???
};

#endif