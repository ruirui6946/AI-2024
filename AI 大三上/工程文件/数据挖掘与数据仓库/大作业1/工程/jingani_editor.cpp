#include"jingani_editor.h"

void jingani_editor::print_res()
{
	while (row = mysql_fetch_row(res))
	{
		for (int t = 0; t < mysql_num_fields(res); t++)
		{
			//printf("%s  ", row[t]);
			if (row[t] == NULL)
			{
				cout << " ";
			}
			else
			{
				cout << row[t] << " ";
			}
		}
		cout << endl;
	}
}

void jingani_editor::check_task(string status)
{
	string attr = "ori_p_num,Source,Painter_name,Status";
	string select = "select " + attr + " from jingani.check";
	string where1 = " where Edit_name='" + name + "'";
	string where2 = "";
	if (status == "ing")
	{
		where2 = " and Status='ing'";
	}
	else if (status == "wait")
	{
		where2 = " and Status='wait'";
	}
	else if (status == "storage")
	{
		where2 = " and Status='pass' and instorige=0";
	}
	string tmp = select + where1 + where2;
	//cout << tmp << endl;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		cout << "查询结果：" << endl;
		cout << "ori_p_num,Source,Painter_name,Status:" << endl;
		this->print_res();
	}
	else
	{
		cout << "查询表达式错误" << endl;
	}
}

bool jingani_editor::can_take()
{
	string select = "select * from editor";
	string where1 = " where Name='" + name + "'";
	string where2 = " and now_edit<max_edit";
	string tmp = select + where1 + where2;
	//cout << tmp << endl;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		if (row = mysql_fetch_row(res))
		{
			return true;
		}
		cout << "你的审核数达到上限" << endl;
	}
	else
	{
		cout << "查询表达式错误" << endl;
		return false;
	}
}

bool jingani_editor::check_num(string num, string status)
{
	string select = "select * from jingani.check";
	string where1 = " where ori_p_num='" + num + "'";
	string where2 = "";
	if (status == "wait")
	{
		where2 = " and Status='wait'";
	}
	else if (status == "ing")
	{
		where2 = " and Status='ing'";
	}
	else if (status == "storage")
	{
		where2 = " and Status='pass' and instorige=0";
	}
	string tmp = select + where1 + where2;
	//cout << tmp << endl;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		if (row = mysql_fetch_row(res))
		{
			return true;
		}
		cout << "不可操作的原画编号" << endl;
		return false;
	}
	else
	{
		cout << "查询表达式错误" << endl;
		//printf("Error making query: %s !!!\n", mysql_error(con));
		return false;
	}
	return true;
}

void jingani_editor::take_task(string num)
{
	string update = "update jingani.check";
	string set = " set Status='ing'";
	string where = " where ori_p_num='" + num + "'";
	string tmp = update + set + where;
	to_update(tmp);

	update = "update editor";
	set = " set now_edit=now_edit+1";
	where = " where Name='" + name + "'";
	tmp = update + set + where;
	to_update(tmp);

	set = " set queue_edit=queue_edit+1";
	tmp = update + set + where;
	to_update(tmp);
}

bool jingani_editor::to_update(string tmp)
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

void jingani_editor::check(string num, string status)
{
	string update = "update editor";
	string set = " set now_edit=now_edit-1";
	string where = " where Name='" + name + "'";
	string tmp = update + set + where;
	to_update(tmp);

	update = "update jingani.check";
	set = " set Status='" + status + "'";
	where = " where ori_p_num='" + num + "'";
	tmp = update + set + where;
	to_update(tmp);
}

void jingani_editor::give_score(string num, string score, string datetime)
{
	string update = "update jingani.check";
	string set = " set Quality='" + score + "'";
	string where = " where ori_p_num='" + num + "'";
	string tmp = update + set + where;
	to_update(tmp);

	set = " set pass_time='" + datetime + "'";
	tmp = update + set + where;
	to_update(tmp);
}

void jingani_editor::ready_instore(string num)
{
	string update = "update jingani.check";
	string set = " set instorige=1";
	string where = " where ori_p_num='" + num + "'";
	string tmp = update + set + where;
	//cout << "3.check:" << tmp << endl;
	to_update(tmp);
}

void jingani_editor::to_instore(string num, string datetime)
{
	string select = "select Painter_name,Source,Quality from jingani.check";
	string where = " where ori_p_num='" + num + "';";
	string tmp = select + where;

	string painter_name;
	string quality;
	string source;

	//cout << tmp << endl;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		row = mysql_fetch_row(res);
		painter_name = row[0];
		source = row[1];
		quality = row[2];
		//cout << painter_name << " " << source << " " << quality << endl;
		//insert_storage(num, datetime);
		insert_ori_paint(num, source, painter_name, quality);
	}
	else
	{
		cout << "查询表达式错误" << endl;
	}
}

void jingani_editor::insert_ori_paint(string num, string source, string painter_name, string quality)
{
	string select = "select Style from painter";
	string where = " where Name='" + painter_name + "'";
	string tmp = select + where;
	size_t rt2 = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt2)
	{
		res = mysql_store_result(con);
		row = mysql_fetch_row(res);
		string style = row[0];

		string insert = "insert into jingani.ori_paint";
		string value = " values('" + num + "','" + source + "','" + painter_name + "','" + style + "','" + quality + "');";
		string tmp2 = insert + value;
		//cout << "1.ori_paint:" << tmp2 << endl;
		size_t rt3 = mysql_real_query(con, tmp2.data(), tmp2.size());
		if (!rt3)
		{
			cout << "编号" << num << "的原画入库成功" << endl;
		}
		else
		{
			cout << "查询表达式错误" << endl;
		}
		//insert_storage(num, datetime);
	}
	else
	{
		cout << "查询表达式错误" << endl;
	}
}

void jingani_editor::insert_storage(string num, string datetime)
{
	string insert = "insert into jingani.storage";
	string value = " values('" + num + "','" + name + "','" + datetime + "')";
	string tmp = insert + value;
	//cout << "2.storage:" << tmp << endl;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		cout << "入库记录成功" << endl;
	}
	else
	{
		cout << "表达式错误" << endl;
	}
}

void jingani_editor::to_instore_all(string datetime)
{
	string select = "select ori_p_num from jingani.check";
	string where = " where Edit_name='" + name + "'";
	string tmp = select + where;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		vector<string> num_set;
		int num = 0;
		while (row = mysql_fetch_row(res))
		{
			num++;
			num_set.push_back(row[0]);
			cout << num << ":" << row[0] << endl;
		}
		cout << "num_all:" << num << endl;
		for (int i = 0; i < num; i++)
		{
			string list = num_set[i];
			insert_storage(list, datetime);   //增加入库记录
			to_instore(list, datetime);       //增加新原画信息
			ready_instore(list);              //已经入库了
		}
	}
	else
	{
		cout << "表达式错误" << endl;
	}
}

bool jingani_editor::regist()
{
	if (name_exist())
	{
		cout << "已存在的姓名" << endl;
		return false;
	}
	string password, sex;
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
	while (password != passcheck)
	{
		cout << "密码不一致，请重新输入验证密码" << endl;
		getline(cin, passcheck);
	}

	cout << "请输入性别" << endl;
	getline(cin, sex);
	while (sex != "m" and sex != "f")
	{
		cout << "性别不合法，请重新输入性别，我们只有f和m" << endl;
		getline(cin, sex);
	}

	string insert = "insert into jingani.editor";
	string value = " values('" + name + "','" + sex + "',1,0,0,1,'" + password + "')";
	string tmp = insert + value;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (rt)
	{
		cout << "查询表达式错误" << endl;
		return false;
	}
	return true;
}

bool jingani_editor::name_exist()
{
	string select = "select * from jingani.editor";
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

bool check_editor(MYSQL* con, string name, string password)
{
	string select = "select * from editor";
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