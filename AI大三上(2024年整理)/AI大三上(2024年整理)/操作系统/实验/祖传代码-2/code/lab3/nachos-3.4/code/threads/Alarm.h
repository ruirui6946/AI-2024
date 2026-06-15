# ifndef ALARM_H
# define ALARM_H
# include "list.h"
# include "synch.h"
class Alarm
{
public:
    Alarm();
    ~Alarm();
    void Pause(int howLong); // 睡howLong Tick
    void Restart();          // 唤醒
private:
    int alarmNum;  // 等待苏醒的线程数 
    List* queue;   // 按苏醒时间顺序，插入线程
};
# endif
