union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
};

void Wait(int sid)
{
    struct sembuf op;
    op.sem_num=0;
    op.sem_op=-1;
    op.sem_flg=0;
    semop(sid,&op,1);
}

void Signal(int sid)
{
    struct sembuf op;
    op.sem_num=0;
    op.sem_op=1;
    op.sem_flg=0;
    semop(sid,&op,1);
}
