//
// Created by ForenewHan on 2018/3/24.
//

#ifndef DLLIST_H
#define DLLIST_H

#include "utility.h"

class DLLElement{
public:
    DLLElement(void *itemPtr, int sortKey);
    DLLElement *next;
    DLLElement *prev;
    int key;
    void *item;
};

class DLList{
    friend int findKey(DLLElement* first, DLLElement *last, int mark);
public:
    DLList();
    ~DLList();

    void Prepend(void *item);
    void Append(void *item);
    void *Remove(int *keyPtr);

    bool isEmpty();

    void SortedInsert(void *item, int sortKey);
    void *SortedRemove(int sortKey);

    void Mapcar(VoidFunctionPtr func);

private:
    DLLElement *first;
    DLLElement *last;
};


void genItems(DLList *dl, const int N);
void remove(DLList *dl, const int N);


#endif 
