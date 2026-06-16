//#include"Lseat.h"
#include"Lseat.cc"
#include<algorithm>
const int hall_line=100;    //最多可拥有的排数
class Hall
{
//public:
private:
    int line_num;   //排数
    int line_len;   //每排长度
    int free_all;   //总共空闲座位数
    //Lseat** h_seat_set;
    Lseat h_seat[hall_line];    //存放每排的类对象
    int rtime;      //可预订时间
    //int test_tmp;
public:
    Hall(){};
    Hall(int lnum,int llen,int time);
    bool hall_set(int lnum,int llen,int time);  //设定Hall
    //~Hall();

    string num2letter(int num);     //数字转字母函数
    int letter2num(string letter);  //字母转数字函数

    int free_seat();                //获取空闲椅子数
    int get_order_num(string name); //得到name预定的座位总数

    int update_h_mes();             //更新音乐厅作为信息
    bool h_timepass(int time);

    //预定/取消预定/购买连续几行
    bool order_lset(int sline,int num,string name,int time);
    bool buy_lset(int sline,int num,string name);
    bool deorder_lset(int sline,int num,string name);

    //预定/取消预定/购买任意几个
    bool order_ran(int num,string name,int time);
    bool buy_ran(int num,string name);
    bool deorder_ran(int num,string name);

    bool print_hall();                  //打印整个音乐厅的作为信息
    bool print_host_hall(string name);  //打印name预定/购买的所有座位信息

    //操作核心函数
    int auto_work(string command,vector<int> t_list);   //根据命令自动选择适合的函数
};
