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
<<<<<<< HEAD
		// tb used to store data
		Table *tb;
		// sz is current size
		int sz;
		// ar stands for active reader
		// wr stands for waiting reader
		// aw stands for active writer
		// ww stands for waiting writer
=======
		Table *tb;
		int sz;
>>>>>>> 8d24c127cd6fa24f945ea2086fb4b1499c80c11b
		int ar, wr, aw, ww;
		Lock *lock;
		Condition *OktoWrite;
		Condition *OktoRead;
   // ???
};

#endif