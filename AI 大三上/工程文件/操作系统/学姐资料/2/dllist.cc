#include "dllist.h"
#include "system.h"
#include "copyright.h"
#include "synch.h"

DLLElement::DLLElement(void *itemPtr, int sortKey)
{
	item = itemPtr;
	key = sortKey;
	prev = NULL;
	next = NULL;
}

DLList::DLList()
{
	first = NULL;
	last = NULL;
}

DLList::~DLList()
{
	while (!IsEmpty())
	{
		Remove(NULL);
	}
}

void
DLList::Prepend(void *item)
{
	DLLElement *element = new DLLElement(item, 0);
	if (IsEmpty())
	{
		first = element;
		last = element;
	}
	else
	{
		element->key = first->key - 1;
		element->next = first;
		element->prev = NULL;
		first->prev = element;
		first = element;
	}
}

void
DLList::Append(void *item)
{
	DLLElement *element = new DLLElement(item, 0);
	if (IsEmpty())
	{
		first = element;
		last = element;
	}
	else
	{
		element->key = last->key + 1;
		element->next = NULL;
		element->prev = last;
		last->next = element;
		last = element;
	}
}

void *
DLList::Remove(int *keyPtr)
{
	void *tmp;
	DLLElement *element = first;
	if (IsEmpty())
	{
		printf("Error: dllist empty\n");
		return NULL;
	}

	tmp = element->item;
	*keyPtr = first->key;
	if (first == last)
	{
		first = NULL;
		last = NULL;
		if (element == NULL)
			printf("Error: element empty\n");
		else
			delete element;
	}
	else
	{
		first = element->next;
		first->prev = NULL;
		delete element;
	}
	PrintDllist();
	currentThread->Yield();
	return tmp;
}

bool DLList::IsEmpty()
{
	if (first == NULL && last == NULL)
		return true;
	return false;
}

void
DLList::SortedInsert(void *item, int sortKey)
{
	
	DLLElement *element = new DLLElement(item, sortKey);
	if (IsEmpty())
	{
		//currentThread->Yield();
		first = element;
		last = element;
		element->prev = NULL;
		element->next = NULL;
		PrintDllist();
		return ;
	}

	DLLElement *ptr;
	for (ptr = first; ptr != NULL; ptr = ptr->next)
	{
		if (sortKey < ptr->key)
			break;
	}
	if (ptr == first)
	{
		element->prev = NULL;
		element->next = first;
		first->prev = element;
		first = element;
	}
	else if (ptr == NULL)
	{
		element->prev = last;
		element->next = NULL;
		last->next = element;
		last = element;
	}
	else
	{
		ptr->prev->next = element;
		element->prev = ptr->prev;
		element->next = ptr;
		ptr->prev = element;
	}
	PrintDllist();
	return;
}

void *
DLList::SortedRemove(int sortKey)
{
	DLLElement *ptr,*element;
	if (IsEmpty())
	{
		printf("Error: dllist empty\n");
		return NULL;
	}
	for (ptr = first; ptr != NULL; ptr = ptr->next)
	{
		if (ptr->key == sortKey)
			break;
	}
	if (ptr == NULL)
	{
		return NULL;
	}
	else if (ptr == first)
	{
		element = ptr;
		first = first->next;
		first->prev = NULL;
		delete element;
	}
	else if (ptr == last)
	{
		element = last;
		last = last->prev;
		last->next = NULL;
		delete element;
	}
	else
	{
		element = ptr;
		ptr->prev->next = ptr->next;
		ptr->next->prev = ptr->prev;
	}
	PrintDllist();
	return ptr->item;
}

bool DLList::IsEmpty1(Lock* lock)
{
	lock->Acquire();
	printf("get lock1\n");
	if (first == NULL && last == NULL)
	{
		lock->Release();
		return true;
	}
	lock->Release();
	printf("release lock1\n");
	return false;
}

void
DLList::SortedInsert1(void *item, int sortKey)
{
	
	DLLElement *element = new DLLElement(item, sortKey);
	//lock->Acquire();
	//printf("get lock0\n");
	if (IsEmpty())
	{
		currentThread->Yield();
		first = element;
		last = element;
		element->prev = NULL;
		element->next = NULL;
		PrintDllist();
		return ;
	}

	DLLElement *ptr;
	for (ptr = first; ptr != NULL; ptr = ptr->next)
	{
		if (sortKey < ptr->key)
			break;
	}
	if (ptr == first)
	{
		element->prev = NULL;
		element->next = first;
		first->prev = element;
		first = element;
	}
	else if (ptr == NULL)
	{
		element->prev = last;
		element->next = NULL;
		last->next = element;
		last = element;
	}
	else
	{
		ptr->prev->next = element;
		element->prev = ptr->prev;
		element->next = ptr;
		ptr->prev = element;
	}
	PrintDllist();
	//lock->Release();
	//printf("release lock0\n");
	return;
}

void DLList::SortedInsert2(void *item, int sortKey, int which)
{
	
	DLLElement *element = new DLLElement(item, sortKey);
	if (IsEmpty())
	{
		if (which == 0)
			currentThread->Yield();	
		if (which == 1)
			currentThread->Yield();
		first = element;
		last = element;
		element->prev = NULL;
		element->next = NULL;
		if (which == 0)
			currentThread->Yield();
		PrintDllist();
		return ;
	}

	DLLElement *ptr;
	for (ptr = first; ptr != NULL; ptr = ptr->next)
	{
		if (sortKey < ptr->key)
			break;
	}
	if (ptr == first)
	{
		element->prev = NULL;
		element->next = first;
		first->prev = element;
		first = element;
	}
	else if (ptr == NULL)
	{
		element->prev = last;
		element->next = NULL;
		last->next = element;
		last = element;
	}
	else
	{
		ptr->prev->next = element;
		element->prev = ptr->prev;
		element->next = ptr;
		ptr->prev = element;
	}
	PrintDllist();
	return;
}

void *
DLList::Remove3(int *keyPtr, int which)
{
	void *tmp;
	DLLElement *element = first;
	if (IsEmpty())
	{
		printf("Error: dllist empty\n");
		return NULL;
	}

	tmp = element->item;
	*keyPtr = first->key;
	if (first == last)
	{
		if (which == 0)
			currentThread->Yield();
		first = NULL;
		last = NULL;
		if (element == NULL)
			printf("Error: dll already empty\n");
		else
			delete element;
	}
	else
	{
		first = element->next;
		first->prev = NULL;
		delete element;
	}
	PrintDllist();
	currentThread->Yield();
	return tmp;
}

void  DLList::PrintDllist (void)
{
	DLLElement *ptr = first;
	if (first == NULL && last == NULL)
	{
		printf("dllist is NULL");
	}
	while(ptr != NULL)
	{
		printf("%d ", ptr->key);
		ptr = ptr->next;
	}
	printf("\n");
}
