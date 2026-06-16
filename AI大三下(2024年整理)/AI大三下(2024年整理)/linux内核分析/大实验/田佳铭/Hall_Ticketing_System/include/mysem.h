//semun声明
union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
};

//信号量Wait函数
void Wait(int sid)
{
    struct sembuf op;
    op.sem_num=0;
    op.sem_op=-1;
    op.sem_flg=0;
    semop(sid,&op,1);
}

//信号量Signal函数
void Signal(int sid)
{
    struct sembuf op;
    op.sem_num=0;
    op.sem_op=1;
    op.sem_flg=0;
    semop(sid,&op,1);
}
