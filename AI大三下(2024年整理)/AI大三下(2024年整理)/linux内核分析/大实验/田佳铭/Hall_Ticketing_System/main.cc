#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<wait.h>
#include<errno.h>
#include<vector>

#include"./include/mysem.h"
//#include"Hall.h"
#include"./include/Hall.cc"
#include"./include/get_mess.h"
using namespace std;

int main()
{
    //input
    string filename="./input/input.txt";    //输入文件名
    fstream _file;
    _file.open(filename.c_str(),ios::in);
    if(!_file)  //文件不存在的情况
    {
        cout<<"open file error"<<endl;
        _file.close();
        exit(0);
    }
    _file.close();

    int n,m,k,rvt;
    vector<vector<string> > com_set;
    vector<vector<int> > time_list;

    //获取所有信息
    struct in_mess res=get_mess(filename);
    n=res.n;    //座位数
    m=res.m;    //每排座位数
    k=res.k;    //代理数
    rvt=res.rvt;//预约保留时间
    com_set=res.com_set;        //命令集合
    time_list=res.time_list;    //每个代理的时间集合
    if(n>hall_line or m>seat_line_num)  //越界判断
    {
        cout<<"The Hall can't hold these seats"<<endl;
        return 0;
    }

    //creat sharememory and sem
    key_t mym,mysem;
    int shmid;
    int semid;
    int ikey;
    //遍历来寻找合适的键值
    for(ikey=1;ikey<256;ikey++)
    {
        mym=ftok("/tmp",ikey);
        shmid=shmget(mym,sizeof(Hall),0666|IPC_CREAT|IPC_EXCL);
        if(mym!=-1 and shmid!=-1)
        {
            break;
        }
    }
    if(shmid==-1)
    {
        cout<<"creat sharememery error"<<endl;
        return 0;
    }
    for(;ikey<256;ikey++)
    {
        mysem=ftok("/tmp",ikey);
        semid=semget(mysem,1,0666|IPC_CREAT|IPC_EXCL);
        if(mysem!=-1 and semid!=-1)
        {
            break;
        }
    }
    if(semid==-1)
    {
        cout<<"creat sem error"<<endl;
        return 0;
    }
    union semun semun1;
    semun1.val=1;
    semctl(semid,0,SETVAL,semun1);      //信号量初始化
    Hall *p=(Hall*)shmat(shmid,NULL,0);
    p->hall_set(n,m,rvt);               //初始化音乐厅
    shmdt(p);

    cout<<"System is initialized, begin to work"<<endl<<endl;
    //creat k agent
    signal(SIGCHLD,SIG_IGN);            //同步信号
    int agent;
    pid_t pid;
    bool flag=true;
    for(agent=1;agent<=k;agent++)       //创造所有代理
    {
        pid=fork();
        if(pid==0)
        {
            break;
        }
        if(pid<0)
        {
            cout<<"Agent"<<agent<<" creat error"<<endl;
            exit(0);
        }
    }
    if(pid==0)//child
    {
        int com_num=com_set[agent-1].size();
        for(int j=0;j<com_num;j++)
        {
            Wait(semid);
            Hall *pp=(Hall*)shmat(shmid,NULL,0);
            if(pp==(void*)-1)
            {
                cout<<"Agent"<<agent<<" error"<<endl;
            }
            int stime=pp->auto_work(com_set[agent-1][j],time_list[agent-1]);
            if(stime!=0)    //调用成功
            {
                cout<<"It costs agent"<<agent<<": "<<stime<<"s"<<endl;
            }
            cout<<endl;
            sleep(stime);
            shmdt(pp);
            Signal(semid);
        }
        exit(0);
    }
    else
    {
        wait((int*)0);//wait for all child
        Hall *ppp=(Hall*)shmat(shmid,NULL,0);
        if(ppp==(void*)-1)
        {
            cout<<"exit error"<<endl;
            exit(0);
        }
        cout<<"Millsion completed, current sutuation:"<<endl;
        ppp->print_hall();
        shmdt(ppp);
        shmctl(shmid,IPC_RMID,NULL);
        semctl(semid,0,IPC_RMID);
    }
    return 0;
}
