#include <iostream>
#include "dllist.h"
#include "thread.h"
#define KEY 50

extern Thread *currentThread; 
extern void YIELD(int place);
extern void DEBUG(char flag, char *format, ...);

DLLElement::DLLElement(void * itemPtr, int sortKey)
{
    YIELD(1);
    next = NULL;
    YIELD(2);
    prev = NULL;
    YIELD(3);
    key = sortKey;
    YIELD(4);
    item = itemPtr;
    YIELD(5);
}

DLList::DLList()
{
    first = NULL;
    last = NULL;
}

DLList::~DLList()
{
    int key;
    while(!IsEmpty())
    {
        Remove(&key);
    } 
}

void DLList::Prepend(void *item)
{
    if(IsEmpty())
    {
        DLLElement *elm = new DLLElement(item, KEY);
        first = elm;
        last = elm;
    }
    else
    {
        DLLElement *elm = new DLLElement(item, first->key - 1);
        elm->next = first;
        first->prev = elm;
        first = elm;
    }
}

void DLList::Append(void *item)
{
    
    if(IsEmpty())
    {
        DLLElement *elm = new DLLElement(item, KEY);
        first = elm;
        last = elm;
    }
    else
    {
        DLLElement *elm = new DLLElement(item, last->key + 1);
        elm->prev = last;
        last->next = elm;
        last = elm;
    }
}

void *DLList::Remove(int *keyPtr)
{
    YIELD(6);
    if(IsEmpty())
    {
        YIELD(7);
        *keyPtr = -1;
        YIELD(8);
        DEBUG('e', "[ERROR]%s : list is empty!\n", currentThread->getName());
        return NULL;
    }
    else
    {
        YIELD(9);
        void *item = first->item;
        YIELD(10);
        *keyPtr = first->key;
        YIELD(11);
        DLLElement *temp = first->next;
        YIELD(12);
        delete first;
        YIELD(13);
        if(temp == NULL)
        {
            YIELD(14);
            last = NULL;
        }
        else
        {
            YIELD(15);
            temp->prev = NULL;
        }
        YIELD(16);
        first = temp;
        YIELD(17);
        return item;
    }
}

bool DLList::IsEmpty()
{
    YIELD(18);
    if(first == NULL)
    {
        YIELD(19);
        if(last == NULL)
        {
            YIELD(20);
            return true;
        }
        DEBUG('e', "[ERROR]first is NULL, last is not NULL!\n");
        YIELD(21);
    }
    else if(last == NULL)
    {
        DEBUG('e', "[ERROR]first is not NULL, last is NULL!\n");
    }
    YIELD(22);
    return false;
}

void DLList::SortedInsert(void *item, int sortKey)
{
    YIELD(23);
    DLLElement *elm = new DLLElement(item, sortKey);
    YIELD(24);
    if(IsEmpty())
    {
        YIELD(25);
        first = elm;
        YIELD(26);
        last = elm;
        YIELD(27);
    }
    else
    {
        YIELD(28);
        DLLElement *ptr = first;
        YIELD(29);
        while(ptr != NULL && ptr->key < sortKey)
        {
            YIELD(30);
            ptr = ptr->next;
            YIELD(31);
        }
        YIELD(32);
        if(ptr == first) // insert in the head
        {
            YIELD(33);
            elm->next = first;
            YIELD(34);
            first->prev = elm;
            YIELD(35);
            first = elm;
            YIELD(36);
        }
        else if(ptr == NULL) // insert in the tail
        {
            YIELD(37);
            elm->prev = last;
            YIELD(38);
            last->next = elm;
            YIELD(39);
            last = elm;
            YIELD(40);
        }
        else
        {
            YIELD(41);
            elm->next = ptr;
            YIELD(42);
            ptr->prev->next = elm;
            YIELD(43);
            elm->prev = ptr->prev;
            YIELD(44);
            ptr->prev = elm;
            YIELD(45);
        }
    }
}

void *DLList::SortedRemove(int sortKey)
{
    DLLElement *ptr = first;
    while(ptr != NULL && ptr->key != sortKey)
    {
        ptr = ptr->next;
    }
    if(ptr == NULL)
    {
        return NULL;
    }
    void *item = ptr->item;
    if(ptr == first)
    {
        first = first->next;
        if(first == NULL) // list is empty
        {
            last = NULL;
        }
        else
        {
            first->prev = NULL;
        }
    }
    else if(ptr == last)
    {
        last = last->prev;
        if(last == NULL) // list is empty
        {
            first = NULL;
        }
        else
        {
            last->next = NULL;
        }
    }
    else
    {
        ptr->next->prev = ptr->prev;
        ptr->prev->next = ptr->next;
    }
    delete ptr;
    return item;
}

void DLList::show()
{
    if(IsEmpty())
    {
        printf("List is empty!\n");
    }
    else
    {
        DLLElement *tmp = first;
        printf("List from head to tail:\n");
        while(tmp != NULL)
        {
            printf("key: %d, item: %p\n", tmp->key, tmp->item);
            tmp = tmp->next;
        }
        tmp = last;
        printf("List from tail to head:\n");
        while(tmp != NULL)
        {
            printf("key: %d, item: %p\n", tmp->key, tmp->item);
            tmp = tmp->prev;
        }
    }
}
