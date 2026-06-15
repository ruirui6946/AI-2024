#ifndef BRIDGE_H
#define BRIDGE_H

#include "system.h"
#include "copyright.h"
#include "synch.h"

enum BridgeStatus {BUSYL,BUSYR, FREE};//分别表示桥有左边的车辆过桥，有右边的车辆过桥、处于空闲状态、

class Bridge
{

	public:
        Bridge();
        ~Bridge();
        void ArriveBridge(char direc);//到达桥
        void CrossBridge(char direc);//过桥
        void ExitBridge(char direc);//下桥
    private:
        Lock *lock;
        BridgeStatus status;//桥可有三个状态，分别为FREE,BUSYL,BUSYR
        Condition *BridgeBusyL;
        Condition *BridgeBusyR;
        int count;	//正在过桥的车辆数量
        int valuel;  //等待在桥左边的车辆数量
        int valuer;	 //等待在桥右边的车辆数量
        bool flag;	//flag为true时，如果桥上有车在过桥并且不满三辆时，如果有同方向的车到达，则其可跟着过桥；
                   //flag为false时，如果桥上有车过并且不满三辆，有同方向的车到达时，也需要等待。
                   //这是为了实现两边的车能交替的过，防止了饥饿的产生。
};
//桥的左侧和右侧车辆交替过桥；当同一方向有三辆或者以上的车辆时，则同时让三辆车过桥，
//然后唤醒桥的另一方向上等待的三辆车，若对面没有一辆车，则唤醒本方向的车辆。
//在这三辆车过桥时不允许本方向的新到达的车跟着过桥，必须等到三辆车过完换成另一方向过完一次后才允许。

#endif
