# include "BoundedBuffer.h"

extern Thread *currentThread;
extern void YIELD(int place);

BoundedBuffer::BoundedBuffer(int maxsize)
{
    buffer = new char[maxsize];
    maxSize = maxsize;
    head = tail = 0;
    readLock = new Semaphore("ReadLock", 1);
    writeLock = new Semaphore("WriteLock", 1);
    use = new Semaphore("Use", 0);
    remain = new Semaphore("Remain", maxsize);
}

BoundedBuffer::~BoundedBuffer()
{
    delete buffer;
    delete readLock;
    delete writeLock;
    delete use;
    delete remain;
}

void BoundedBuffer::Read(void *data, int size)
{
    char *out = (char *) data;
    readLock->P();
    for(int i = 0; i < size; ++ i)
    {
        use->P();
        out[i] = buffer[head];
        DEBUG('e', "%s: read %c in %d.\n", currentThread->getName(), out[i], head);
        head = (head + 1) % maxSize;
        remain->V();
    }
    readLock->V();
}

void BoundedBuffer::Write(void *data, int size)
{
    char *in = (char *) data;
    writeLock->P();
    for(int i = 0; i < size; ++ i)
    {
        remain->P();
        buffer[tail] = in[i];
        DEBUG('e', "%s: write %c in %d.\n", currentThread->getName(), in[i], tail);
        tail = (tail + 1) % maxSize;
        use->V();
    }
    writeLock->V();
}
