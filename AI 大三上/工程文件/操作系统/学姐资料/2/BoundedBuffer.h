#include "synch.h"
#include "thread.h"
#include "utility.h"
#include "copyright.h"
#include "system.h"

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

     void BoundedBufferTest(int which, int loop);

   private:
     //s: mutex, ensure critical part (vaild in the function)
     //n: synch, ensure writer start first (vaild between funtion)
     //e: buffersize check
     char* buffer;
     int buffersize;
     int cursize;
     int readptr;
     int writeptr;
     Semaphore* s;
     Semaphore* n;
     Semaphore* e;

     void Take(void* data, int index);
     void Consume(void* data, int index);
     void Produce(void* data, int index);
     void Append(void* data, int index);
     //void BoundedBufferTest(int which, int loop);
     
};

