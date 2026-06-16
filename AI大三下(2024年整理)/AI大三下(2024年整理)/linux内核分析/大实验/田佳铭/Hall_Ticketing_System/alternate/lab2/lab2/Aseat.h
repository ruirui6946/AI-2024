#include<iostream>
#include<string.h>
using namespace std;
const int sfree=0;
const int ordered=1;
const int sold=2;
const string nohost="1nohost";//Real name can't start with digit
class Aseat
{
private:
//public:
    int left_time;
    int state;
    //string host;
    char host[100];
public:
    Aseat();
    bool host_set(string name);
    string host_get();
    int state_get();
    int left_get();
    
    bool canchange();
    bool canbuy(string name);
    
    bool timepass(int time);
    bool order(int time,string name);
    bool deorder(string name);
    bool buy(string name);
    
    void print_seat(); 
};
