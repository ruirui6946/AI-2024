//
// Created by jiang on 2018/3/24.
//

#ifndef NACHOS_DLLIST_H
#define NACHOS_DLLIST_H

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

private:
    DLLElement *first;
    DLLElement *last;
};


void genItems(DLList *dl, const int N);
void remove(DLList *dl, const int N);


#endif 
