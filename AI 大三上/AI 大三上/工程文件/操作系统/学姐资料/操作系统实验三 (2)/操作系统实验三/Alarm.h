#ifndef ALARMCLOCK_H_INCLUDED
#define ALARMCLOCK_H_INCLUDED

#include "copyright.h"
#include "list.h"
#include "system.h"

class Alarm
{
	public:
		Alarm();
		~Alarm();
		void Pause(int howLong);//让线程在howLong事件后开始运行
		void Call();//闹钟结束后重新开始运行线程
	private:
		List *pending;//存放睡眠线程的阻塞队列
		int num;//睡眠线程数
};



#endif // ALARMCLOCK_H_INCLUDED
