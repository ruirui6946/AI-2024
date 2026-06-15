#include<iostream>
#include<string>
#include"mysql.h"
#include"work.h"

#pragma comment(lib,"libmysql.lib")
using namespace std;

int main()
{
	string dbuser = "root";
	string dbpasswd = "111455aa"; 
	string dbip = "localhost";
	string database = "jingani";
	//上述信息需要修改

	MYSQL *con = mysql_init((MYSQL*) 0);
	if (con != NULL && mysql_real_connect(con, dbip.data(), dbuser.data(), dbpasswd.data(), database.data(), 3306, NULL, 0))
	{
		if (!mysql_select_db(con, database.data()))
		{
			cout << "成功连接数据库,系统可以开始使用"<<endl;
			con->reconnect = 1;
		}
		menu_begin();
		cout << "您是否要注册？" << endl;
		cout << "输入yes开始注册，否则开始登陆" << endl;
		string willgis;
		getline(cin, willgis);
		if (willgis == "yes")
		{
			work_register(con);
			cout << "是否现在登录？" << endl;
			string getin;
			getline(cin, getin);
			cout << "输入yes开始登陆，否则退出系统" << endl;
			if (getin != "yes")
			{
				system("pause");
				return 0;
			}
		}

		menu_in();
		string choose;
		getline(cin, choose);
		while (choose != "exit")
		{
			if (choose == "admit")
			{
				string table = "admit";
				work_admit(table,con);
			}
			else if (choose == "painter")
			{
				work_painter(con);
			}
			else if (choose == "editor")
			{
				work_editor(con);
			}
			else if (choose == "tv station")
			{
				work_tv_station(con);
			}
			else if (choose == "test")
			{
				work_test(con);
			}
			cout << "注销成功，是否以其他身份登录？" << endl;
			getline(cin, choose);
		}
	}

	system("pause");
	return 0;
}
