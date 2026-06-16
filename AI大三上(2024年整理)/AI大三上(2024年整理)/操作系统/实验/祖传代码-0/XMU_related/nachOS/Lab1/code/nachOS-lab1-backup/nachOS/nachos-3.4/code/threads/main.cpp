#include <iostream>
#include "dllist.h"

int main() {
    DLList dl;
    void* item = nullptr;
    genItems(&dl, 3);

    dl.Prepend(item);

    std::cout << "--------------\n";
    dl.SortedRemove(-6);
    remove(&dl, 3);
    return 0;
}