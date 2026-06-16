// BoundBuffer.cc
// BoundBuffer, used as an example to solve Read-Write problem

#include "BoundBuffer.h"

BoundBuffer::BoundBuffer(int maxsize)
{
	tb = new Table(maxsize);
	sz = maxsize;
	ar = 0;
	wr = 0;
	aw = 0;
	ww = 0;
	lock = new Lock("protect lock");
	OktoWrite = new Condition("writing condition");
	OktoRead = new Condition("reading condition");
}

BoundBuffer::~BoundBuffer()
{
	delete tb;
	delete lock;
	delete OktoRead;
	delete OktoWrite;
}

void BoundBuffer::Read(void *data, int size){
	lock->Acquire();
	while((aw + ww)>0){
		wr++;
		OktoRead->Wait(lock);
		wr--;
	}
	ar++;
	lock->Release();
	
	// access data part
	ASSERT(size <= sz);
	for(int i=0;i<size;i++){
		data[i] = *(tb->Get(i));
		tb->Release(i);
	}
	
	ar--;
	if(ar == 0 && ww > 0){
		OktoWrite->Signal(lock);
		lock->Release();
	}
}

void BoundBuffer::Write(void *data, int size){
	lock->Acquire();
	while((ar + aw)>0){
		ww++;
		OktoWrite->Wait(lock);
		ww--;
	}
	aw++;
	lock->Release();
	
	// write data part
	ASSERT(size<=sz);
	for(int i=0;i<size;i++){
		tb->Alloc(data+i);
	}
	
	aw--;
	if(ww>0){
		OktoWrite->Signal(lock);
	}
	else if(wr > 0){
		OktoRead->Broadcast(lock);
		lock->Release();
	}
}

