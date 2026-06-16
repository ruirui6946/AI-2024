//#include"Aseat.h"
/**string转char数组函数**/
void str2char(char *a,string b)
{
    int i;
    int len=b.length();
    for(i=0;i<len;i++)
    {
        a[i]=b[i];
    }
    a[len]='\0';
}

Aseat::Aseat()
{
    //host=nohost;
    //strcpy(host,nohost);
    str2char(host,nohost);
    state=sfree;    //开始是free状态
    left_time=0;
}

bool Aseat::host_set(string name)
{
    //host=name;
    str2char(host,name);
    return true;
}

string Aseat::host_get()
{
    string str=host;
    return str;
}

int Aseat::state_get()
{
    return state;
}

int Aseat::left_get()
{
    return left_time;
}

bool Aseat::canchange()
{
    if(state==sfree)
    {
        return true;
    }
    return false;
}

bool Aseat::canbuy(string name)
{
    string shost=host;
    //可购买的情况：座位不被占用或者为本人预定
    if(canchange() or (state==ordered and shost==name))
    {
        return true;
    }
    return false;
}

bool Aseat::timepass(int time)
{
    if(left_time>time)
    {
        left_time-=time;
    }
    else
    {
        left_time=0;
        if(state==ordered)//预定自动取消
        {
            state=sfree;
            //host=nohost;
            //strcpy(host,nohost);
            str2char(host,nohost);
        }
    }
    return true;
}

bool Aseat::order(int time,string name)
{
    if(!canchange())
    {
        cout<<"This seat is not free"<<endl;
        return false;
    }
    left_time=time;
    state=ordered;
    //host=name;
    str2char(host,name);
    return true;
}

bool Aseat::deorder(string name)
{
    string shost=host;
    if(shost!=name)
    {
        //cout<<"You are not the host of this seat"<<endl;
        return false;
    }
    if(state!=ordered)
    {
        //cout<<"This seat is not ordered, it can't be cancle"<<endl;
        return false;
    }
    //host=nohost;
    str2char(host,nohost);
    state=sfree;
    left_time=0;
    return true;
}

bool Aseat::buy(string name)
{
    /*if(!canchange())
    {
        cout<<"This seat is not free"<<endl;
        return false;
    }*/
    string shost=host;
    if(state==sold)
    {
        //cout<<"sold"<<endl;
        return false;
    }
    if(state==ordered and shost!=name)
    {
        //cout<<"ordered by others"<<endl;
        return false;
    }
    //host=name;
    str2char(host,name);
    state=sold;
    left_time=0;
    return true;
}

void Aseat::print_seat()
{
    if(state==sfree)
    {
        cout<<"free";
    }
    else if(state==sold)
    {
        cout<<"sold,host:"<<host;
    }
    else
    {
        cout<<"ordered,host:"<<host<<",left time:"<<left_time<<"s";
    }
}

//测试时使用
/*int main()
{
    Aseat a;
    a.print_seat();
    cout<<endl;
    a.order(3,"com");
    a.print_seat();
    cout<<endl;
    a.timepass(5);
    a.print_seat();
    cout<<endl;
    a.buy("cc");
    a.print_seat();
    cout<<endl;
    a.buy("DD");
    return 0;
}*/
