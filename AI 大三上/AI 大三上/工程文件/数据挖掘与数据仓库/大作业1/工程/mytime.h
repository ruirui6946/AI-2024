#include<iostream>
#include<string>
#include<time.h>

#include<cstdio>
#pragma once

using namespace std;

/*struct tm {
	int tm_sec;   // 秒，正常范围从 0 到 59，但允许至 61
	int tm_min;   // 分，范围从 0 到 59
	int tm_hour;  // 小时，范围从 0 到 23
	int tm_mday;  // 一月中的第几天，范围从 1 到 31
	int tm_mon;   // 月，范围从 0 到 11
	int tm_year;  // 自 1900 年起的年数
	int tm_wday;  // 一周中的第几天，范围从 0 到 6，从星期日算起
	int tm_yday;  // 一年中的第几天，范围从 0 到 365，从 1 月 1 日算起
	int tm_isdst; // 夏令时
}*/

/*struct easytime
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
};*/

class mytime
{
private:
	time_t now;
	tm local;
public:
	//构造函数
	mytime() {};
	mytime(string datetime);
	mytime(int year, int month, int day, int hour, int minute, int second);
	
	//时间设置函数
	void set_time(string datetime);
	void set_time(int year, int month, int day, int hour, int minute, int second);
	
	//获得值函数
	string get_datatime();   //获取当前时间（以datetimr格式返回）
	string get_tm();         //获取类中存储时间（以datetimr格式返回）
	int get_day();           //获取本月天数
	int get_last_day();      //获取上个月天数

	//时间平移函数
	bool putoff_time(int year, int month, int day, int hour, int minute, int second);   //时间推移
	bool advance_time(int year, int month, int day, int hour, int minute, int second);  //时间提前
	
	//友元函数，时间比较
	friend bool operator>(const mytime a, const mytime b);
};

bool operator>(const mytime a, const mytime b)
{
	if (a.local.tm_year > b.local.tm_year)
	{
		return true;
	}
	else
	{
		return false;
	}
	if (a.local.tm_mon > b.local.tm_mon)
	{
		return true;
	}
	else
	{
		return false;
	}
	if (a.local.tm_mday > b.local.tm_mday)
	{
		return true;
	}
	else
	{
		return false;
	}
	if (a.local.tm_hour > b.local.tm_hour)
	{
		return true;
	}
	else
	{
		return false;
	}
	if (a.local.tm_min > b.local.tm_min)
	{
		return true;
	}
	else
	{
		return false;
	}
	if (a.local.tm_sec > b.local.tm_sec)
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

bool isleapyear(int year);

mytime::mytime(string datetime)
{
	//wait
	sscanf_s(datetime.data(), "%d-%d-%d %d:%d:%d",
		&local.tm_year, &local.tm_mon, &local.tm_mday, &local.tm_hour, &local.tm_min, &local.tm_sec);
	local.tm_year -= 1900;
	local.tm_mon -= 1;
	//我们不需要其他属性
}

mytime::mytime(int year, int month, int day, int hour, int minute, int second)
{
	local.tm_year = year - 1900;
	local.tm_mon = month - 1;
	local.tm_mday = day;
	local.tm_hour = hour;
	local.tm_min = minute;
	local.tm_sec = second;
}

void mytime::set_time(string datetime)
{
	sscanf_s(datetime.data(), "%d-%d-%d %d:%d:%d",
		&local.tm_year, &local.tm_mon, &local.tm_mday, &local.tm_hour, &local.tm_min, &local.tm_sec);
	local.tm_year -= 1900;
	local.tm_mon -= 1;
}

void mytime::set_time(int year, int month, int day, int hour, int minute, int second)
{
	local.tm_year = year - 1900;
	local.tm_mon = month - 1;
	local.tm_mday = day;
	local.tm_hour = hour;
	local.tm_min = minute;
	local.tm_sec = second;
}

int mytime::get_day()
{
	int tmp = local.tm_mon + 1;
	int res;
	switch (tmp)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		res = 31;
		break;
	case 2:
		if (isleapyear(1900 + local.tm_year))
		{
			res = 29;
		}
		else
		{
			res = 28;
		}
		break;
	default:
		res = 30;
	}
	return res;
}
int mytime::get_last_day()
{
	int tmp = local.tm_mon + 1;
	int res;
	switch (tmp)
	{
	case 2:
	case 4:
	case 6:
	case 8:
	case 9:
	case 11:
	case 12:
		res = 31;
		break;
	case 3:
		if (isleapyear(1900 + local.tm_year))
		{
			res = 29;
		}
		else
		{
			res = 28;
		}
		break;
	default:
		res = 30;
	}
	return res;
}

string mytime::get_datatime()
{
	//cout << "now=time(NULL)";
	//now = time(NULL);
	time(&now);
	//cout << "localtime" << endl;
	localtime_s(&local,&now);
	char tmp[50];
	sprintf_s(tmp, "%4d-%02d-%02d %02d:%02d:%02d", 
		1900+local.tm_year, local.tm_mon+1, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);
	string res(tmp);
	//cout << res;
	return res;
}

string mytime::get_tm()
{
	char tmp[50];
	sprintf_s(tmp, "%4d-%02d-%02d %02d:%02d:%02d",
		1900 + local.tm_year, local.tm_mon + 1, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);
	string res(tmp);
	//cout << res;
	return res;
}

bool mytime::putoff_time(int year, int month, int day, int hour, int minute, int second)
{
	if (year < 0 or month < 0 or day < 0 or hour < 0 or minute < 0 or second < 0)
	{
		return false;
	}

	local.tm_sec += second;
	local.tm_min += minute + local.tm_sec / 60;
	local.tm_sec %= 60;
	local.tm_hour += hour + local.tm_min / 60;
	local.tm_min %= 60;
	local.tm_mday += day + local.tm_hour / 24;
	local.tm_hour %= 24;
	//local.tm_mon += month + local.tm_mday / 31;
	local.tm_mon += month;
	local.tm_year += year + local.tm_year / 12;
	local.tm_mon %= 12;
	while (local.tm_mday > get_day())
	{
		local.tm_mday -= get_day();
		local.tm_mon++;
		local.tm_year += local.tm_mon / 12;
		local.tm_mon %= 12;
	}

	return true;
}

bool mytime::advance_time(int year, int month, int day, int hour, int minute, int second)
{
	if (year < 0 or month < 0 or day < 0 or hour < 0 or minute < 0 or second < 0)
	{
		return false;
	}

	local.tm_sec -= second;
	if (local.tm_sec < 0)
	{
		local.tm_min += local.tm_sec / 60 - 1;
		local.tm_sec %= 60;
		if (local.tm_sec != 0)
		{
			local.tm_sec += 60;
		}
	}
	local.tm_min -= minute;
	if (local.tm_min < 0)
	{
		local.tm_hour += local.tm_min / 60 - 1;
		local.tm_min % 60;
		if (local.tm_min != 0)
		{
			local.tm_min += 60;
		}
	}
	local.tm_hour -= hour;
	if (local.tm_hour < 0)
	{
		local.tm_mday += local.tm_hour / 24 - 1;
		local.tm_hour % 24;
		if (local.tm_hour != 0)
		{
			local.tm_hour += 24;
		}
	}
	local.tm_mday -= day;
	local.tm_mon -= month;
	if (local.tm_mon < 0)
	{
		local.tm_year += local.tm_mon / 12 - 1;
		local.tm_mon % 12;
		if (local.tm_mon != 0)
		{
			local.tm_mon += 12;
		}
	}
	local.tm_year -= year;

	if (local.tm_mday > get_day())
	{
		local.tm_mday -= get_day();
		local.tm_mon++;
		local.tm_year += local.tm_mon / 12;
		local.tm_mon %= 12;
	}

	while (local.tm_mday < 0)
	{
		local.tm_mday += get_last_day();
		local.tm_mon--;
		if (local.tm_mon == -1)
		{
			local.tm_mon = 11;
			local.tm_year--;
		}
	}

	return true;
}

bool isleapyear(int year)
{
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
	{
		return true;
	}
	else
	{
		return false;
	}
}