#include "time.h"
#include "copyright.h"
#include "dllist.h"
#include "system.h"

void InsertElement1(int n, DLList *dll, int which)
{
	RandomInit(which * 250);
	for (int i = 0; i < n; i++)
	{
		int index = Random() % 100;
		printf("*** thread %d insert %d: ", which, index);
		//	currentThread->Yield();
		dll->SortedInsert1(NULL, index);
	}
}

void DeleteElement1(int n, DLList *dll, int which)
{
	int key;
	for (int i = 0; i < n; i++)
	{
		printf("*** thread %d remove: ", which);
		dll->Remove(&key);
	}
	
}

void InsertElement2(int n, DLList *dll, int which)
{
	RandomInit(which * 250);
	for (int i = 0; i< n; i++)
	{
		int index = Random() % 100;
		printf("*** thread %d insert %d: ", which, index);
		dll->SortedInsert2(NULL, index, which);
	}
}

void DeleteElement3(int n, DLList *dll, int which)
{
	int key;
	for (int i = 0; i < n; i++)
	{
		printf("*** thread %d remove:  ", which);
		dll->Remove3(&key, which);
	}
}
