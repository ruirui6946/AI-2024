#include<iostream>
#include<string>
#include"mysql.h"
#pragma once

using namespace std;

class jingani_painter
{
private:
	MYSQL* con;
	MYSQL_RES *res;
	MYSQL_ROW row;
	string name;
public:
	jingani_painter(MYSQL* ori, string n)
	{
		con = ori;
		name = n;
	}
	//通用函数
	void print_res();               //打印查询结果
	bool to_update(string tmp);     //更新语句
	
	//基本查询与检查函数
	void select_edit(string mode);                //选择编辑，根据不同模式去查找编辑
	bool check_editor(string editor,string mode); //检查编辑是否可以接受、预约原画审核（根据mode选择）
	bool check_num(string num);                   //检查提交原画编号是否合法
	void mode_check(string mode);                 //按模式查找审核记录
	void paint_in_store();                        //查询画师已经入库过的原画信息

	//交付、预约
	void add_check(string editor, string num,string source,string status);  //增加一条审核记录
	void change_editor(string editor,string status);                        //相应地修改编辑信息

	/*注册函数*/
	bool regist();       //注册函数
	bool name_exist();   //注册时姓名检察函数

	/*进阶函数*/
	void re_check(string num);     //重新提交审核，待实现
};

bool check_painter(MYSQL* con, string name, string password);  //判断画师登录信息

/*
Table painter
{
	Name     varchar(20) PK
	Sex      varchar(2)
	Style    varchar(20)
	Level    int
	password varchar(20)
}

Table check
{
	ori_p_num    varchar(10) PK
	Edit_name    varchar(20) PK
	Source       varchar(30)
	Painter_name varchar(20)
	Status       varchar(10)
	Quality      int
	pass_time    datetime
	instorige    bit(1)
}

Table editor
{
	Name       varchar(20) PK
	Sex        varchar(2)
	max_edit   int
	now_edit   int
	queue_edit int
	Level      int
	password   varchar(20)
}
*/