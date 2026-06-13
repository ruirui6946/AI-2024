#include<iostream>
#include"dllist.h"
using namespace std;
DLLElement::DLLElement(void *itemPtr,int sortKey)
{
    item=itemPtr;
    key=sortKey;
    next=NULL;
    prev=NULL;
}

DLList::DLList()
{
    first=NULL;
    last=NULL;
}

DLList::~DLList()
{
    int k=0;
    while(Remove(&k)!=NULL);
}

void DLList::Prepend(void *item)
{
    if(!IsEmpty())//no elements
    {
        DLLElement *anew=new DLLElement(item,0);
        first=anew;
        last=anew;
    }
    else
    {
        DLLElement *anew=new DLLElement(item,first->key-1);
        anew->next=first;
        anew->prev=NULL;
        first->prev=anew;
        first=anew;
    }
}

void DLList::Append(void *item)
{
    if(!IsEmpty())//no elements
    {
        DLLElement *anew=new DLLElement(item,0);
        first=anew;
        last=anew;
    }
    else
    {
        DLLElement *anew=new DLLElement(item,last->key+1);
        anew->next=NULL;
        anew->prev=last;
        last->next=anew;
        last=anew;
    }
}

void* DLList::Remove(int *keyPtr)
{
    if(!IsEmpty())//no elements
    {
        return NULL;
    }
    *keyPtr=first->key;
    void *res=first->item;
    DLLElement *dropone;
    dropone=first;
    if(first==last)
    {
        first=NULL;
        last=NULL;
    }
    else
    {
        first=first->next;
        first->prev=NULL;
    }
    delete dropone;
    return res;
}

void DLList::SortedInsert(void *item,int sortKey)
{
    DLLElement *anew=new DLLElement(item,sortKey);
    if(!IsEmpty())//no elements
    {
        first=anew;
        last=anew;
    }
    else
    {
        DLLElement *p=first;
        for(;p!=NULL;p=p->next)//find the location
        {
	    if(p->key>sortKey)
	    {
	        break;
	    }
        }
        
        if(p==NULL)//is the last one
        {
            anew->prev=last;
            last->next=anew;
            last=last->next;
            last->next=NULL;
        }
        else if(p==first)//is the first one
        {
            anew->next=first;
            first->prev=anew;
            first=first->prev;
            first->prev=NULL;
        }
        else//is normal
        {
            p->prev->next=anew;
            anew->prev=p->prev;
            anew->next=p;
            p->prev=anew;
        }
    }
}

void* DLList::SortedRemove(int sortKey)
{
    DLLElement *p=first;
    for(;p!=NULL;p=p->next)
    {
        if(p->key==sortKey)
        {
            break;
        }
    }
    
    if(p==NULL)//empty or not found
    {
        return NULL;
    }
    else if(p==first)//is the first one
    {
        first=first->next;
        if(first!=NULL)
        {
            first->prev=NULL;
        }
        else
        {
            last=NULL;
        }
    }
    else if(p==last)//is the last one
    {
        last=last->prev;
        last->next=NULL;
    }
    else//is normal
    {
        p->prev->next=p->next;
        p->next->prev=p->prev;
    }
    
    void *res=p->item;
    delete p;
    return res;
}

bool DLList::IsEmpty()//empty->return false?
{
    if(first==NULL and last==NULL)
    {
        return false;
    }
    return true;
}
