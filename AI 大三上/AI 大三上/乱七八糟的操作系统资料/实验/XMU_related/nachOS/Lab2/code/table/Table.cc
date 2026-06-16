// table.cc
// implementd by ForenewHan

#include "Table.h"

Table::Table(int size)
{
	tb = new void*[size];	// new void* array
	idx = new int[size]();	// new int array
	sz = size;		// store size
	lock = new Lock("table lock");
}

int Table::Alloc(void *object)
{
	for(int i=0;i<sz;i++){	// do a simple loop, find first empty slot
		lock->Acquire();
		if(idx[i] == 0){
			tb[i] = object;
			idx[i] = 1;
			lock->Release();
			return i;
		}
		lock->Release();
	}
	return -1;		// if there is no empty slot, return -1
}

void* Table::Get(int index)		// using index to get void*
{
	ASSERT(idx[index] == 1);	// assert if this index is empty
	return tb[index];
}

void Table::Release(int index)	// release index
{
	idx[index] = 0;
}

Table::~Table()
{
	delete []tb;
	delete []idx;
	delete lock;
}
