#include <iostream>
#include "dllist.h"
#include "copyright.h"
#include "utility.h"
#include "system.h"
using namespace std;
extern int error_type;

DLLElement::DLLElement(void *itemPtr, int sortKey)
{
    // <- error 1 --
    if (error_type == 1)
    {
        printf("Error 1\n");
        currentThread->Yield();
    }
    // -- error 1 ->
    item = itemPtr;
    key = sortKey;
    prev = NULL;
    next = NULL;

}

// initialize the list
DLList::DLList()
{
    first = NULL;
    last = NULL;
}

// de-allocate the list
DLList::~DLList()
{
    // is not empty and release the memory
    while (this->IsEmpty())
    {
        int k;
        this->Remove(&k);
    }
}

// add to head of list (set key = min_key-1)
void DLList::Prepend(void *item)
{
    // is empty
    if (!this->IsEmpty())
    {
        DLLElement *temp = new DLLElement(item, START_KEY);
        temp->prev = NULL;
        temp->next = NULL;
        // create the head
        first = temp;
        last = temp;
    }
    else
    {
        DLLElement *temp = new DLLElement(item, first->key - 1);
        temp->prev = NULL;
        // prepend
        temp->next = first;
        first->prev = temp;
        first = temp;
    }
}

// add to tail of list (set key = max_key+1)
void DLList::Append(void *item)
{
    // is empty
    if (!this->IsEmpty())
    {
        DLLElement *temp = new DLLElement(item, START_KEY);
        temp->prev = NULL;
        temp->next = NULL;
        // create head
        first = temp;
        last = temp;
    }
    else
    {
        DLLElement *temp = new DLLElement(item, last->key + 1);
        // append
        temp->prev = last;
        temp->next = NULL;
        last->next = temp;
        last = temp;
    }
}

// remove from head of list
// set *keyPtr to key of the removed item
// return item (or NULL if list is empty)
void *DLList::Remove(int *keyPtr)
{
    void *RemovedItem;
    // is empty
    if (!this->IsEmpty())
    {
        keyPtr = NULL;
        return NULL;
    }
    // is not empty
    else
    {
        *keyPtr = first->key;
        RemovedItem = &(first->key);
        first = first->next;
        if (first == NULL)
            last = NULL;
        else
        {
            // <- error 2 --
            if (error_type == 2)
            {
                printf("Error 2\n");
                currentThread->Yield();
            }
            // -- error 2 ->
            first->prev = NULL;
        }
    }
    return RemovedItem;
}

// amazing
// return true if list has elements
bool DLList::IsEmpty()
{
    if (first == NULL)
        return false;
    else
        return true;
}

void DLList::SortedInsert(void *item, int sortKey)
{
    // is empty
    if (!this->IsEmpty())
    {
        DLLElement *temp = new DLLElement(item, sortKey);
        temp->prev = NULL;
        temp->next = NULL;
        first = temp;
        // <- error 3 --
        if (error_type == 3)
        {
            printf("Error 3\n");
            currentThread->Yield();
        }
        last = temp;
        // -- error 3 ->
    }
    // is not empty
    else
    {
        DLLElement *temp = new DLLElement(item, sortKey);
        DLLElement *traverse = first;
        // <- error 4 --
        if (error_type == 4)
        {
            printf("Error 4\n");
            currentThread->Yield();
        }
        // -- error 4 ->
        while (traverse != NULL && sortKey >= traverse->key)
        {
            // <- error 5 --
            if (error_type == 5)
            {
                printf("Error 5\n");
                currentThread->Yield();
            }
            // -- error 5 ->
            traverse = traverse->next;
        }
        // insert in the end of list
        if (traverse == NULL)
        {
            temp->prev = last;
            temp->next = NULL;
            last->next = temp;
            // <- error 6 --
            if (error_type == 6)
            {
                printf("Error 6\n");
                currentThread->Yield();
            }
            // -- error 6 ->
            last = temp;
        }
        // insert in the head of list
        else if (traverse == first)
        {
            temp->prev = NULL;
            temp->next = first;
            // <- error 7 --
            if (error_type == 7)
            {
                printf("Error 7\n");
                currentThread->Yield();
            }
            // -- error 7 ->
            first->prev = temp;
            // <- error 8 --
            if (error_type == 8)
            {
                printf("Error 8\n");
                currentThread->Yield();
            }
            // -- error 8 ->
            first = temp;
        }
        // insert in the middle of list
        else
        {
            // <- error 9 --
            if (error_type == 9)
            {
                printf("Error 9\n");
                currentThread->Yield();
            }
            temp->prev = traverse->prev;
            // -- error 9 ->
            traverse->prev->next = temp;
            // <- error 10 --
            if (error_type == 10)
            {
                printf("Error 10\n");
                currentThread->Yield();
            }
            traverse->prev = temp;
            // -- error 10 ->
            temp->next = traverse;
        }
    }
}

// remove first item with key==sortKey
// return NULL if no such item exists
void *DLList::SortedRemove(int sortKey)
{
    void *ReturnItem;
    // is empty
    if (!this->IsEmpty())
        return NULL;
    else
    {
        DLLElement *traverse = first;
        while (traverse != NULL && sortKey != traverse->key)
            traverse = traverse->next;
        if (traverse == NULL)
            return NULL;
        else
        {
            ReturnItem = traverse->item;
            // only one ele
            if (traverse == first)
            {
                int *key;
                this->Remove(key);
            }
            // in the end of list
            else if (traverse == last)
            {
                last = last->prev;
                last->next = NULL;
                if (last == NULL)
                    first = NULL;
            }
            // in the middle of list
            else
            {
                traverse->prev->next = traverse->next;
                traverse->next->prev = traverse->prev;
            }
            return ReturnItem;
        }
    }
}
