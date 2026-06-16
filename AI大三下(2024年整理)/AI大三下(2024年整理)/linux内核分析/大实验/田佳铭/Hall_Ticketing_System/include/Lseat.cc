//#include"Aseat.h"
#include"Lseat.h"
//#include"Aseat.cc"

Lseat::Lseat(int num)
{
    /*l_seat_set=new Aseat*[num];
    for(int i=0;i<num;i++)
    {
        l_seat_set[i]=new Aseat;
    }*/
    all_num=num;
    free_num=num;
}

/*Lseat::~Lseat()
{
    for(int i=0;i<all_num;i++)
    {
        delete l_seat_set[i];
    }
    delete l_seat_set;
}*/

bool Lseat::line_set(int num)
{
    /*l_seat_set=new Aseat*[num];
    for(int i=0;i<num;i++)
    {
        l_seat_set[i]=new Aseat;
    }*/
    all_num=num;
    free_num=num;
    return true;
}

int Lseat::l_free_get()
{
    return free_num;
}

int Lseat::update_mes()
{
    int num=0;
    for(int i=0;i<all_num;i++)
    {
        //if(l_seat_set[i]->state_get()==sfree)
        if(l_seat[i].state_get()==sfree)
        {
            num++;
        }
    }
    free_num=num;       //更新free_num的值
    return free_num;
}

int Lseat::l_get_order_num(string name)
{
    int num=0;
    for(int i=0;i<all_num;i++)
    {
        //if(l_seat_set[i]->state_get()==ordered and l_seat_set[i]->host_get()==name)
        if(l_seat[i].state_get()==ordered and l_seat[i].host_get()==name)
        {
            num++;
        }
    }
    return num;
}

bool Lseat::l_all_free()
{
    if(all_num==free_num)
    {
        return true;
    }
    return false;
}

bool Lseat::can_buy_l(string name)
{
    //空闲+被本人预定
    if(all_num==free_num+l_get_order_num(name))
    {
        return true;
    }
    return false;
}

bool Lseat::l_timepass(int time)
{
    for(int i=0;i<all_num;i++)
    {
        //l_seat_set[i]->timepass(time);
        l_seat[i].timepass(time);
    }
    update_mes();       //调用update_mes()更新信息
    return true;
}

bool Lseat::order_one(int num,string name,int time)
{
    if(num<=0 or num>all_num)
    {
        cout<<"command out of range"<<endl;
        return false;
    }
    //if(!l_seat_set[num-1]->canchange())
    if(!l_seat[num-1].canchange())
    {
        cout<<"the seat "<<num<<" is occupied"<<endl;
        return false;
    }
    //if(l_seat_set[num-1]->order(time,name))
    if(l_seat[num-1].order(time,name))
    {
        cout<<"The seat "<<num<<" is reserved"<<endl;
        free_num--;
        return true;
    }
    return false;       //其他错误
}

bool Lseat::buy_one(int num,string name)
{
    if(num<=0 or num>all_num)
    {
        cout<<"command out of range"<<endl;
        return false;
    }
    //if(!l_seat_set[num-1]->canbuy(name))
    if(!l_seat[num-1].canbuy(name))
    {
        cout<<"the seat "<<num<<" is occupied"<<endl;
        return false;
    }
    //if(l_seat_set[num-1]->buy(name))
    if(l_seat[num-1].buy(name))
    {
        cout<<"The seat "<<num<<" is ticketed"<<endl;
        update_mes();
        return true;
    }
    return false;
}

bool Lseat::deorder_one(int num,string name)
{
    if(num<=0 or num>all_num)
    {
        cout<<"command out of range"<<endl;
        return false;
    }
    //if(!l_seat_set[num-1]->deorder(name))
    if(!l_seat[num-1].deorder(name))
    {
        cout<<"the seat "<<num<<" is not your order"<<endl;
        return false;
    }
    cout<<"The seat "<<num<<" is canceled"<<endl;
    free_num++;
    return true;
}

bool Lseat::order_set(int snum,int endnum,string name,int time)
{
    if(snum<=0 or endnum>all_num)
    {
        cout<<"command out of range"<<endl;
        return false;
    }
    for(int i=snum-1;i<endnum;i++)
    {
        //if(!l_seat_set[i]->canchange())
        if(!l_seat[i].canchange())
        {
            cout<<"Some of these seat is occupied"<<endl;
            return false;
        }
    }
    for(int i=snum-1;i<endnum;i++)
    {
        //if(!l_seat_set[i]->order(time,name))
        if(!l_seat[i].order(time,name))
        {
            cout<<"system error"<<endl;
            return false;
        }
    }
    cout<<"The seat "<<snum<<" to "<<endnum<<" are reserved"<<endl;
    free_num-=endnum-snum+1;
    return true;
}

bool Lseat::buy_set(int snum,int endnum,string name)
{
    if(snum<=0 or endnum>all_num)
    {
        cout<<"command out of range"<<endl;
        return false;
    }
    for(int i=snum-1;i<endnum;i++)
    {
        //if(!l_seat_set[i]->canbuy(name))
        if(!l_seat[i].canbuy(name))
        {
            cout<<"Some of these seat is occupied"<<endl;
            return false;
        }
    }
    for(int i=snum-1;i<endnum;i++)
    {
        //if(!l_seat_set[i]->buy(name))
        if(!l_seat[i].buy(name))
        {
            cout<<"system error"<<endl;
            return false;
        }
    }
    cout<<"The seat "<<snum<<" to "<<endnum<<" are ticketed"<<endl;
    update_mes();
    return true;
}

bool Lseat::deorder_set(int snum,int endnum,string name)
{
    if(snum<=0 or endnum>all_num)
    {
        cout<<"command out of range"<<endl;
        return false;
    }
    int dnum=0;
    for(int i=snum-1;i<endnum;i++)
    {
        //if(l_seat_set[i]->deorder(name))
        if(l_seat[i].deorder(name))
        {
            cout<<"The seat "<<i+1<<" is canceled"<<endl;
            dnum++;
        }
    }
    if(dnum==0)
    {
        cout<<"Nothing to do"<<endl;
    }
    update_mes();
    return true;
}

bool Lseat::order_line(string name,int time)
{
    for(int i=0;i<all_num;i++)
    {
        //if(!l_seat_set[i]->canchange())
        if(!l_seat[i].canchange())
        {
            cout<<"Some of these seat is occupied"<<endl;
            return false;
        }
    }
    for(int i=0;i<all_num;i++)
    {
        //if(!l_seat_set[i]->order(time,name))
        if(!l_seat[i].order(time,name))
        {
            cout<<"system error"<<endl;
            return false;
        }
    }
    cout<<"The seat in this line are reserved"<<endl;
    free_num=0;
    return true;
}

bool Lseat::buy_line(string name)
{
    for(int i=0;i<all_num;i++)
    {
        //if(!l_seat_set[i]->canbuy(name))
        if(!l_seat[i].canbuy(name))
        {
            cout<<"Some of these seat is occupied"<<endl;
            return false;
        }
    }
    for(int i=0;i<all_num;i++)
    {
        //if(!l_seat_set[i]->buy(name))
        if(!l_seat[i].buy(name))
        {
            cout<<"system error"<<endl;
            return false;
        }
    }
    cout<<"The seat in this line are ticketed"<<endl;
    free_num=0;
    return true;
}

bool Lseat::deorder_line(string name)
{
    int dnum=0;
    for(int i=0;i<all_num;i++)
    {
        //if(l_seat_set[i]->deorder(name))
        if(l_seat[i].deorder(name))
        {
            cout<<"The seat "<<i+1<<" is canceled"<<endl;
            dnum++;
        }
    }
    if(dnum==0)
    {
        cout<<"No seat is canceled"<<endl;
    }
    update_mes();
    return true;
}

bool Lseat::order_some(int num,string name,int time)
{
    if(num>free_num)
    {
        cout<<"No enough seat left"<<endl;
        return false;
    }
    int onum=0;
    for(int i=0;i<all_num;i++)
    {
        //if(l_seat_set[i]->canchange())
        if(l_seat[i].canchange())
        {
            //if(!l_seat_set[i]->order(time,name))
            if(!l_seat[i].order(time,name))
            {
                cout<<"system error"<<endl;
                return false;
            }
            cout<<"The seat "<<i+1<<" is reserved"<<endl;
            free_num--;
            onum++;
        }
        if(onum==num)
        {
            break;
        }
    }
    return true;
}

bool Lseat::buy_some(int num,string name)
{
    if(num>free_num+l_get_order_num(name))
    {
        cout<<"No enough seat left"<<endl;
        return false;
    }
    int onum=0;
    //优先购买已预订的座位
    for(int i=0;i<all_num;i++)
    {
        //if(l_seat_set[i]->canbuy(name))
        if(l_seat[i].canbuy(name) and l_seat[i].state_get()!=sfree)
        {
            //if(!l_seat_set[i]->buy(name))
            if(!l_seat[i].buy(name))
            {
                cout<<"system error"<<endl;
                return false;
            }
            cout<<"The seat "<<i+1<<" is ticketed"<<endl;
            onum++;
        }
        if(onum==num)
        {
            break;
        }
    }
    if(onum==num)
    {
        update_mes();
        return true;
    }
    for(int i=0;i<all_num;i++)
    {
        if(l_seat[i].state_get()==sfree)
        {
            if(!l_seat[i].buy(name))
            {
                cout<<"system error"<<endl;
                return false;
            }
            cout<<"The seat "<<i+1<<" is ticketed"<<endl;
            onum++;
        }
        if(onum==num)
        {
            break;
        }
    }
    update_mes();
    return true;
}

bool Lseat::deorder_some(int num,string name)
{
    int onum=0;
    for(int i=0;i<all_num;i++)
    {
        //if(l_seat_set[i]->deorder(name))
        if(l_seat[i].deorder(name))
        {
            cout<<"The seat "<<i+1<<" is canceled"<<endl;
            free_num++;
            onum++;
        }
        if(onum==num)
        {
            break;
        }
    }
    if(onum==0)
    {
        cout<<"Nothing to do"<<endl;
    }
    return true;
}

void Lseat::print_l()
{
    cout<<"[";
    for(int i=0;i<all_num;i++)
    {
        cout<<"seat"<<i+1<<":[";
        //l_seat_set[i]->print_seat();
        l_seat[i].print_seat();
        cout<<"]";
        if(i!=all_num-1)
        {
            cout<<",";
        }
    }
    cout<<"]"<<endl;
}

void Lseat::print_host_l(string name)
{
    for(int i=0;i<all_num;i++)
    {
        //if(l_seat_set[i]->host_get()==name)
        if(l_seat[i].host_get()==name)
        {
            //if(l_seat_set[i]->state_get()==ordered)
            if(l_seat[i].state_get()==ordered)
            {
                //cout<<"you reserved seat "<<i+1<<",left time:"<<l_seat_set[i]->left_get()<<"s"<<endl;
                cout<<"you reserved seat "<<i+1<<",left time:"<<l_seat[i].left_get()<<"s"<<endl;
            }
            //else if(l_seat_set[i]->state_get()==sold)
            else if(l_seat[i].state_get()==sold)
            {
                cout<<"you ticketed seat "<<i+1<<endl;
            }
        }
    }
}

//测试时使用
/*int main()
{
    Lseat l;
    l.line_set(5);
    l.print_l();
    l.order_line("bb",3);
    l.buy_one(2,"aa");
    l.buy_one(1,"bb");
    l.print_l();
    l.l_timepass(5);
    l.print_l();
    l.order_some(3,"DD",4);
    l.print_l();
    l.deorder_some(2,"DD");
    l.print_l();
    return 0;
}*/
