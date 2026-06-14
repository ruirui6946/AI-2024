#include "stdlib.h"
#include "BoundedBuffer.h"
#include "synch.h"
#include "thread.h"
#include "utility.h"
#include "copyright.h"
#include "system.h"

BoundedBuffer::BoundedBuffer(int maxsize)
{
	s = new Semaphore("s", 1);
	n = new Semaphore("n", 0);
	e = new Semaphore("e", maxsize);
	buffersize = maxsize;
	cursize = 0;
	readptr = 0;
	writeptr = 0;
	buffer = (char*)malloc(maxsize * sizeof(char));
	for (int i = 0; i < maxsize; i++)
		buffer[i] = '0';
}

BoundedBuffer::~BoundedBuffer()
{
	free(buffer);
	buffersize = 0;
	cursize = 0;
	readptr = 0;
	writeptr = 0;
	delete s;
	delete n;
	delete e;
}

void
BoundedBuffer::Read(void *data, int size)
{
	if (size >= buffersize || size < 0)
	{
		printf("Error: invaild size\n");
		return ;
	}
	
	int i = 0;
	while (i < size)
	{
		n->P();			//wait Write to write
		s->P();			//lock the process

		Take(data, i);		//take char from buffer
		i++;

		s->V();			//unlock the proces
		e->V();			//读完之后唤醒一个写的进程

		Consume(data, i-1);
	}
}

void
BoundedBuffer::Write(void *data, int size)
{
	if (size >= buffersize || size < 0)
	{
		printf("Error: invaild size\n");
		return ;
	}
	int i = 0;
	while(i < size)
	{
		Produce(data, i);
		i++;

		e->P();			//wait if buffer is full
		s->P();			//lock the process
		
		Append(data, i-1);	//put char into buffer
		
		s->V();			//unlock the process
		n->V();			//写完之后唤醒一个读的线程
	}
}

void
BoundedBuffer::Take(void* data, int index)
{
	readptr = readptr%buffersize;
	((char*)data)[index]=buffer[readptr];
	readptr++;
	//cursize--;
}

void
BoundedBuffer::Consume(void* data, int index)
{
	cursize--;
	printf("consume %c cursize=%d readptr=%d writptr=%d\n", ((char*)data)[index], cursize, readptr, writeptr);
}

void
BoundedBuffer::Produce(void* data, int index)
{
	cursize++;
	printf("produce %c cursize=%d readptr=%d writptr=%d\n", ((char*)data)[index], cursize, readptr, writeptr);
}

void
BoundedBuffer::Append(void* data, int index)
{
	writeptr = writeptr%buffersize;
	buffer[writeptr] = ((char*)data)[index];
	writeptr++;
	//cursize++;
}

void
BoundedBuffer::BoundedBufferTest(int which, int loop)
{
	for (int j = 0; j < loop; j++)
	{
		RandomInit((unsigned) which * 250);
		int len = Random()%20;
		char* data = (char*)malloc(len * sizeof(char));

		if (which %2 == 0)
		{	
			for (int i = 0; i < len; i++)
				data[i] ='A' + Random()%26;
			printf("*** thread %d write ***\n", which);
			Write(data, len);			
			printf("*** thread %d end write ***\n", which);
		}
		else
		{
			printf("*** thread %d read ***\n", which);
			Read(data, len);
			printf("*** thread %d end read ***\n", which);
		}
		free(data);
	}
}
