#include<iostream>
#include<string>
#include"mysql.h"

#include"jingani_admit.h"
#include"jingani_painter.h"
#include"jingani_editor.h"
#include"jingani_tv_station.h"

#include"mytime.h"

#include"menu.h"

#pragma once

using namespace std;

void work_admit(string table,MYSQL* con)
{
	string account;
	string password;
	cout << "请输入账号" << endl;
	getline(cin, account);
	cout << "请输入密码" << endl;
	getline(cin, password);

	if (check_admit(table,con,account,password))
	{
		cout << "登陆成功" << endl;
		jingani_admit admit(con);
		string choose;
		cout << "输入exit退出，输入command进入上帝模式,否则重新输入" << endl;
		getline(cin, choose);
		while (choose != "exit")
		{
			if (choose == "command")
			{
				cout << "您可以输入任何合法的sql语句" << endl
					<<"注意，如果删除数据库或其中的表格，系统将无法正常运行，而您也要承担责任" << endl;
				admit.command();
			}
			cout << "服务完成，请选择新的服务" << endl;
			getline(cin, choose);
		}
	}
	else
	{
		cout << "登陆失败" << endl;
	}

	return;
}

void work_painter(MYSQL* con)
{
	string name;
	string password;
	cout << "请输入姓名" << endl;
	getline(cin, name);
	cout << "请输入密码" << endl;
	getline(cin, password);
	if (check_painter(con, name, password))
	{
		cout << "登陆成功" << endl;
		menu_painter();
		jingani_painter painter(con,name);
		string job;
		getline(cin, job);
		while (job != "exit")
		{
			if (job == "mode check")
			{
				cout << "请选择查询模式" << endl;
				string mode;
				getline(cin, mode);
				if (mode == "all" or mode == "ing" or mode == "wait" or mode == "false" or mode=="pass")
				{
					cout << "模式正确，开始查询" << endl;
					painter.mode_check(mode);
				}
			}
	        else if (job == "find editors")
			{
				string mode;
				cout << "请选择查找模式"<<endl;
				getline(cin, mode);
				if (mode == "all" or mode == "free" or mode == "reserve")
				{
					painter.select_edit(mode);
				}
			}
			else if (job == "delivery")
			{
				string editor;
				string num;
				cout << "请输入编辑"<<endl;
				getline(cin, editor);
				cout << "请输入原画编号" << endl;
				getline(cin, num);
				if (painter.check_editor(editor, "free") and painter.check_num(num))
				{
					cout << "请输入原画来源" << endl;
					string source;
					getline(cin, source);
					painter.add_check(editor, num, source, "ing");
					painter.change_editor(editor, "delivery");
				}
				else
				{
					cout << "输入错误，无法插入" << endl;
				}
			}
			else if (job == "reserve")
			{
				string editor;
				string num;
				cout << "请输入编辑" << endl;
				getline(cin, editor);
				cout << "请输入原画编号" << endl;
				getline(cin, num);
				if (painter.check_editor(editor, "reserve") and painter.check_num(num))
				{
					cout << "请输入原画来源" << endl;
					string source;
					getline(cin, source);
					painter.add_check(editor, num, source, "wait");
					painter.change_editor(editor, "reserve");
				}
				else
				{
					cout << "输入错误，无法插入" << endl;
				}
			}
			else if (job == "re_delivery")
			{
				string num;
				cout << "请输入原画编号" << endl;
				getline(cin, num);
				painter.re_check(num);
			}
			else if (job == "my paint instorge")
			{
				painter.paint_in_store();
			}
			cout << "服务完成，请选择新的服务" << endl;
			getline(cin, job);
		}
	}
	else
	{
		cout << "登陆失败" << endl;
	}
	return;
}

void work_editor(MYSQL* con)
{
	string name;
	string password;
	cout << "请输入姓名" << endl;
	getline(cin, name);
	cout << "请输入密码" << endl;
	getline(cin, password);
	if (check_editor(con, name, password))
	{
		cout << "登陆成功" << endl;
		menu_editor();
		jingani_editor editor(con, name);
		string job;
		getline(cin, job);
		while (job != "exit")
		{
			if (job == "check task")
			{
				cout << "请选择模式" << endl;
				string status;
				getline(cin, status);
				if (status == "all" or status == "ing" or status == "wait")
				{
					editor.check_task(status);
				}
			}
			else if (job == "take")
			{
				if (!editor.can_take())
				{
					cout << "无法操作" << endl;
				}
				else
				{
					cout << "可接受的预约原画" << endl;
					editor.check_task("wait");
					cout << "请选择原画编号" << endl;
					string num;
					getline(cin, num);
					if (editor.check_num(num, "wait"))
					{
						editor.take_task(num);
					}
				}
			}
			else if (job == "check")
			{
				cout << "待审核原画信息：" << endl;
				editor.check_task("ing");

				cout << "请选择原画编号" << endl;
				string num;
				getline(cin, num);
				if (editor.check_num(num, "ing"))
				{
					cout << "请评判原画是否合格" << endl;
					string canpass;
					getline(cin, canpass);
					if (canpass == "false")
					{
						editor.check(num, canpass);
					}
					else if (canpass == "pass")
					{
						cout << "请打分" << endl;
						string score;
						getline(cin, score);
						mytime now;
						string datetime = now.get_datatime();
						editor.give_score(num,score,datetime);
						editor.check(num, canpass);
						//editor.ready_instore(num);
					}
				}
			}
			else if (job == "storage")
			{
				cout << "待入库原画信息：" << endl;
				editor.check_task("storage");
				cout << "请选择模式" << endl;
				string mode;
				getline(cin, mode);
				if (mode == "all")
				{
					mytime now;
					string datetime = now.get_datatime();
					editor.to_instore_all(datetime);
				}
				else if (mode == "one")
				{
					cout << "请选择原画编号" << endl;
					string num;
					getline(cin, num);
					if (editor.check_num(num, "storage"))
					{
						mytime now;
						string datetime = now.get_datatime();

						editor.insert_storage(num, datetime);   //增加入库记录
						editor.to_instore(num, datetime);       //增加新原画信息
						editor.ready_instore(num);              //已经入库了
					}
				}
			}
			cout << "服务完成，请选择新的服务" << endl;
			getline(cin, job);
		}
	}
}

void work_tv_station(MYSQL* con)
{
	string name;
	string password;
	cout << "请输入电视台名" << endl;
	getline(cin, name);
	cout << "请输入密码" << endl;
	getline(cin, password);
	if (check_tv_station(con, name, password))
	{
		cout << "登陆成功" << endl;
		menu_tv_station();
		jingani_tv_station tv(con, name);
		string job;
		getline(cin, job);
		while (job != "exit")
		{
			if (job == "check list")
			{
				tv.check_list_all();
			}
			else if (job == "check anime")
			{
				string anime;
				cout << "请输入要查找的番名" << endl;
				getline(cin, anime);
				tv.check_anime(anime);
			}
			else if (job == "add list")
			{
				string anime, source;
				int epi;
				cout << "请输入番名" << endl;
				getline(cin, anime);
				cout << "请输入来源" << endl;
				getline(cin, source);
				cout << "请输入集数" << endl;
				cin >> epi;
				int year, month, day, hour, minute, second;
				cout << "请输入播放开始时间" << endl;
				cout << "请输入年份" << endl;
				cin >> year;
				cout << "请输入月份" << endl;
				cin >> month;
				cout << "请输入日" << endl;
				cin >> day;
				cout << "请输入小时" << endl;
				cin >> hour;
				cout << "请输入分钟" << endl;
				cin >> minute;
				cout << "请输入秒" << endl;
				cin >> second;
				mytime st(year, month, day, hour, minute, second);
				cout << "请输入播放结束时间" << endl;
				cout << "请输入年份" << endl;
				cin >> year;
				cout << "请输入月份" << endl;
				cin >> month;
				cout << "请输入日" << endl;
				cin >> day;
				cout << "请输入小时" << endl;
				cin >> hour;
				cout << "请输入分钟" << endl;
				cin >> minute;
				cout << "请输入秒" << endl;
				cin >> second;
				mytime et(year, month, day, hour, minute, second);
				if (st > et)
				{
					cout << "出错，结束放映时间比开始放映时间早" << endl;
				}
				else
				{
					tv.add_a_list(anime, source, epi, st.get_tm(), et.get_tm());
				}
			}
			else if (job == "change list")
			{
				string anime, source;
				int epi;
				cout << "请输入番名" << endl;
				getline(cin, anime);
				cout << "请输入来源" << endl;
				getline(cin, source);
				cout << "请输入集数" << endl;
				cin >> epi;
				int year, month, day, hour, minute, second;
				cout << "请输入调整后的播放开始时间" << endl;
				cout << "请输入年份" << endl;
				cin >> year;
				cout << "请输入月份" << endl;
				cin >> month;
				cout << "请输入日" << endl;
				cin >> day;
				cout << "请输入小时" << endl;
				cin >> hour;
				cout << "请输入分钟" << endl;
				cin >> minute;
				cout << "请输入秒" << endl;
				cin >> second;
				mytime st(year, month, day, hour, minute, second);
				cout << "请输入调整后的播放结束时间" << endl;
				cout << "请输入年份" << endl;
				cin >> year;
				cout << "请输入月份" << endl;
				cin >> month;
				cout << "请输入日" << endl;
				cin >> day;
				cout << "请输入小时" << endl;
				cin >> hour;
				cout << "请输入分钟" << endl;
				cin >> minute;
				cout << "请输入秒" << endl;
				cin >> second;
				mytime et(year, month, day, hour, minute, second);
				if (st > et)
				{
					cout << "出错，结束放映时间比开始放映时间早" << endl;
				}
				else
				{
					tv.change_time(anime, source, epi, st.get_tm(), et.get_tm());
				}
			}
			else if (job == "put off list" or job == "advance list")
			{
			    string anime, source;
			    int epi;
			    cout << "请输入番名" << endl;
			    getline(cin, anime);
			    cout << "请输入来源" << endl;
			    getline(cin, source);
			    cout << "请输入集数" << endl;
			    cin >> epi;
				string start_time = tv.get_start_time(anime, source, epi);
				string end_time = tv.get_end_time(anime, source, epi);
				mytime st(start_time);
				mytime et(end_time);
				if (job == "put off list")
				{
					cout << "请输入要推迟的时间" << endl;
				}
				else
				{
					cout << "请输入要提前的时间" << endl;
				}
			    int year, month, day, hour, minute, second;
				cout << "请输入年份" << endl;
				cin >> year;
				cout << "请输入月份" << endl;
				cin >> month;
				cout << "请输入日" << endl;
				cin >> day;
				cout << "请输入小时" << endl;
				cin >> hour;
				cout << "请输入分钟" << endl;
				cin >> minute;
				cout << "请输入秒" << endl;
				cin >> second;
				if (job == "advance list")
				{
					if (st.advance_time(year, month, day, hour, minute, second) and et.advance_time(year, month, day, hour, minute, second))
					{
						tv.change_time(anime, source, epi, st.get_tm(), et.get_tm());
					}
					else
					{
						cout << "提前时间错误" << endl;
					}
				}
				else
				{
					if (st.putoff_time(year, month, day, hour, minute, second) and et.putoff_time(year, month, day, hour, minute, second))
					{
						tv.change_time(anime, source, epi, st.get_tm(), et.get_tm());
					}
					else
					{
						cout << "推迟时间错误" << endl;
					}
				}
            }
			else if (job == "check with time")
			{
			    cout << "注意，以播放开始时间为准";
			    int year, month, day, hour, minute, second;
				cout << "请输入查询起始时间：" << endl;
				cout << "请输入年份" << endl;
				cin >> year;
				cout << "请输入月份" << endl;
				cin >> month;
				cout << "请输入日" << endl;
				cin >> day;
				cout << "请输入小时" << endl;
				cin >> hour;
				cout << "请输入分钟" << endl;
				cin >> minute;
				cout << "请输入秒" << endl;
				cin >> second;
				mytime st(year, month, day, hour, minute, second);
				cout << "请输入查询结束时间" << endl;
				cout << "请输入年份" << endl;
				cin >> year;
				cout << "请输入月份" << endl;
				cin >> month;
				cout << "请输入日" << endl;
				cin >> day;
				cout << "请输入小时" << endl;
				cin >> hour;
				cout << "请输入分钟" << endl;
				cin >> minute;
				cout << "请输入秒" << endl;
				cin >> second;
				mytime et(year, month, day, hour, minute, second);
				if (st > et)
				{
					cout << "出错，结束时间比开始时间早" << endl;
				}
				else
				{
					tv.check_time(st.get_tm(), et.get_tm());
				}
            }
			cout << "服务完成，请选择新的服务" << endl;
			getline(cin, job);
		}
	}
}

void work_test(MYSQL* con)
{
	cout << "在这里，你可以尝试所有sql语句" << endl
		<< "注意，如果删除数据库或其中的表格，系统将无法正常运行，而您也要承担责任" << endl;
	cout << "请输入测试语句，输入exit退出" << endl;
	string test;
	getline(cin, test);
	while (test != "exit")
	{
		size_t rt = mysql_real_query(con, test.data(), test.size());
		if (!rt)
		{
			cout << "查询表达式正确" << endl;
		}
		else
		{
			cout << "查询表达式错误" << endl;
		}
		cout << "服务完成，请选择新的服务" << endl;
		getline(cin, test);
	}
}

void work_register(MYSQL* con)
{
	cout << "请输入您的身份" << endl;
	string id;
	getline(cin, id);
	while (1)
	{
		if (id == "painter")
		{
			cout << "请输入您的姓名" << endl;
			string name;
			getline(cin, name);
			while (name.size() > 20)
			{
				cout << "姓名过长，请重新输入" << endl;
				getline(cin, name);
			}
			jingani_painter painter(con,name);
			if (!painter.regist())
			{
				cout << "注册失败" << endl;
			}
			else
			{
				cout << "注册成功" << endl;
			}
		}
		else if (id == "editor")
		{
			cout << "请输入您的姓名" << endl;
			string name;
			getline(cin, name);
			while (name.size() > 20)
			{
				cout << "姓名过长，请重新输入" << endl;
				getline(cin, name);
			}
			jingani_editor editor(con, name);
			if (!editor.regist())
			{
				cout << "注册失败" << endl;
			}
			else
			{
				cout << "注册成功" << endl;
			}
		}
		else if (id == "tv_station")
		{
			cout << "请输入电视台名" << endl;
			string name;
			getline(cin, name);
			while (name.size() > 50)
			{
				cout << "姓名过长，请重新输入" << endl;
				getline(cin, name);
			}
			jingani_tv_station tv(con, name);
			if (!tv.regist())
			{
				cout << "注册失败" << endl;
			}
			else
			{
				cout << "注册成功" << endl;
			}
		}
		else if (id == "exit")
		{
			break;
		}
		else
		{
			cout << "请重新输入您的身份" << endl;
			getline(cin, id);
			continue;
		}
		break;
	}
}