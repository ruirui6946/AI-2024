#include "copyright.h"
#include "utility.h"
#define START_KEY 100

class DLLElement {  
public:
    DLLElement( void *itemPtr, int sortKey );
    DLLElement *next;
    DLLElement *prev;
    int key;
    void *item;
};

class DLList {  
public:  

    DLList();
    ~DLList();

    void Prepend(void *item);
    void Append(void *item);
    void *Remove(int *keyPtr);
  

    bool IsEmpty();//声明判空函数  
  

    void SortedInsert(void *item, int sortKey);
    void *SortedRemove(int sortKey);
  
private:  

    DLLElement *first;
    DLLElement *last;
}; 


