#include <stdio.h>
class DLLElement
{
public:
	DLLElement(void *itemPtr, int sortKey=0):next(NULL),prev(NULL),key(sortKey),item(itemPtr)//initialize a list element
	{
	}
	DLLElement*next;//next element on list
					//NULL if this is the last
	DLLElement*prev;//next element on list
					//NULL if this is the first

	int key;		//priority, for a sorted list
	void *item;		//pointer to item on the list
	
};
class dllist
{
public:
	dllist();				  //initialize the list
	~dllist();				  //de-allocate the list
	void Prepend(void*item);  //add to head of the list (set key=min-1)
	void Append(void * item, int sortKey = 0);	  //add to tail of list  (set key=max+1)
	void *Remove(int *keyPtr);//remove from head of list
							  //set *keyPtr to key of the removed item
							  //return item(or NULL if list is empty)
	bool IsEmpty();//return ture if list has elements
				   //routines to put/get items on/off list in order (sorted by key)
	void SortedInsert(void*item, int sortKey);
	void* SortedRemove(int sortKey);//remove first item with key==sortKey
									//return NULL if no such item exists
	void ListShow(int which);
	void NodeShow(DLLElement* tag);

private:
	DLLElement * first;//head of the list,NULL if empty
	DLLElement *last;//last element of the list,NULL if empty
};

