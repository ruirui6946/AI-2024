/*

Table implements a simple fixed-size table, a common kernel data
structure.  The table consists of "size" entries, each of which
holds a pointer to an object.  Each object in the table can be
named by an index in the range [0..size-1], corresponding to its
position in the table.  Table::Alloc allocates a free entry,
stores an object pointer in it, and returns its index.  The
object pointer can be retrieved by passing its index to Table::Get.
An entry is released by passing its index to Table::Release.

Table knows nothing about the objects it indexes.  In particular,
it is the responsibility of the caller to delete each object when
it is no longer needed (some time after the table entry is released).

It is also the caller's responsibility to correctly handle the types
of the objects stored in the Table.  The object pointer in each Table
entry is untyped (void*).  It is necessary to cast an object pointer
to a (void *) before storing it in the table, and to cast it back to
its correct type (e.g., (Process *)) after retrieving it with Get.
A more sophisticated solution would use parameterized types.aux

In later assignments, the Table class may be used to implement internal
operating system tables of processes, threads, memory page frames, open
files, etc.

*/
# ifndef TABLE_H
#include "synch.h"
class Table {
   public:
     // create a table to hold at most 'size' entries.
     Table(int size);
      ~Table();
     // allocate a table slot for 'object'.
     // return the table index for the slot or -1 on error.
     int Alloc(void *object);
   
     // return the object from table index 'index' or NULL on error.
     // (assert index is in range).  Leave the table entry allocated
     // and the pointer in place.
     void *Get(int index);
   
     // free a table slot
     void Release(int index);
   private:
		int TableSize;// 表大小，防止越界
		Lock *lock;// 锁，保证临界区安全
		void **table;//指针数组，存储表项数据	   
		int *used;// 存储是否使用该位置的表
};

Table::Table(int size)
{
	TableSize=size;
	lock = new Lock("Table lock");
	table = new void*[size];
	used = new int[size];
	for (int i = 0; i < size; ++i)
	{
		table[i] = NULL;
		used[i] = 0;
	}
}

Table::~Table()
{
	delete table;
	delete used;
	delete lock;
}

int Table::Alloc(void* object)
{
/*
分配一个地址给object，
若分配成功，返回分配好的下标，
否则返回-1
*/
	lock->Acquire();
	for(int i=0;i<TableSize;i++){
		if(used[i]==0){
			table[i]=object;
			printf("insert object:%p in the %dth position in the array\n",object,i+1);
      used[i]=1;
			lock->Release();
			return i;
		}
	}
	printf("insertion failed because of full table\n");
	lock->Release();
	return -1;
}

void* Table::Get(int index)
{
	lock->Acquire();
	void* item;
	ASSERT(index<TableSize);
	if(index>=TableSize||index<0)//数组越界
	{
		printf("index out of range! ");
		item=NULL;
		lock->Release();
		return item;
	}
	item = table[index];
	lock->Release();
	return item;
}

void Table::Release(int index)
{
	lock->Acquire();
	if(!(index>=0&&index<TableSize)){
		printf("index out of range!\n");
		lock->Release();
		return;
	}//数组越界
	if(used[index]==0){
		printf("the %dth element doesn't exist!\n",index);
		lock->Release();
		return;
	}//不存在该元素
	printf("the %dth element released!\n", index);
	table[index] = NULL;
	lock->Release();
	used[index]=0;//释放成功
}


# endif

