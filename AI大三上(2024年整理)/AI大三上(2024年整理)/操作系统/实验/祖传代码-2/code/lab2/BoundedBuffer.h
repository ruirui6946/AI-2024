# ifndef BOUNDEDBUFFER_H
# define BOUNDEDBUFFER_H
#include "synch.h"
class BoundedBuffer
{
public:
    BoundedBuffer(int maxsize); // Create a bounded buffer to hold at most maxsize bytes.
    ~BoundedBuffer();
    void Read (void* data, int size); // Read size bytes from the buffer, 
    // blocking until enough bytes are available to completely satisfy the request. 
    // Copy the bytes into memory starting at address data
    void Write (void* data, int size); // Write size bytes into the buffer, 
    // blocking until enough space is available to completely satisfy the request. 
    // Copy the bytes from memory starting at address data .
private:
    char *buffer;
    int maxSize;
    int head;
    int tail;
    Semaphore* writeLock;
    Semaphore* readLock;
    Semaphore* use;
    Semaphore* remain;
};
# endif
