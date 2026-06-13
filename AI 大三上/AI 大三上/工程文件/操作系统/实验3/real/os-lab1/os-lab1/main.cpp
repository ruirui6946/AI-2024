#include"dllist-driver.cc"
#include<iostream>
using namespace std;

void print(DLList *list);

int main()
{
    cout<<"hello world"<<endl;
    
    //to creat the first list
    DLList one_ori;
    DLList *one=&one_ori;
    
    //to test Prepend
    for(int i=0;i<5;i++)
    {
        one->Prepend(&i);
    }
    
    //to test Append
    for(int i=-5;i<0;i++)
    {
        one->Append(&i);
    }
    
    //to test Remove
    int kk=0;
    int m=0;
    void *fulanda=(void *)&m;
    cout<<"i wanna remove"<<endl;
    fulanda=one->Remove(&kk);
    cout<<" "<<kk;
    cout<<endl<<"test1:";
    print(one);
    
    
    //to test SortedInsert
    for(int i=0;i<5;i++)
    {
        one->Prepend(&i);
    }
    one->SortedInsert(fulanda,9);
    cout<<"test2:";
    print(one);
    
    //to test SortedRemove
    for(int i=0;i<5;i++)
    {
        one->Prepend(&i);
    }
    one->SortedInsert(fulanda,-3);
    cout<<"test3:";
    one->SortedRemove(-3);
    print(one);

    //to test SortedRemove 2
    for(int i=0;i<5;i++)
    {
        one->Prepend(&i);
    }
    one->SortedInsert(fulanda,-3);
    cout<<"test3-2:";
    one->SortedRemove(100);
    print(one);

    //to test SortedRemove 3
    //no member
    one->SortedInsert(fulanda,-3);
    cout<<"test3-3:";
    one->SortedRemove(-3);
    //one->SortedRemove(100);
    print(one);

    //to creat the second list
    DLList two_ori;
    DLList *two=&two_ori;

    //to test insertn 1
    for(int i=0;i<5;i++)
    {
        one->Prepend(&i);
    }
    for(int i=-5;i<0;i++)
    {
        two->Append(&i);
    }
    insertn(5,one,two);
    cout<<"test4:";
    print(one);
    print(two);

    
    //to test insertn 2
    //to test can it stop when two_ori has no elements
    for(int i=0;i<5;i++)
    {
        one->Prepend(&i);
    }
    for(int i=-5;i<0;i++)
    {
        two->Append(&i);
    }
    insertn(10,one,two);
    cout<<"test5:";
    print(one);
    print(two);
    
    //to test insertn 3
    for(int i=0;i<5;i++)
    {
        one->Prepend(&i);
    }
    for(int i=-5;i<0;i++)
    {
        two->Append(&i);
    }
    insertn(3,one,two);
    cout<<"test6:";
    print(one);
    print(two);

    //to test removen 1
    for(int i=0;i<5;i++)
    {
        one->Prepend(&i);
    }
    for(int i=0;i<5;i++)
    {
        two->Prepend(&i);
    }
    removen(1,one,two);
    cout<<"test7:";
    print(one);
    print(two);
    
    //to test removen 2
    //all member of list2
    for(int i=0;i<6;i++)
    {
        one->Prepend(&i);
    }
    for(int i=0;i<5;i++)
    {
        two->Prepend(&i);
    }
    removen(5,one,two);
    cout<<"test8:";
    print(one);
    print(two);
    
    //to test removen 2
    //to test can it stop when two_ori has no elements
    for(int i=0;i<6;i++)
    {
        one->Prepend(&i);
    }
    for(int i=0;i<5;i++)
    {
        two->Prepend(&i);
    }
    removen(10,one,two);
    cout<<"test9:";
    print(one);
    print(two);
    
    return 0;
}

void print(DLList *list)
{
    //print all elements from the head to the tail
    //notice: this function will destory the list
    if(!list->IsEmpty())
    {
        cout<<"no elements at all"<<endl;
    }
    int kk;
    void *fulanda;
    while(list->IsEmpty())
    {
        fulanda=list->Remove(&kk);
        cout<<" "<<kk;
    }
    cout<<endl;
}
