#include<iostream>
#include"system.h"
#include"dllist.cc"
using namespace std;

static void insertn(int n,DLList *list,DLList *insertone)
{
    for(int i=0;i<n;i++)
    {
        if(!insertone->IsEmpty())//no elements
        {
            break;
        }
        int kk;
        void *fulanda;
        fulanda=insertone->Remove(&kk);
        currentThread->Yield();
        list->SortedInsert(fulanda,kk);
    }
}

static void removen(int n,DLList *list,DLList *removeone)
{
    for(int i=0;i<n;i++)
    {
        if(!removeone->IsEmpty())//no elements
        {
            break;
        }
        if(!list->IsEmpty())//no elements
        {
            break;
        }
        int kk;
        void *fulanda;
        fulanda=removeone->Remove(&kk);
        currentThread->Yield();
        list->SortedRemove(kk);
    }
}
