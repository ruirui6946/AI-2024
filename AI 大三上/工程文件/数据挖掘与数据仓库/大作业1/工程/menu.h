#pragma once
#include<iostream>

using namespace std;

void menu_painter()
{
	cout << "输入相应的字符获取相应的服务" << endl
		 << "mode check        根据模式查询审查表信息" << endl
		 << "find editors      查找不同状态的编辑" << endl
		 << "delivery          交付原画" << endl
		 << "reserve           预约原画" << endl
		 << "re_delivery       重新交付原画" << endl
		 << "my paint instorge 查询自己已经入库的原画信息" << endl
		 << "exit              注销" << endl
		 << "其他              重新输入" << endl << endl;
}

void menu_editor()
{
	cout << "输入相应的字符获取相应的服务" << endl
		 << "check task 根据模式查询自己的审查任务" << endl
		 << "take       接收预约的原画" << endl
		 << "check      审核原画" << endl
		 << "storage    入库原画" << endl
		 << "exit       注销" << endl
		 << "其他       重新输入" << endl << endl;
}

void menu_tv_station()
{
	cout << "输入相应的字符获取相应的服务" << endl
		<< "check list      查询所有的播放计划" << endl
		<< "check anime     查找某一番剧的播放计划" << endl
		<< "add list        增加一条本电视台的播放计划" << endl
		<< "change list     修改一条本电视台的播放计划" << endl
		<< "put off list    推迟播放某一播放计划" << endl
		<< "advance list    提前播放某一播放计划" << endl
		<< "check with time 查询某一时间段内自己电视台的播放计划" << endl
		<< "exit            注销" << endl
		<< "其他            重新输入" << endl << endl;
}

void menu_in()
{
	cout << "请输入以下字符串来让我们识别您的身份" << endl
		 << "识别身份后请根据提示输入登录信息" << endl
	  	 << "admit      您是管理员" << endl
		 << "painter    您是画师" << endl
		 << "editor     您是编辑" << endl
		 << "tv station 您是电视台管理员" << endl
		 << "test       您是测试员" << endl
		 << "exit       您要退出系统" << endl
		 << "其他       您要重新输入" << endl << endl;
}

void menu_begin()
{
	cout << "欢迎来到京阿尼原画管理系统原型机" << endl
		 << "请根据提示完成操作" << endl
		 << "数据库结构请参照相关文档，如需求分析、“数据库结构.txt”等" << endl << endl;
}