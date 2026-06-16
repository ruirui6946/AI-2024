#include"Aseat.h"
#include"Aseat.cc"
const int seat_line_num=100;
class Lseat
{
private:
//public:
    Aseat** l_seat_set;
    Aseat l_seat[seat_line_num];
    int all_num;
    int free_num;
public:
    Lseat(){};
    Lseat(int num);
    bool line_set(int num);
    //~Lseat();
    
    int l_free_get();
    int update_mes();
    int l_get_order_num(string name);
    
    bool l_all_free();
    bool can_buy_l(string name);
    bool l_timepass(int time);
    
    bool order_one(int num,string name,int time);
    bool buy_one(int num,string name);
    bool deorder_one(int num,string name);
    
    bool order_set(int snum,int endnum,string name,int time);
    bool buy_set(int snum,int endnum,string name);
    bool deorder_set(int snum,int endnum,string name);
    
    bool order_line(string name,int time);
    bool buy_line(string name);
    bool deorder_line(string name);
    
    bool order_some(int num,string name,int time);
    bool buy_some(int num,string name);
    bool deorder_some(int num,string name);
    
    void print_l();
    void print_host_l(string name);
};
