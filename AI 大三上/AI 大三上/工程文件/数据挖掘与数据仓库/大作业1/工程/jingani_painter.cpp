#include"jingani_painter.h"

void jingani_painter::print_res()
{
	while (row = mysql_fetch_row(res))
	{
		for (int t = 0; t < mysql_num_fields(res); t++)
		{
			//printf("%s  ", row[t]);
			if(row[t]==NULL)
			{
				cout << " ";
			}
			else
			{
				cout << row[t] << " ";
			}
			//cout << row[t] << " ";
		}
		cout << endl;
	}
}

void jingani_painter::mode_check(string mode)
{
	string select = "select * from jingani.check";
	string where1 = " where Painter_name='"+name+"'";
	string where2 = "";
	if (mode != "all")
	{
		where2 = " and Status='" + mode + "'";
	}
	string tmp = select + where1 + where2;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		cout << "查询结果：" << endl;
		cout << "全部信息为（具体信息参考相关文档）" << endl;
		this->print_res();
	}
	else
	{
		cout << "查询表达式错误" << endl;
	}
}

void jingani_painter::paint_in_store()
{
	string select = "select * from jingani.ori_paint";
	string where = " where Painter_name='" + name + "'";
	string tmp = select + where;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		cout << "查询结果：" << endl;
		cout << "全部信息为（具体信息参考相关文档）" << endl;
		this->print_res();
	}
	else
	{
		cout << "查询表达式错误" << endl;
	}
}

void jingani_painter::select_edit(string mode)
{
	string attr = "Name,Sex,max_edit,now_edit,queue_edit,Level";
	string select = "select " + attr + " from editor";
	string where = "";
	if (mode == "free")
	{
		where = " where now_edit<max_edit";
	}
	else if (mode == "reserve")
	{
		where = " where queue_edit>0";
	}
	string tmp = select + where;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		this->print_res();
	}
	else
	{
		cout << "查询表达式错误" << endl;
	}
}

bool jingani_painter::check_editor(string editor, string mode)
{
	string select = "select * from editor";
	string where = "";
	if (mode == "free")
	{
		where = " where Name='" + editor + "' and now_edit<max_edit";
	}
	else if (mode == "reserve")
	{
		where = " where Name='" + editor + "' and queue_edit>0";
	}
	string tmp = select + where;
	//cout << tmp << endl;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		if (row = mysql_fetch_row(res))
		{
			return true;
		}
		cout << "编辑审核数达到上限" << endl;
	}
	else
	{
		cout << "查询表达式错误" << endl;
	}
	return false;
}

bool jingani_painter::check_num(string num)
{
	string select = "select * from jingani.check";
	string where = " where ori_p_num='" + num + "'";
	string tmp = select + where;
	//cout << tmp << endl;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		if (row = mysql_fetch_row(res))
		{
			cout << "存在的原画编号" << endl;
			return false;
		}
	}
	else
	{
		cout << "查询表达式错误" << endl;
		//printf("Error making query: %s !!!\n", mysql_error(con));
		return false;
	}
	return true;
}

void jingani_painter::add_check(string editor, string num, string source, string status)
{
	string insert = "insert into jingani.check (ori_p_num,Edit_name,Source,Painter_name,Status,instorige)";
	string value = " values('" + num + "','" + editor + "','" + source + "','" + name + "','" + status + "',0)";
	string tmp = insert + value;
	//cout << tmp << endl;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		cout << "操作成功" << endl;
	}
	else
	{
		cout << "表达式错误" << endl;
	}
}

bool jingani_painter::to_update(string tmp)
{
	//cout << tmp << endl;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		return true;
	}
	else
	{
		cout << "表达式错误" << endl;
		return false;
	}
}

void jingani_painter::re_check(string num)
{
	string select1 = "select * from jingani.editor";
	string where1 = " where now_edit<max_edit and Name in";
	string select2 = "select Edit_name from jingani.check";
	string where2 = " where ori_p_num='"+num+"' and status='false'";
	string tmp = select1 + where1 + " (" + select2 + where2 + ")";
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	cout << tmp << endl;
	if (!rt)
	{
		res = mysql_store_result(con);
		if (row = mysql_fetch_row(res))
		{
			cout << "当前可重新交付，开始交付" << endl;
		}
		else
		{
			cout << "当前不可重新交付" << endl;
			return;
		}
	}
	else
	{
		cout << "查询表达式错误1" << endl;
		return;
	}

	tmp = select2 + where2;
	string editor;
	rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		row = mysql_fetch_row(res);
		editor = row[0];
	}
	else
	{
		cout << "查询表达式错误2" << endl;
		return;
	}

	change_editor(editor,"delivery");
	string update = "update jingani.check";
	string set = " set Status='ing'";
	string where = " where ori_p_num='"+num+"'";
	tmp = update + set + where;
	to_update(tmp);
}

void jingani_painter::change_editor(string editor, string status)
{
	string update = "update editor";
	string set;
	if (status == "delivery")
	{
		set = " set now_edit=now_edit+1";
	}
	else if (status == "reserve")
	{
		set = " set queue_edit=queue_edit-1";
	}
	else
	{
		cout << "无此操作，错误" << endl;
		return;
	}
	string where = " where Name='" + editor + "'";
	string tmp = update + set + where;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		//cout << "操作成功" << endl;
		if (status == "delivery")
		{
			cout << "交付成功" << endl;
		}
		else
		{
			cout << "预约成功" << endl;
		}
	}
	else
	{
		cout << "表达式错误" << endl;
	}
}

bool jingani_painter::regist()
{
	if (name_exist())
	{
		cout << "已存在的姓名" << endl;
		return false;
	}
	string password, sex, style;
	string passcheck;
	cout << "请输入密码" << endl;
	getline(cin, password);
	while (password.size() > 20)
	{
		cout << "密码过长，请重新输入密码" << endl;
		getline(cin, password);
	}
	cout << "请确认密码" << endl;
	getline(cin, passcheck);
	while (password!=passcheck)
	{
		cout << "密码不一致，请重新输入验证密码" << endl;
		getline(cin, passcheck);
	}

	cout << "请输入性别" << endl;
	getline(cin, sex);
	while (sex!="m" and sex!="f")
	{
		cout << "性别不合法，请重新输入性别，我们只有f和m" << endl;
		getline(cin, sex);
	}

	cout << "请输入作画风格" << endl;
	getline(cin, style);
	while (style.size() > 20)
	{
		cout << "作画风格过长，请重新输入" << endl;
		getline(cin, style);
	}

	string insert = "insert into jingani.painter";
	string value = " values('"+name+"','"+sex+"','"+style+"',1,'"+password+"')";
	string tmp = insert + value;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (rt)
	{
		cout << "查询表达式错误" << endl;
		return false;
	}
	return true;
}

bool jingani_painter::name_exist()
{
	string select = "select * from painter";
	string where = " where Name='" + name + "'";
	string tmp = select + where;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		if (row = mysql_fetch_row(res))
		{
			return true;
		}
		return false;
	}
	else
	{
		cout << "查询表达式错误" << endl;
		return false;
	}
}

bool check_painter(MYSQL* con, string name, string password)
{
	string select = "select * from painter";
	string where = " where Name='" + name + "' and password='" + password + "'";
	string tmp = select + where;
	//cout << tmp << endl;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		MYSQL_RES *res = mysql_store_result(con);
		MYSQL_ROW row;
		if (row = mysql_fetch_row(res))
		{
			return true;
		}
		return false;
	}
	else
	{
		cout << "查询表达式错误" << endl;
	}
	return false;
}