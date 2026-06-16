//
// Created by jiang on 2018/3/23.
//

#include "dllist.h"
#include <iostream>
#include <ctime>

// test program, genItems to generate N keys and use sortedInsert to add to list
void genItems(DLList *dl, const int N){
    int key[N];
    srand(time(NULL));
    for(int i=0;i<N;i++){
        key[i] = rand()%100;
    }

    // print out the keys inserted
    for(int i=0;i<N;i++){
        std::cout << key[i] << std::endl;
    }

    int *item = NULL;
    for(int i=0;i<N;i++){
        dl->SortedInsert((void*)item, key[i]);
    }
}

// remove function calls DLList::Remove to remove and print out all the keys
void remove(DLList *dl, const int N){

    int key;
    for(int i=0;i<N;i++){
        dl->Remove(&key);
        std::cout << key << std::endl;
    }
}
