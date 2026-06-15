#include"jingani_admit.h"

void jingani_admit::print_res()
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
		}
		cout << endl;
	}
}

void jingani_admit::command()
{
	string tmp;
	cout << "请输入命令" << endl;
	getline(cin, tmp);
	size_t rt = mysql_real_query(con, tmp.data(), tmp.size());
	if (!rt)
	{
		cout << "操作成功" << endl;
		if (tmp.substr(0, 6) == "select")
		{
			res = mysql_store_result(con);
			this->print_res();
		}
	}
	else
	{
		cout << "表达式错误" << endl;
	}
}

bool check_admit(string table, MYSQL* con, string account, string password)
{
	string select = "select * from " + table;
	string where = " where Account='" + account + "' and password='" + password + "'";
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