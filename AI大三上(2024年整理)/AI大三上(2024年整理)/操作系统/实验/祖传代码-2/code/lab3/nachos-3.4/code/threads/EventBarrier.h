# ifndef EVENTBARRIER_H
# define EVENTBARRIER_H
# include "synch.h"
class EventBarrier
{
public:
    EventBarrier(char* debugName);
    ~EventBarrier();
    char* getName(){ return name; }
    void Wait();    // 等待事件发生的信号，若处于有信号状态，直接返回
	void Signal();  // 广播事件发生的信号，并等待所有线程都完成响应
                    // 当Signal()返回时，EventBarrier将恢复为条无信号状态
	void Complete();// 线程完成对事件信号的响应，并等待其他线程都完成响应
	int Waiters();  // 返回正在等待事件或尚未完成响应的线程的数量
private:
    char* name;         
    int waiterNum;      // 正在等待事件或尚未完成响应的线程的数量
    bool signalStatus;  // 有无信号状态
    Lock* lock;
    Condition* signaled;     // 有信号状态的条件变量
    Condition* complete;     // 完成响应的条件变量
    Condition* allCompleted; // 全部完成响应的条件变量
};
# endif
