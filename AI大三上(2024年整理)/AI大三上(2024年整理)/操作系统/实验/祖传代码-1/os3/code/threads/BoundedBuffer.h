#include "list.h"
#include "synch.h"

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
     void Print();
    
   private:
  int num_element;//已存内容
	char* buffer;//缓冲区
  char* pointer_consumer;//读指针
  char* pointer_producer;//写指针
  int maxsize;//缓冲区容量
  Semaphore* mutex;//控制互斥
  Semaphore* consumer;//读缓冲区的消费者
  Semaphore* producer;//写缓冲区的生产者
};

void BoundedBuffer::Print(){
  printf("buffer:");
  char* point=pointer_consumer;
  int temp=num_element;
  while(temp--){
    printf(" %c",*((char*)point));
    point++;
    if(point==buffer+maxsize)point=buffer;
  }
  printf("\n");
}

BoundedBuffer::BoundedBuffer(int size)
{
	maxsize = size;
  num_element=0;
	mutex = new Semaphore("Mutex lock",1);//互斥信号量初始化
	consumer = new Semaphore("consumer used to read",0);//消费者初始化
	producer = new Semaphore("producer used to write",size);//生产者初始化
  buffer = new char[maxsize];
	pointer_consumer=pointer_producer=buffer;//读写指针指向buffer首位
}

BoundedBuffer::~BoundedBuffer()
{
	delete buffer;
	delete mutex;
	delete consumer;
	delete producer;
}

void BoundedBuffer::Write(void* data, int size)
{
  for(int i=0;i<size;i++){
    producer->P();
    mutex->P();
    num_element++;
    *pointer_producer=*((char*)data+i);//赋值
    printf("%s write '%c' successfully!\n",currentThread->getName(),*pointer_producer);
    Print();
    pointer_producer++;
    if(pointer_producer==buffer+maxsize){
      pointer_producer=buffer;//写到底了，从头写
    }
    mutex->V();
    consumer->V();
  }
}

void BoundedBuffer::Read(void* data, int size)
{
  for(int i=0;i<size;i++){
    consumer->P();
    mutex->P();
    num_element--;
    *((char*)data+i)=*(char*)pointer_consumer;
    printf("%s read '%c' successfully!\n",currentThread->getName(),*((char*)data+i));
    pointer_consumer++;
    if(pointer_consumer==buffer+maxsize){//读到底了，从头读
      pointer_consumer=buffer;
    }
    mutex->V();
    producer->V();
  }
}
