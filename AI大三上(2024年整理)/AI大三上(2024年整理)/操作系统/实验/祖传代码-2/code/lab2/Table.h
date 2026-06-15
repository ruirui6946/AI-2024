# ifndef TABLE_H
# define TABLE_H
# include "synch.h"
class Table
{
public:
    Table(int size);     // Create a table to hold at most size entries.
    ~Table();
    int Alloc (void* object);   // Allocate a table slot for object, 
                                // returning index of the allocated entry.
                                // Return an error (-1) if no free table slots are available.
    void* Get (int index);  // Retrieve the object from table slot at index, 
                            // or NULL if not allocated. 
    void Release (int index);    // Free the table slot at index.
private:
    int TableSize;
    void **table;
    Semaphore* lock;
};
# endif
