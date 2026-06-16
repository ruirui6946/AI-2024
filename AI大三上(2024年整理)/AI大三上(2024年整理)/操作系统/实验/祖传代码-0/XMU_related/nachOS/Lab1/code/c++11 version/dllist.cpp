//
// Created by jiang on 2018/3/20.
//

#include "dllist.h"

const int min = 0;
const int max = 1;

DLLElement::DLLElement(void *itemPtr, int sortKey){
    item = itemPtr;
    key = sortKey;
}

DLList::DLList() {
    first = nullptr;
    last = nullptr;
}

DLList::~DLList(){
    int key;
    if(!isEmpty()){
        Remove(&key);
    }
}

int findKey(DLLElement* first, DLLElement* last, int mark){
    auto fst = first;
    auto lst = last;
    auto key = lst->key;
    if(mark == min){        // if we need min key, then do while loop to find minimum key
        while(fst!=lst){
            if(fst->key < key)
                key = fst->key;
            fst = fst->next;
        }
    }
    if(mark == max){        // if we need max key, then do while loop to find maximum key
        while(fst!=lst){
            if(fst->key > key)
                key = fst->key;
            fst = fst->next;
        }
    }
    return key;
}

bool DLList::isEmpty() {
    if(first == nullptr && last == nullptr)
        return true;
    else
        return false;
}

void DLList::Prepend(void *item) {          // add to head of list (set key = min_key-1, at first, min_key = 0)
    int key;
    if(isEmpty()){                          // if list is empty, then allocate key as 0, make first = last = newElm
        key = 0;
        DLLElement* newElm = new DLLElement(item, key);
        newElm->prev = nullptr;
        newElm->next = nullptr;
        first = newElm;
        last = newElm;
    }
    else{
        key = findKey(first, last, min) - 1;
        DLLElement* newElm = new DLLElement(item, key);

        newElm->prev = nullptr;
        newElm->next = first;

        first->prev = newElm;
        first = newElm;
    }
}

void DLList::Append(void *item) {       // add to tail of list (set key = max_key+1, at first, max_key = 0)
    int key;
    if(isEmpty()){                      // if list is empty, then allocate key as 0, make first = last = newElm
        key = 0;
        DLLElement* newElm = new DLLElement(item, key);
        newElm->prev = nullptr;
        newElm->next = nullptr;
        first = newElm;
        last = newElm;
    }
    else{
        key = findKey(first, last, max) + 1;
        DLLElement* newElm = new DLLElement(item, key);

        newElm->next = nullptr;
        newElm->prev = last;

        last->next = newElm;
        last = newElm;
    }
}

void *DLList::Remove(int *keyPtr) {

    if(isEmpty())       // if list is empty, then there is no need to search
        return nullptr;

    if(first == last){
        auto fst = first;
        first = nullptr;
        last = nullptr;
        *keyPtr = fst->key;

        void *item = fst->item;
        delete fst;
        return item;
    }
    else{
        auto fst = first;
        first = first->next;
        first->prev = nullptr;
        *keyPtr = fst->key;

        void *item = fst->item;
        delete fst;
        return item;
    }
}

void DLList::SortedInsert(void *item, int sortKey) {

    DLLElement *newElm = new DLLElement(item, sortKey);

    if(isEmpty()){
        newElm->prev = nullptr;
        newElm->next = nullptr;
        first = newElm;
        last = newElm;
        return;
    }
    if(sortKey < (first->key)){     // if sortKey is even smaller than first key, then do preappend.
        newElm->prev = nullptr;
        newElm->next = first;
        first->prev = newElm;
        first = newElm;
        return;
    }
    if(sortKey >= (last->key)){   // if sortKey is even larger than last key, then do append
        newElm->next = nullptr;
        newElm->prev = last;
        last->next = newElm;
        last = newElm;
        return;
    }

    // else, assert newElm in the middle
    auto fst = first;
    auto lst = last;
    while(fst!=lst){
        fst = fst->next;
        if(sortKey <= (fst->key)){
            newElm->prev = fst->prev;
            newElm->next = fst;
            fst->prev->next = newElm;
            fst->prev = newElm;
            return;
        }
    }
}

void *DLList::SortedRemove(int sortKey) {

    if(isEmpty())       // if list is empty, then there is no need to search
        return nullptr;

    auto fst = first;
    auto lst = last;
    if(fst->key == sortKey){        // judge if sortKey is in first
        fst->next->prev = nullptr;
        first = fst->next;

        int *item = (int*)(fst->item);
        delete fst;
        return (void*)item;
    }
    if(lst->key == sortKey){        // judge if sortKey is in last
        lst->prev->next = nullptr;
        last = lst->prev;

        int *item = (int*)(lst->item);
        delete lst;
        return (void*)item;
    }


    while(fst!=lst){                // judge if sortKey is in middle node
        fst = fst->next;
        if(fst->key == sortKey){    // if find key, then set link relation and return
            fst->prev->next = fst->next;
            fst->next->prev = fst->prev;

            void *item = fst->item;
            delete fst;
            return item;
        }
    }

    return nullptr;        // if cannot find sortKey, then return nullptr
}


