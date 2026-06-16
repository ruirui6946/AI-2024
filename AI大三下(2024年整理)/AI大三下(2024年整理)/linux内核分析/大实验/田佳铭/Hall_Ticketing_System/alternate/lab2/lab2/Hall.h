//#include"Lseat.h"
#include"Lseat.cc"
#include<algorithm>
class Hall
{
//public:
private:
    int line_num;
    int line_len;
    int free_all;
    //Lseat** h_seat_set;
    Lseat h_seat[100];
    int rtime;
    //int test_tmp;
public:
    Hall(){};
    Hall(int lnum,int llen,int time);
    bool hall_set(int lnum,int llen,int time);
    //~Hall();
    
    string num2letter(int num);
    int letter2num(string letter);
    
    int free_seat();
    int get_order_num(string name);
    
    int update_h_mes();
    bool h_timepass(int time); 
    
    bool order_lset(int sline,int num,string name,int time);
    bool buy_lset(int sline,int num,string name);
    bool deorder_lset(int sline,int num,string name);
    
    bool order_ran(int num,string name,int time);
    bool buy_ran(int num,string name);
    bool deorder_ran(int num,string name);
    
    bool print_hall();
    bool print_host_hall(string name);
    
    int auto_work(string command,vector<int> t_list);
};
