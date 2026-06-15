#include "stdlib.h"
#include "Table.h"
#include "synch.h"
#include "copyright.h"
#include "system.h"

Table::Table(int size)//构造表
{
	tlock = new Lock("tlock");
	tlock->Acquire();
	tsize = size;
	tptr = (void**) malloc(sizeof(void*) * tsize);
	for (int  i = 0; i < size; i++)
		tptr[i]=NULL;
	tlock->Release();
}

Table::~Table()//
{
	tlock->Acquire();
	void* tmp;
	while (tsize != 0)
	{
		tmp = tptr[tsize-1];
		tsize--;
		free(tmp);
	}
	tsize = 0;
	tlock->Release();
	delete tlock;
}

bool
Table::IsFull()
{
	for (int i = 0; i < tsize; i++)
	{
		if (tptr[i] == NULL)
			return false;
	}
	return true;
}

// allocate a table slot for 'object'
// return the table index for the slot or -1 on error.
int Table::Alloc(void* object)
{
	tlock->Acquire();
	int i;
	for (i = 0; i < tsize; i++)//找一个空位
	{
		if (tptr[i]==NULL)
			break;
	}
	if (i == tsize)
	{
		printf("Error: table is ful\n");
		tlock->Release();
		return -1;
	}
	else
		tptr[i] = object;
	tlock->Release();
	return i;
}

// return the object from table index 'index' or NULL on error.
// (assert index is in range).  Leave the table entry allocated
// and the pointer in place.
void* Table::Get(int index)
{
	tlock->Acquire();
	if (index >= tsize||index<0)
	{
		printf("Error: index invaild\n");
		tlock->Release();
		return NULL;
	}
	tlock->Release();
	return tptr[index];
}

// free a table slot
void Table::Release(int index)
{
	tlock->Acquire();
	if (index >= tsize)
	{
		printf("Error: index invaild\n");
	}
	tptr[index]=NULL;
	tlock->Release();
}

void Table::PrintTable()//打印
{
	printf("\n1: slot has an object, 0: slot is empty\n");
	for (int i = 0; i < tsize; i++)
	{
		if (tptr[i] == NULL)
			printf("0 ");
		else
			printf("1 ");
	}
	printf("\n");
}
