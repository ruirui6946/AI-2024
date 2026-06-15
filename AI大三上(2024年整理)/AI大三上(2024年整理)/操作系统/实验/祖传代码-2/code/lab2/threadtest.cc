#include <stdlib.h>
#include <time.h>
#include "copyright.h"
#include "system.h"
#include "dllist.h"
#include "Table.h"
#include "BoundedBuffer.h"

extern void insert(int n, DLList &dllist);
extern void remove(int n, DLList &dllist);
extern void YIELD(int place);

static int *fargv;
static int fargc;
static int testNum;
static DLList dllist;
static Table *table;
static BoundedBuffer *buffer;

void ThreadInit(int* argv, int argc)
{
   if(argc > 0)
   {
        testNum = argv[0];
        if(argc > 1)
        {
            fargv = argv + 1;
            fargc = argc - 1;
        } 
        else
        {
            fargv = NULL;
            fargc = 0;
        } 
   }
}

int getIntBit(int x)
{
    int n = 0;
    do ++ n, x /= 10; while(x != 0);
    return n;
}

void makeRandThread(void(*func)(int arg), char *kind, int id, int MinPriority)
{
    int n1 = strlen(kind), n2 = getIntBit(id);
    char* name = new char[n1 + n2 + 2];

    name[0] = '\0';
    strcat(name, kind);
    sprintf(name + n1, " %d", id);

    Thread *thread = new Thread(name, rand() % MinPriority);
    thread->Fork(func, id);

    DEBUG('e', "%s: make thread %s %d\n", currentThread->getName(), thread->getName(), thread->getPriority());
}

void makeThread(void(*func)(int arg), char *kind, int id, int priority)
{
    int n1 = strlen(kind), n2 = getIntBit(id);
    char* name = new char[n1 + n2 + 2];

    name[0] = '\0';
    strcat(name, kind);
    sprintf(name + n1, " %d", id);

    Thread *thread = new Thread(name, priority);
    thread->Fork(func, id);

    DEBUG('e', "%s: make thread %s %d\n", currentThread->getName(), thread->getName(), thread->getPriority());
}

void makeThread(void(*func)(int arg), char *kind, int id)
{
    int n1 = strlen(kind), n2 = getIntBit(id);
    char* name = new char[n1 + n2 + 2];

    name[0] = '\0';
    strcat(name, kind);
    sprintf(name + n1, " %d", id);

    Thread *thread = new Thread(name);
    thread->Fork(func, id);

    DEBUG('e', "%s: make thread %s\n", currentThread->getName(), thread->getName());
}

void SimpleThread(int which)
{
    int N = fargv[1];
    DEBUG('e', "%s is running.\n", currentThread->getName());
    YIELD(60);
    insert(N, dllist);
    YIELD(61);
    remove(N, dllist);
    YIELD(62);
    DEBUG('e', "%s is finish.\n", currentThread->getName());
    dllist.show();
}

void test1()
{
    ASSERT(fargc == 2);
    int T = fargv[0];
    srand(time(0));
    for (int i = 0; i < T; ++ i)
    {
        makeThread(SimpleThread, "thread", i);
    }
}

void dllistInsert(int witch)
{
    DEBUG('e', "%s is running.\n", currentThread->getName());
    int n1 = fargv[1];
    insert(n1, dllist);
    DEBUG('e', "%s is finish.\n", currentThread->getName());
}

void dllistRemove(int witch)
{
    DEBUG('e', "%s is running.\n", currentThread->getName());
    int n2 = fargv[3];
    remove(n2, dllist);
    DEBUG('e', "%s is finish.\n", currentThread->getName());
}

void test2()
{
    ASSERT(fargc == 4);
    int t1 = fargv[0], t2 = fargv[2];
    srand(time(0));
    for(int i = 0; i < t1; ++ i)
        makeRandThread(dllistInsert, "Insert", i, 100);
    for(int i = 0; i < t2; ++ i)
        makeRandThread(dllistRemove, "Remove", i, 100);
}

void test3()
{
    ASSERT(fargc == 0);
    table = new Table(3);
    int index = 0;
    void *object;
    // 空表分配
    index = table->Alloc(new int[1]);
    ASSERT(index == 0);
    // 非空表非满表分配
    index = table->Alloc(new int[1]);
    ASSERT(index == 1);
    index = table->Alloc(new int[1]);
    ASSERT(index == 2);
    // 满表分配
    index = table->Alloc(new int[1]);
    ASSERT(index == -1);
    // 验证每个表槽与分配时一致
    for(int i = 0; i < 3; ++ i)
        table->Get(i);
    // 每个表槽释放再重新分配
    for(int i = 0; i < 3; ++ i)
    {
        table->Release(i);
        index = table->Alloc(new int[1]);
        ASSERT(index == i)
    }
    // 全部释放
    for(int i = 0; i < 3; ++ i)
        table->Release(i);
    // 验证每个表槽都释放成功
    for(int i = 0; i < 3; ++ i)
    {
        object = table->Get(i);
        ASSERT(object == NULL);
    }
}

void TableAlloc(int witch)
{
    table->Alloc(new int[1]);
}

void TableGet(int witch)
{
    int size = fargv[0];
    table->Get(rand() % size);
}

void TableRelease(int witch)
{
    int size = fargv[0];
    table->Release(rand() % size);
}

void test4()
{
    ASSERT(fargc == 4);
    int size = fargv[0], n1 = fargv[1], n2 = fargv[2], n3 = fargv[3];
    table = new Table(size);
    srand(time(0));

    for (int i = 0; i < n1; ++ i)
    {
        makeRandThread(TableAlloc, "Alloc", i, 100);
    }
    for (int i = 0; i < n2; ++ i)
    {
        makeRandThread(TableGet, "Get", i, 100);
    }
    for (int i = 0; i < n3; ++ i)
    {
        makeRandThread(TableRelease, "Release", i, 100);
    }
}

void Writer(int witch)
{
    int n1 = fargv[2];
    char *data = new char[n1 + 1];
    
    for(int i = 0; i < n1; ++ i) 
        data[i] = 'a' + rand() % 26;
    data[n1] = '\0';
    
    DEBUG('e', "%s: want to write %s.\n", currentThread->getName(), data);
    buffer->Write(data, n1);
}

void Reader(int witch)
{
    int n2 = fargv[4];
    char *data = new char[n2 + 1];
    data[n2] = '\0';

    buffer->Read(data, n2);
    DEBUG('e', "%s: read %s.\n", currentThread->getName(), data);
}

void test5()
{
    ASSERT(fargc == 5);
    int size = fargv[0], t1 = fargv[1], t2 = fargv[3];
    buffer = new BoundedBuffer(size);
    srand(time(0));
    for(int i = 0; i < t1; ++ i)
    {
        makeRandThread(Writer, "Writer", i, 100);
    }
    for(int i = 0; i < t2; ++ i)
    {
        makeRandThread(Reader, "Reader", i, 100);
    }
}

void thread6(int witch)
{
    printf("%s %d begin to run.\n", currentThread->getName(), currentThread->getPriority());
}

void test6()
{
    ASSERT(fargc == 1);

    int n = fargv[0];
    srand(time(0));
    for(int i = 0; i < n; ++ i)
        makeRandThread(thread6, "mythread", i, 100);
}

void Writer0(int witch)
{
    int n0 = fargv[2];
    char *data = new char[3];
    
    data[2] = '\0';
    for(int i = 0; i < n0; ++ i) 
    {
        // data[0] = 'a' + rand() % 26;
        // data[1] = 'a' + rand() % 26;
        data[0] = 'a' + i * 2;
        data[1] = 'a' + i * 2 + 1;
        DEBUG('e', "%s: want to write %s.\n", currentThread->getName(), data);
        buffer->Write(data, 2);
        YIELD(100);
    }
}

void Writer1(int witch)
{
    int n1 = fargv[4];
    char *data = new char[3];
    
    data[2] = '\0';
    for(int i = 0; i < n1; ++ i) 
    {
        // data[0] = 'a' + rand() % 26;
        // data[1] = 'a' + rand() % 26;
        data[0] = 'A' + i * 2;
        data[1] = 'A' + i * 2 + 1;
        DEBUG('e', "%s: want to write %s.\n", currentThread->getName(), data);
        buffer->Write(data, 2);
        YIELD(100);
    }
}

void Reader0(int witch)
{
    int n2 = fargv[6];
    char *data = new char[3];

    data[2] = '\0';
    for(int i = 0; i < n2; ++ i) 
    {
        buffer->Read(data, 2);
        DEBUG('e', "%s: read %s.\n", currentThread->getName(), data);
        YIELD(100);
    }
}

void test7()
{
    ASSERT(fargc == 7);
    int size = fargv[0], p0 = fargv[1], p1 = fargv[3], p2 = fargv[5];

    buffer = new BoundedBuffer(size);
    srand(time(0));
    makeThread(Writer0, "Writer", 0, p0);
    makeThread(Writer1, "Writer", 1, p1);
    makeThread(Reader0, "Reader", 0, p2);
}

void ThreadTest()
{
    switch(testNum)
    {
        case 1:
            test1();
            break;
        case 2:
            test2();
            break;
        case 3:
            test3();
            break;
        case 4:
            test4();
            break;
        case 5:
            test5();
            break;
        case 6:
            test6();
            break;
        case 7:
            test7();
            break;
        default:
            ASSERT(false);
            break;   
    }
}
