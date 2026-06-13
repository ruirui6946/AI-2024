#include<iostream>
#include"dllist.h"
#include"system.h"
using namespace std;
inline DLLElement::DLLElement(void *itemPtr,int sortKey)
{
    item=itemPtr;
    key=sortKey;
    next=NULL;
    prev=NULL;
}

inline DLList::DLList()
{
    first=NULL;
    last=NULL;
}

inline DLList::~DLList()
{
    int k=0;
    while(Remove(&k)!=NULL);
}

inline void DLList::Prepend(void *item)
{
    if(!IsEmpty())//no elements
    {
        DLLElement *anew=new DLLElement(item,0);
        
        //cout<<"isempty in prepend"<<endl;
        first=NULL;
        currentThread->Yield();
        first=anew;
        
        currentThread->Yield();
        if(first!=anew)
        {
            cout<<"yes1"<<endl;
        }
        currentThread->Yield();
        if(first!=anew)
        {
            cout<<"yes2"<<endl;
        }
        currentThread->Yield();
        if(first!=anew)
        {
            cout<<"yes3"<<endl;
        }
        
        last=first;
        if(first!=last)
        {
            cout<<"error in SortedInsert, first!=last"<<endl;
        }
        if(anew==NULL)
        {
            cout<<"wo bu xiang xin"<<endl;
        }
        else
        {
            cout<<"wo xiang xin"<<endl;
        }
        if(last==NULL)
        {
            cout<<"error in SortedInsert, last==NULL"<<endl;
        }
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

inline void DLList::Append(void *item)
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

inline void* DLList::Remove(int *keyPtr)
{
    if(!IsEmpty())//no elements
    {
        return NULL;
    }
    void *res;
    DLLElement *dropone=first;

    *keyPtr=first->key;
    
    //cout<<"in Remove i yield"<<endl;
    currentThread->Yield();
    
    res=dropone->item;

    if(first==last)
    {
        first=NULL;
        last=NULL;
    }
    else
    {
        currentThread->Yield();
        first=first->next;
        first->prev=NULL;
    }
    delete dropone;
    return res;
}

inline void DLList::SortedInsert(void *item,int sortKey)
{
    DLLElement *anew=new DLLElement(item,sortKey);
    if(!IsEmpty())//no elements
    {
        //cout<<"isempty"<<endl;
        first=NULL;
        currentThread->Yield();
        first=anew;
        currentThread->Yield();
        last=first;
        if(first!=last)
        {
            cout<<"error in SortedInsert, first!=last"<<endl;
        }
        if(last==NULL)
        {
            cout<<"error in SortedInsert, last==NULL"<<endl;
        }
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
	    //cout<<"in SortedInsert i yield 2"<<endl;
            currentThread->Yield();
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
        //cout<<"in SortedInsert i yield 3"<<endl;
        currentThread->Yield();
    }
}

inline void* DLList::SortedRemove(int sortKey)
{
    DLLElement *p=first;
    for(;p!=NULL;p=p->next)
    {
        if(p->key==sortKey)
        {
            break;
        }
        //cout<<"in SortedRemove i yield 1"<<endl;
        currentThread->Yield();
    }
    
    if(p==NULL)//empty or not found
    {
        //cout<<"in SortedRemove i yield 2"<<endl;
        currentThread->Yield();
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
    
    //cout<<"in SortedRemove i yield 3"<<endl;
    currentThread->Yield();
    
    void *res=p->item;
    delete p;
    return res;
}

inline bool DLList::IsEmpty()//empty->return false?
{
    if(first==NULL and last==NULL)
    {
        return false;
    }
    return true;
}
