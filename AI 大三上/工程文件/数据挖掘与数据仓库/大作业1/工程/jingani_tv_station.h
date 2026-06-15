#include<iostream>
#include<string>
#include"mysql.h"
#pragma once

using namespace std;

class jingani_tv_station
{
private:
	MYSQL* con;
	MYSQL_RES *res;
	MYSQL_ROW row;
	string name;
public:
	jingani_tv_station(MYSQL* ori,string n)
	{
		con = ori;
		name = n;
	}
	
	//通用函数
	void print_res();                //打印查找结果
	bool to_update(string tmp);      //更新语句

	//查找函数
	void check_list_all();                              //检查所有的播放列表
	void check_anime(string anime);                     //检查某一番剧的播放列表
	void check_time(string start_time,string end_time); //查询播放时间在此时间段内的播放列表

	//获取函数
	string get_start_time(string anime, string source, int episode);
	string get_end_time(string anime, string source, int episode);
	//得到某一播放列表的开始时间或者结束时间

	//增加函数
	void add_a_list(string anime,string source,int episode,string start_time,string end_time);
	//在播放列表里增加一条播放计划

	//改变函数
	void change_time(string anime, string source, int episode, string start_time, string end_time);

	/*注册函数*/
	bool regist();       //注册函数
	bool name_exist();   //注册时电视台名检察函数
};

bool check_tv_station(MYSQL* con, string name, string password);   //判断电视台登录信息

/*
Table tv_station
{
	Name       varchar(50) PK
	Local      varchar(20)
	Credit     int
	Value      int
	Reputation int
	password   varchar(20)
}

Table tv_list
{
	TV_name    varchar(50) PK
	Anime      varchar(50) PK
	Source     varchar(30) PK
	Episode    int         PK
	start_time datetime
	end_time   datetime
}
*/

