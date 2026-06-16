#include"Aseat.h"
#include"Aseat.cc"
const int seat_line_num=100;    //一行最多的座位数

//Aseat类，表示一行/排座位
class Lseat
{
private:
//public:
    Aseat** l_seat_set;             //暂时不用
    Aseat l_seat[seat_line_num];    //座位集合
    int all_num;                    //总座位数
    int free_num;                   //空闲座位数
public:
    Lseat(){};
    Lseat(int num);
    bool line_set(int num);         //设置总座位数
    //~Lseat();

    int l_free_get();               //获取此行free座位总数
    int update_mes();               //更新信息（每个座位信息等）,返回free座位总数（方便Hall类调用）
    int l_get_order_num(string name);   //获取name预定的座位数

    bool l_all_free();              //此排全部空闲
    bool can_buy_l(string name);    //name可以购买整排
    bool l_timepass(int time);      //整排时间流逝

    //三个一组，后面只描述一个
    bool order_one(int num,string name,int time);   //预定1个
    bool buy_one(int num,string name);              //取消预定1个
    bool deorder_one(int num,string name);          //购买1个

    //预定/取消预定/购买连续几个
    bool order_set(int snum,int endnum,string name,int time);
    bool buy_set(int snum,int endnum,string name);
    bool deorder_set(int snum,int endnum,string name);

    //预定/取消预定/购买整排
    bool order_line(string name,int time);
    bool buy_line(string name);
    bool deorder_line(string name);

    //预定/取消预定/购买一些
    bool order_some(int num,string name,int time);
    bool buy_some(int num,string name);
    bool deorder_some(int num,string name);

    void print_l();                 //打印整排座位信息
    void print_host_l(string name); //打印name预定/购买的座位信息
};
