#include<iostream>
#include<string.h>
using namespace std;
const int sfree=0;      //空闲状态
const int ordered=1;    //被预定状态
const int sold=2;       //被买状态
const int len_host=100; //允许的最长姓名长度
const string nohost="1nohost";//Real name can't start with digit

//Aseat类，表示单个座位
class Aseat
{
private:
//public:
    int left_time;          //预约剩余时间，仅state=ordered时有效（单位s）
    int state;              //座位状态，可取free、ordered、sold
    //string host;
    char host[len_host];    //座位的预定/购买者
public:
    Aseat();
    bool host_set(string name); //设定座位的预定/购买者
    string host_get();          //获取座位的预定/购买者
    int state_get();            //获取座位状态
    int left_get();             //获取预约剩余时间

    bool canchange();           //此座位可被预定或购买
    bool canbuy(string name);   //此座位可被name购买

    bool timepass(int time);    //时间流逝函数
    bool order(int time,string name);   //name预定某座位，成功返回true
    bool deorder(string name);  //name取消预定某座位，成功返回true
    bool buy(string name);      //name购买某座位，成功返回true

    void print_seat();          //打印座位信息
};
