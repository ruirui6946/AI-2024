#include "dllist.h"
#include "system.h"
#include "utility.h"
extern int testnum;
extern int a;
int error_type = 0;

DLLElement::DLLElement( void *itemPtr, int sortKey )
{
    next = prev = NULL;
    key = sortKey;
    item = itemPtr;
}


DLList::DLList()
{
    first = last = NULL;
}

DLList::~DLList()
{
    while (Remove(NULL) != NULL);
}

void DLList::Prepend(void *item)
{
    if (IsEmpty())
    {
        DLLElement *elem = new DLLElement(item, 1);
        first = last = elem;
    }
    else
    {
        DLLElement *elem = new DLLElement(item, first->key - 1);
        elem->next = first;
		if (error_type == 6)
		{
			DEBUG('p', "error2: switching thread\n");
			printf("tail insert  error:表头插入节点时还未修改完指针就跳转导致节点添加失效\n");
			printf("jump to another\n");
			currentThread->Yield();
		}
		first->prev = elem;
        first = elem;
    }
}

void DLList::Append(void *item)
{
    if (IsEmpty())
    {
        DLLElement *elem = new DLLElement(item, 1);
        first = last = elem;
    }
    else
    {
        DLLElement *elem = new DLLElement(item, last->key + 1);
        last->next = elem;
		if (error_type == 4)
		{
			DEBUG('p', "error2: switching thread\n");
			printf("tail insert  error:末尾插入节点时还未修改last指针就跳转导致线程添加失效\n");
			printf("jump to another\n");
			currentThread->Yield();
		}
        elem->prev = last;
        last = elem;
    }
}

void *DLList::Remove(int *keyPtr)
{
    if (IsEmpty())
        return NULL;
    *keyPtr = first->key;
    DLLElement *head = first;
    if (error_type == 2)
    {
    	DEBUG('p', "error2: switching thread\n");
		printf("remove error:节点未正常删除，first指针指向错误\n");
		printf("jump to another\n");
	    currentThread->Yield();
    }
    first = head->next;
    if (first == NULL)
        last = NULL;    //  list is empty
    else
        first->prev = NULL;
    void *item = head->item;
	
    delete head;
    return item;
}

bool DLList::IsEmpty()
{
    return((first == NULL) && (last == NULL));
}

void DLList::SortedInsert(void *item, int sortKey)
{
    DLLElement *elem = new DLLElement(item, sortKey);
    if (IsEmpty())
    {
        first = last = elem;
    }
    else
    {
        DLLElement *p;
        for (p = first; p != NULL && p->key < sortKey; p = p->next);
        
		if (error_type == 1)
		{
			DEBUG('p', "error1: switching thread\n");
			printf("insert error:节点插入位置丢失\n");
			printf("jump to another\n");
			currentThread->Yield();
		}
        if (p == NULL)  // last one
        {
            elem->prev = last;
            last->next = elem;
			
			  // last one
            last = elem;    // append to tail
        }
        else
        {
            elem->next = p;
            elem->prev = p->prev;
			if (p->prev!= NULL)
                p->prev->next = elem;
            else
                first = elem;   // Prepend to head
			if(error_type == 3)
			{
				DEBUG('p', "error1: switching thread\n");
				printf("insert error:未完成指针指示就跳转导致原链表混乱\n");
				printf("jump to another\n");
				currentThread->Yield();
			}
            
            p->prev = elem;
        }
    }
}

void *DLList::SortedRemove(int sortKey)
{
    DLLElement *p;
	void* item=NULL;
    for (p = first; p != NULL && p->key <= sortKey; p = p->next)
         
		 {if (p != first)
            {
                p->prev->next = p->next;
				if(error_type == 5)
				{
				DEBUG('p', "error1: switching thread\n");
				printf("insert error:修改指针的操作进行到一半就跳转导致两个线程p指向不一，链表断裂\n");
				printf("jump to another");
				currentThread->Yield();
				}
                if (p != last)
                    p->next->prev = p->prev;
                else
                    last = p->prev;
                item = p->item;
                delete p;
			}
			else 
			{
				if(error_type == 5)
				{
				DEBUG('p', "error1: switching thread\n");
				printf("insert error:修改指针的操作进行到一半就跳转导致两个线程p指向不一，链表断裂\n");
				printf("jump to another");
				currentThread->Yield();
				}
				first=p->next;
				item =p->item;
			delete p;
			}
		 }
		
		return item;
}
     