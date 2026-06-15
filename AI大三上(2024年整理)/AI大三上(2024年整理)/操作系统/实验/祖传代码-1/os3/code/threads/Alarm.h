#ifndef ALARM_H
#define ALARM_H
#include "system.h"
#include "list.h"

void check(int which);
class Alarm
{
public:
    Alarm();
    ~Alarm();
    void Pause(int howLong);
    List *queue;
    // Timer *timer;
    int waiters;
    void Wakeup();
    static void new_instance();
    static Alarm *instance;
    static void Continue(int arg);
};
#endif
