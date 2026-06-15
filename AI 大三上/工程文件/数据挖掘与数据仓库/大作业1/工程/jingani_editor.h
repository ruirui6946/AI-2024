#include<iostream>
#include<string>
#include"mysql.h"
#include<vector>
#pragma once

using namespace std;

class jingani_editor
{
private:
	MYSQL* con;
	MYSQL_RES *res;
	MYSQL_ROW row;
	string name;   //保存编辑姓名
public:
	jingani_editor(MYSQL* ori, string n)
	{
		con = ori;
		name = n;
	}

	//公用函数
	void print_res();                           //查询结果打印
	bool to_update(string tmp);                 //更新语句
    
	//基本查询函数
	void check_task(string status);             //查询任务

	//基本检查函数
	bool check_num(string num, string status);  //检查原画是否可操作

	/*关于审查的操作*/
	//take
	bool can_take();                            //检查自己是否可从take预约
	void take_task(string num);                 //从预约审查中接手
    
	//check
	void check(string num, string status);                       //增加审查记录
	void give_score(string num, string score,string datetime);   //给通过的原画打分

	/*注册函数*/
	bool regist();       //注册函数
	bool name_exist();   //注册时姓名检察函数
	
	/*关于入库的操作*/
	//自动全部入库
	void to_instore_all(string datetime);          //增加新原画信息,并且自动全部入库，待实现

	//first step
	void insert_storage(string num, string datetime);  //增加入库记录（1）
	
	//second step
	void to_instore(string num,string datetime);       //增加新原画信息（2）
	void insert_ori_paint(string num, string source, string painter_name, string quality);//被to_instore调用

	//final step
	void ready_instore(string num);    //已经入库了，修改check状态（3）
};

bool check_editor(MYSQL* con, string name, string password);   //判断编辑登录信息

/*
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

Realtion check
{
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

	Table painter
	{
		Name     varchar(20) PK
		Sex      varchar(2)
		Style    varchar(20)
		Level    int
		password varchar(20)
	}
}

Relation storage
{
    Table ori_paint
	{
	    Num          varchar(10) PK 
	    Source       varchar(30) 
	    Painter_name varchar(20) 
	    Style        varchar(20) 
	    Quality      int
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

	Table storage
	{
		paint_num  varchar(10) PK
		edit_name  varchar(20) PK
		store_time datetime
	}
}
*/