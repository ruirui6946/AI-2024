#include "jingani_tv_station.h"
#include<sstream>

void jingani_tv_station::print_res()
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

bool jingani_tv_station::to_update(string tmp)
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

string jingani_tv_station::get_start_time(string anime, string source, int episode)
{
	stringstream st;
	st << episode;
	string epi = st.str();

	string select = "select start_time from jingani.tv_list";
	string where = " where TV_name='" + name + "' and Anime='" + anime + "' and Source='" + source + "' and Episode='" + epi + "'";
	string tmp = select + where;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		row = mysql_fetch_row(res);
		string start_time = row[0];
		return start_time;
	}
	else
	{
		cout << "查询表达式错误" << endl;
	}
}

string jingani_tv_station::get_end_time(string anime, string source, int episode)
{
	stringstream st;
	st << episode;
	string epi = st.str();

	string select = "select end_time from jingani.tv_list";
	string where = " where TV_name='" + name + "' and Anime='" + anime + "' and Source='" + source + "' and Episode='" + epi + "'";
	string tmp = select + where;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		row = mysql_fetch_row(res);
		string end_time = row[0];
		return end_time;
	}
	else
	{
		cout << "查询表达式错误" << endl;
	}
}

void jingani_tv_station::check_list_all()
{
	string select = "select * from tv_list";
	string where = " where TV_name='"+name+"'";
	string tmp = select + where;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		cout << "查询结果：" << endl;
		cout << "全部信息为（具体信息参考相关文档）" << endl;
		print_res();
	}
	else
	{
		cout << "查询表达式错误" << endl;
	}
}

void jingani_tv_station::check_anime(string anime)
{
	string select = "select * from tv_list";
	string where = " where TV_name='" + name + "' and Anime='"+anime+"'";
	string order = " order by Episode";
	string tmp = select + where + order;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		cout << "查询结果：" << endl;
		cout << "全部信息为（具体信息参考相关文档）" << endl;
		print_res();
	}
	else
	{
		cout << "查询表达式错误" << endl;
	}
}

void jingani_tv_station::check_time(string start_time, string end_time)
{
	string select = "select * from jingani.tv_list";
	string where = " where TV_name='"+name+"' and start_time>'"+start_time+"' and start_time<'"+end_time+"'";
	string order = " order by start_time";
	string tmp = select + where + order;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		cout << "查询结果：" << endl;
		cout << "全部信息为（具体信息参考相关文档）" << endl;
		print_res();
	}
	else
	{
		cout << "查询表达式错误" << endl;
	}
}

bool check_tv_station(MYSQL* con, string name, string password)
{
	string select = "select * from jingani.tv_station";
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

void jingani_tv_station::add_a_list(string anime, string source, int episode, string start_time, string end_time)
{
	string select = "select * from jingani.tv_list";
	string where = " where TV_name='"+name+"' and start_time<'"+start_time+"' and end_time>'"+start_time+"'";
	string tmp = select+where;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		if (row = mysql_fetch_row(res))
		{
			cout << "播放时间冲突" << endl;
			return;
		}
	}
	else
	{
		cout << "查询表达式错误" << endl;
		return;
	}

	where = " where TV_name='" + name + "' and start_time<'" + end_time + "' and end_time>'" + end_time + "'";
	tmp = select + where;
	rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		if (row = mysql_fetch_row(res))
		{
			cout << "播放时间冲突" << endl;
			return;
		}
	}
	else
	{
		cout << "查询表达式错误" << endl;
		return;
	}

	where = " where TV_name='" + name + "' and start_time>'" + start_time + "' and end_time<'" + end_time + "'";
	tmp = select + where;
	rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		if (row = mysql_fetch_row(res))
		{
			cout << "播放时间冲突" << endl;
			return;
		}
	}
	else
	{
		cout << "查询表达式错误" << endl;
		return;
	}

	where = " where TV_name='" + name + "' and start_time<'" + start_time + "' and end_time>'" + end_time + "'";
	tmp = select + where;
	rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		if (row = mysql_fetch_row(res))
		{
			cout << "播放时间冲突" << endl;
			return;
		}
	}
	else
	{
		cout << "查询表达式错误" << endl;
		return;
	}
	
	stringstream st;
	st << episode;
	string epi = st.str();
	string insert = "insert into jingani.tv_list";
	string value = " values('" + name + "','" + anime + "','" + source + "','" + epi + "','" + start_time + "','" + end_time + "')";
	tmp = insert + value;
	cout << tmp << endl;
	rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		cout << "增加播放列表成功" << endl;
	}
	else
	{
		cout << "查询表达式错误" << endl;
	}
}

void jingani_tv_station::change_time(string anime, string source, int episode, string start_time, string end_time)
{
	string select = "select * from jingani.tv_list";
	string where = " where TV_name='" + name + "' and start_time<'" + start_time + "' and end_time>'" + start_time + "'";
	string tmp = select + where;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		if (row = mysql_fetch_row(res))
		{
			cout << "播放时间冲突" << endl;
			return;
		}
	}
	else
	{
		cout << "查询表达式错误" << endl;
		return;
	}

	where = " where TV_name='" + name + "' and start_time<'" + end_time + "' and end_time>'" + end_time + "'";
	tmp = select + where;
	rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		if (row = mysql_fetch_row(res))
		{
			cout << "播放时间冲突" << endl;
			return;
		}
	}
	else
	{
		cout << "查询表达式错误" << endl;
		return;
	}

	where = " where TV_name='" + name + "' and start_time>'" + start_time + "' and end_time<'" + end_time + "'";
	tmp = select + where;
	rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		if (row = mysql_fetch_row(res))
		{
			cout << "播放时间冲突" << endl;
			return;
		}
	}
	else
	{
		cout << "查询表达式错误" << endl;
		return;
	}

	where = " where TV_name='" + name + "' and start_time<'" + start_time + "' and end_time>'" + end_time + "'";
	tmp = select + where;
	rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		res = mysql_store_result(con);
		if (row = mysql_fetch_row(res))
		{
			cout << "播放时间冲突" << endl;
			return;
		}
	}
	else
	{
		cout << "查询表达式错误" << endl;
		return;
	}

	stringstream st;
	st << episode;
	string epi = st.str();

	string update = "update jingani.tv_list";
	string set = " set start_time='"+start_time+"'";
	where = " where TV_name='"+name+"' and Anime='"+anime+"' and Source='"+source+"' and Episode='"+epi+"'";

	tmp = update + set + where;
	if (!to_update(tmp))
	{
		cout << "修改播放时间失败" << endl;
	}

	set= " set end_time='"+end_time+"'";

	tmp = update + set + where;
	if (to_update(tmp))
	{
		cout << "修改播放时间成功" << endl;
	}
}

bool jingani_tv_station::regist()
{
	if (name_exist())
	{
		cout << "已存在的电视台名" << endl;
		return false;
	}
	string password, local;
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

	cout << "请输入所在地" << endl;
	getline(cin, local);
	while (local.size() > 20)
	{
		cout << "所在地过长，请重新输入" << endl;
		getline(cin, local);
	}

	string insert = "insert into jingani.tv_station";
	string value = " values('" + name + "','" + local + "',0,0,0,'" + password + "')";
	string tmp = insert + value;
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (rt)
	{
		cout << "查询表达式错误" << endl;
		return false;
	}
	return true;
}

bool jingani_tv_station::name_exist()
{
	string select = "select * from jingani.tv_station";
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