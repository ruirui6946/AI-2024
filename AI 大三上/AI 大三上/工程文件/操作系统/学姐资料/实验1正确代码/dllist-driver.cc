#include <cstdlib>
#include "dllist.h"
#include "system.h"
#include "utility.h"
#include <time.h>

char item[] = "test_item：";
int a;

void DLLGenerate(DLList *list,int n, int which)
{
	srand(time(0));
    for (int i = 0; i < n; i++)
    {
	int key = rand()%100;
	a=key;
	DEBUG('p', "Thread %d is inserting key %d\n", which, key);
		printf("next key %d\n",key);
        list->SortedInsert(item, key);
		printf("Thread %d inserted key %d\n", which, key);
    }
    
}

void DLLRemove(DLList *list,int n, int which)
{
    int key;
    for (int i = 0; i < n; i++)
    {
        list->Remove(&key);
        printf("Thread %d removed key %d\n", which, key);
    }
}
