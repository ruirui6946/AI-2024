#include<iostream>
#include<string>
#include"mysql.h"
#pragma once

using namespace std;

class jingani_admit
{
private:
	MYSQL* con;
	MYSQL_RES *res;
	MYSQL_ROW row;
public:
	jingani_admit(MYSQL* ori)
	{
		con = ori;
	}

	void command();   //直接输入SQL语句，进行上帝查询
	void print_res(); //打印结果
};

bool check_admit(string table, MYSQL* con, string account, string password);  //判断管理员登录信息

/*
Table Admit
{
	Account  varchar(20) PK
	password varchar(20)
}
*/