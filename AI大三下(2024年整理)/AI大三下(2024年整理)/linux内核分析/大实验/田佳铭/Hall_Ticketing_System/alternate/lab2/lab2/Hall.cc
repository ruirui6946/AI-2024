#include"Hall.h"

Hall::Hall(int lnum,int llen,int time)
{
    //h_seat_set=new Lseat*[lnum];
    for(int i=0;i<lnum;i++)
    {
        //h_seat_set[i]=new Lseat;
        //h_seat_set[i]->line_set(llen);
        //
        h_seat[i].line_set(llen);
    }
    line_num=lnum;
    line_len=llen;
    free_all=lnum*llen;
    rtime=time;
    //test_tmp=0;
}

bool Hall::hall_set(int lnum,int llen,int time)
{
    //h_seat_set=new Lseat*[lnum];
    for(int i=0;i<lnum;i++)
    {
        //h_seat_set[i]=new Lseat;
        //h_seat_set[i]->line_set(llen);
        //
        h_seat[i].line_set(llen);
    }
    line_num=lnum;
    line_len=llen;
    free_all=lnum*llen;
    rtime=time;
    //test_tmp=0;
}

/*Hall::~Hall()
{
    for(int i=0;i<line_num;i++)
    {
        delete h_seat_set[i];
    }
    delete h_seat_set;
}*/

string Hall::num2letter(int num)
{
    string str="";
    num++;
    while(num!=0)
    {
        int snum=num/26;
        int onum=num%26;
        if(onum==0)
        {
            str+="Z";
            snum--;
        }
        else
        {
            str+=char(onum)-1+'A';
        }
        num=snum;
    }
    reverse(str.begin(),str.end());
    return str;
}

int Hall::letter2num(string letter)
{
    int num=0;
    int len=letter.length();
    for(int i=0;i<len;i++)
    {
        num=num*26+int(letter[i]-'A'+1);
    }
    return num-1;
}

int Hall::free_seat()
{
    int fnum=0;
    for(int i=0;i<line_num;i++)
    {
        //fnum+=h_seat_set[i]->l_free_get();
        fnum+=h_seat[i].l_free_get();
    }
    free_all=fnum;
    return fnum;
}

int Hall::get_order_num(string name)
{
    int fnum=0;
    for(int i=0;i<line_num;i++)
    {
        //fnum+=h_seat_set[i]->l_get_order_num(name);
        fnum+=h_seat[i].l_get_order_num(name);
    }
    //free_all=fnum;
    return fnum;
}

int Hall::update_h_mes()
{
    int fnum=0;
    for(int i=0;i<line_num;i++)
    {
        //fnum+=h_seat_set[i]->update_mes();
        fnum+=h_seat[i].update_mes();
    }
    free_all=fnum;
    return fnum;
}

bool Hall::h_timepass(int time)
{
    for(int i=0;i<line_num;i++)
    {
        //h_seat_set[i]->l_timepass(time);
        h_seat[i].l_timepass(time);
    }
    free_seat();
    return true;
}

bool Hall::order_lset(int sline,int num,string name,int time)
{
    if(sline<=0 or sline+num>line_num)
    {
        cout<<"command out of range"<<endl;
        return false;
    }
    for(int i=0;i<num;i++)
    {
        //if(!h_seat_set[sline+i-1]->l_all_free())
        if(!h_seat[sline+i-1].l_all_free())
        {
            cout<<"Some of these seat is occupied"<<endl;
            return false;
        }
    }
    for(int i=0;i<num;i++)
    {
        cout<<"Line"<<num2letter(sline+i-1)<<": "<<endl;
        //h_seat_set[sline+i-1]->order_line(name,time);
        h_seat[sline+i-1].order_line(name,time);
    }
    //free_seat();
    free_all-=num*line_len;
    return true;
}

bool Hall::buy_lset(int sline,int num,string name)
{
    if(sline<=0 or sline+num>line_num)
    {
        cout<<"command out of range"<<endl;
        return false;
    }
    for(int i=0;i<num;i++)
    {
        //if(!h_seat_set[sline+i-1]->can_buy_l(name))
        if(!h_seat[sline+i-1].can_buy_l(name))
        {
            cout<<"Some of these seat is occupied"<<endl;
            return false;
        }
    }
    for(int i=0;i<num;i++)
    {
        cout<<"Line"<<num2letter(sline+i-1)<<": "<<endl;
        //h_seat_set[sline+i-1]->buy_line(name);
        h_seat[sline+i-1].buy_line(name);
    }
    free_seat();
    return true;
}

bool Hall::deorder_lset(int sline,int num,string name)
{
    if(sline<=0 or sline+num>line_num)
    {
        cout<<"command out of range"<<endl;
        return false;
    }
    for(int i=0;i<num;i++)
    {
        cout<<"Line"<<num2letter(sline+i-1)<<": "<<endl;
        //h_seat_set[sline+i-1]->deorder_line(name);
        h_seat[sline+i-1].deorder_line(name);
    }
    free_seat();
    return true;
}

bool Hall::order_ran(int num,string name,int time)
{
    if(num>free_all)
    {
        cout<<"No enough seat left"<<endl;
        return false;
    }
    int onum=num;
    for(int i=0;i<line_num;i++)
    {
        //int free_l=h_seat_set[i]->l_free_get();
        int free_l=h_seat[i].l_free_get();
        if(onum>free_l and free_l!=0)
        {
            onum-=free_l;
            cout<<"Line"<<num2letter(i)<<": "<<endl;
            //h_seat_set[i]->order_some(free_l,name,time);
            h_seat[i].order_some(free_l,name,time);
        }
        else if(free_l!=0)
        {
            cout<<"Line"<<num2letter(i)<<": "<<endl;
            //h_seat_set[i]->order_some(onum,name,time);
            h_seat[i].order_some(onum,name,time);
            break;
        }
    }
    free_all-=num;
    //free_seat();
    return true;
}

bool Hall::buy_ran(int num,string name)
{
    if(num>free_all+get_order_num(name))
    {
        cout<<"No enough seat left"<<endl;
        return false;
    }
    int bnum=num;
    for(int i=0;i<line_num;i++)
    {
        //int onum=h_seat_set[i]->l_get_order_num(name);
        int onum=h_seat[i].l_get_order_num(name);
        if(onum!=0)
        {
            cout<<"Line"<<num2letter(i)<<": "<<endl;
            if(onum>=bnum)
            {
                //h_seat_set[i]->buy_some(bnum,name);
                h_seat[i].buy_some(bnum,name);
                bnum=0;
                break;
            }
            else
            {
                bnum-=onum;
                //h_seat_set[i]->buy_some(onum,name);
                h_seat[i].buy_some(onum,name);
            }
        }
    }
    if(bnum==0)
    {
        return true;
    }
    for(int i=0;i<line_num;i++)
    {
        //int onum=h_seat_set[i]->l_free_get();
        int onum=h_seat[i].l_free_get();
        if(onum!=0)
        {
            cout<<"Line"<<num2letter(i)<<": "<<endl;
            if(onum>=bnum)
            {
                //h_seat_set[i]->buy_some(bnum,name);
                h_seat[i].buy_some(bnum,name);
                bnum=0;
                break;
            }
            else
            {
                bnum-=onum;
                //h_seat_set[i]->buy_some(onum,name);
                h_seat[i].buy_some(onum,name);
            }
        }
    }
    update_h_mes();
    return 0;
}

bool Hall::deorder_ran(int num,string name)
{
    int dnum=num;
    for(int i=0;i<line_num;i++)
    {
        //int onum=h_seat_set[i]->l_get_order_num(name);
        int onum=h_seat[i].l_get_order_num(name);
        if(onum!=0)
        {
            cout<<"Line"<<num2letter(i)<<": "<<endl;
            if(onum>=dnum)
            {
                //h_seat_set[i]->deorder_some(dnum,name);
                h_seat[i].deorder_some(dnum,name);
                break;
            }
            else
            {
                dnum-=onum;
                //h_seat_set[i]->deorder_some(onum,name);
                h_seat[i].deorder_some(onum,name);
            }
        }
    }
    if(dnum==num)
    {
        cout<<"Nothing to do"<<endl;
    }
    update_h_mes();
    return true;
}

bool Hall::print_hall()
{
    for(int i=0;i<line_num;i++)
    {
        cout<<"Line"<<num2letter(i)<<": ";
        //h_seat_set[i]->print_l();
        h_seat[i].print_l();
    }
    return true;
}

bool Hall::print_host_hall(string name)
{
    for(int i=0;i<line_num;i++)
    {
        cout<<"Line"<<num2letter(i)<<": "<<endl;
        //h_seat_set[i]->print_host_l(name);
        h_seat[i].print_host_l(name);
    }
    return true;
}

vector<string> subcom(string com)
{
    vector<string> tmp;
    string stmp;
    istringstream istr(com);
    while(!istr.eof())
    {
        istr>>stmp;
        tmp.push_back(stmp);
    }
    return tmp;
}

bool valid_name(string name)
{
    int len=name.length();
    if(len<1)
    {
        return false;
    }
    if(name[0]>='0' and name[0]<='9')
    {
        return false;
    }
    return true;
}

bool is_a_line(string str)
{
    int len=str.length();
    for(int i=0;i<len;i++)
    {
        if(str[i]<'A' or str[i]>'Z')
        {
            return false;
        }
    }
    return true;
}

bool is_a_num(string str)
{
    int len=str.length();
    for(int i=0;i<len;i++)
    {
        if(str[i]<'0' or str[i]>'9')
        {
            return false;
        }
    }
    return true;
}

int Hall::auto_work(string command,vector<int> t_list)
{
    cout<<"The result of command["<<command<<"]:"<<endl;
    if(t_list.size()!=4)
    {
        cout<<"Time parameter error!"<<endl;
        return 0;
    }
    
    vector<string> scom=subcom(command);
    int slen=scom.size();
    if(slen<3 or slen>5)
    {
        cout<<"Invalid command, too many item"<<endl;
        return 0;
    }
    if(!valid_name(scom[slen-1]))
    {
        cout<<"Invalid command, invalid name"<<endl;
        return 0;
    }
    
    if(scom[0]=="reserve")
    {
        if(is_a_num(scom[1]))
        {
            if(slen!=3)
            {
                cout<<"Invalid command"<<endl;
                return 0;
            }
            int tnum=atoi(scom[1].c_str());
            if(order_ran(tnum,scom[2],rtime))
            {
                //cout<<"here 2"<<endl;
                //print_hall();
                //cout<<"here 2 end"<<endl;
                h_timepass(t_list[0]);
                return t_list[0];
            }
            else
            {
                //cout<<"exec error"<<endl;
                return 0;
            }
        }
        else if(is_a_line(scom[1]))
        {
            int line=letter2num(scom[1]);
            if(slen==3)
            {
                //if(h_seat_set[line]->order_line(scom[2],rtime))
                if(h_seat[line].order_line(scom[2],rtime))
                {
                    h_timepass(t_list[0]);
                    return t_list[0]; 
                }
                else
                {
                    //cout<<"exec error"<<endl;
                    return 0;
                }
            }
            else if(slen==5)
            {
                if(is_a_num(scom[2]) and is_a_num(scom[3]))
                {
                    int snum=atoi(scom[2].c_str());
                    int endnum=atoi(scom[3].c_str());
                    //if(h_seat_set[line]->order_set(snum,endnum,scom[4],rtime))
                    if(h_seat[line].order_set(snum,endnum,scom[4],rtime))
                    {
                        //cout<<"here 1"<<endl;
                        //print_hall();
                        //cout<<"here 1 end"<<endl;
                        h_timepass(t_list[0]);
                        return t_list[0];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    cout<<"Invalid command"<<endl;
                    return 0;
                }
            }
            else
            {
                int scom_len=scom[2].length();
                if(is_a_num(scom[2]))
                {
                    //if(h_seat_set[line]->order_one(atoi(scom[2].c_str()),scom[3],rtime))
                    if(h_seat[line].order_one(atoi(scom[2].c_str()),scom[3],rtime))
                    {
                        //cout<<"here 1"<<endl;
                        //print_hall();
                        //cout<<"here 1 end"<<endl;
                        h_timepass(t_list[0]);
                        return t_list[0];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else if(is_a_num(scom[2].substr(0,scom_len-1)) and scom[2][scom_len-1]=='r')
                {
                    int line_num=atoi(scom[2].substr(0,scom_len-1).c_str());
                    if(order_lset(line+1,line_num,scom[3],rtime))//!!
                    {
                        h_timepass(t_list[0]);
                        return t_list[0];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else if(is_a_num(scom[2].substr(0,scom_len-1)) and scom[2][scom_len-1]=='c')
                {
                    int seat_num=atoi(scom[2].substr(0,scom_len-1).c_str());
                    //if(h_seat_set[line]->order_some(seat_num,scom[3],rtime))
                    if(h_seat[line].order_some(seat_num,scom[3],rtime))
                    {
                        h_timepass(t_list[0]);
                        return t_list[0];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    cout<<"Invalid command"<<endl;
                    return 0;
                }
            }
        }//end of else if(is_a_line(scom[1]))
        else
        {
            cout<<"Invalid command"<<endl;
            return 0;
        }
    }//end of if(scom[0]=="reserve")
    else if(scom[0]=="ticket")
    {
        if(is_a_num(scom[1]))
        {
            if(slen!=3)
            {
                cout<<"Invalid command"<<endl;
                return 0;
            }
            int tnum=atoi(scom[1].c_str());
            if(buy_ran(tnum,scom[2]))
            {
                //cout<<"here 2"<<endl;
                //print_hall();
                //cout<<"here 2 end"<<endl;
                h_timepass(t_list[1]);
                return t_list[1];
            }
            else
            {
                //cout<<"exec error"<<endl;
                return 0;
            }
        }
        else if(is_a_line(scom[1]))
        {
            int line=letter2num(scom[1]);
            if(slen==3)
            {
                //if(h_seat_set[line]->order_line(scom[2],rtime))
                if(h_seat[line].buy_line(scom[2]))
                {
                    h_timepass(t_list[1]);
                    return t_list[1]; 
                }
                else
                {
                    //cout<<"exec error"<<endl;
                    return 0;
                }
            }
            else if(slen==5)
            {
                if(is_a_num(scom[2]) and is_a_num(scom[3]))
                {
                    int snum=atoi(scom[2].c_str());
                    int endnum=atoi(scom[3].c_str());
                    //if(h_seat_set[line]->order_set(snum,endnum,scom[4],rtime))
                    if(h_seat[line].buy_set(snum,endnum,scom[4]))
                    {
                        //cout<<"here 1"<<endl;
                        //print_hall();
                        //cout<<"here 1 end"<<endl;
                        h_timepass(t_list[1]);
                        return t_list[1];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    cout<<"Invalid command"<<endl;
                    return 0;
                }
            }
            else
            {
                int scom_len=scom[2].length();
                if(is_a_num(scom[2]))
                {
                    //if(h_seat_set[line]->order_one(atoi(scom[2].c_str()),scom[3],rtime))
                    if(h_seat[line].buy_one(atoi(scom[2].c_str()),scom[3]))
                    {
                        //cout<<"here 1"<<endl;
                        //print_hall();
                        //cout<<"here 1 end"<<endl;
                        h_timepass(t_list[1]);
                        return t_list[1];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else if(is_a_num(scom[2].substr(0,scom_len-1)) and scom[2][scom_len-1]=='r')
                {
                    int line_num=atoi(scom[2].substr(0,scom_len-1).c_str());
                    if(buy_lset(line+1,line_num,scom[3]))//!!
                    {
                        h_timepass(t_list[1]);
                        return t_list[1];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else if(is_a_num(scom[2].substr(0,scom_len-1)) and scom[2][scom_len-1]=='c')
                {
                    int seat_num=atoi(scom[2].substr(0,scom_len-1).c_str());
                    //if(h_seat_set[line]->order_some(seat_num,scom[3],rtime))
                    if(h_seat[line].buy_some(seat_num,scom[3]))
                    {
                        h_timepass(t_list[1]);
                        return t_list[1];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    cout<<"Invalid command"<<endl;
                    return 0;
                }
            }
        }//end of else if(is_a_line(scom[1]))
        else
        {
            cout<<"Invalid command"<<endl;
            return 0;
        }
        //print_hall();
        return 0;
    }//end of else if(scom[0]=="ticket")
    else if(scom[0]=="cancel")
    {
        if(is_a_num(scom[1]))
        {
            if(slen!=3)
            {
                cout<<"Invalid command"<<endl;
                return 0;
            }
            int tnum=atoi(scom[1].c_str());
            if(deorder_ran(tnum,scom[2]))
            {
                //cout<<"here 2"<<endl;
                //print_hall();
                //cout<<"here 2 end"<<endl;
                h_timepass(t_list[2]);
                return t_list[2];
            }
            else
            {
                //cout<<"exec error"<<endl;
                return 0;
            }
        }
        else if(is_a_line(scom[1]))
        {
            int line=letter2num(scom[1]);
            if(slen==3)
            {
                //if(h_seat_set[line]->order_line(scom[2],rtime))
                if(h_seat[line].deorder_line(scom[2]))
                {
                    h_timepass(t_list[2]);
                    return t_list[2]; 
                }
                else
                {
                    //cout<<"exec error"<<endl;
                    return 0;
                }
            }
            else if(slen==5)
            {
                if(is_a_num(scom[2]) and is_a_num(scom[3]))
                {
                    int snum=atoi(scom[2].c_str());
                    int endnum=atoi(scom[3].c_str());
                    //if(h_seat_set[line]->order_set(snum,endnum,scom[4],rtime))
                    if(h_seat[line].deorder_set(snum,endnum,scom[4]))
                    {
                        //cout<<"here 1"<<endl;
                        //print_hall();
                        //cout<<"here 1 end"<<endl;
                        h_timepass(t_list[2]);
                        return t_list[2];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    cout<<"Invalid command"<<endl;
                    return 0;
                }
            }
            else
            {
                int scom_len=scom[2].length();
                if(is_a_num(scom[2]))
                {
                    //if(h_seat_set[line]->order_one(atoi(scom[2].c_str()),scom[3],rtime))
                    if(h_seat[line].deorder_one(atoi(scom[2].c_str()),scom[3]))
                    {
                        //cout<<"here 1"<<endl;
                        //print_hall();
                        //cout<<"here 1 end"<<endl;
                        h_timepass(t_list[2]);
                        return t_list[2];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else if(is_a_num(scom[2].substr(0,scom_len-1)) and scom[2][scom_len-1]=='r')
                {
                    int line_num=atoi(scom[2].substr(0,scom_len-1).c_str());
                    if(deorder_lset(line+1,line_num,scom[3]))//!!
                    {
                        h_timepass(t_list[2]);
                        return t_list[2];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else if(is_a_num(scom[2].substr(0,scom_len-1)) and scom[2][scom_len-1]=='c')
                {
                    int seat_num=atoi(scom[2].substr(0,scom_len-1).c_str());
                    //if(h_seat_set[line]->order_some(seat_num,scom[3],rtime))
                    if(h_seat[line].deorder_some(seat_num,scom[3]))
                    {
                        h_timepass(t_list[2]);
                        return t_list[2];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    cout<<"Invalid command"<<endl;
                    return 0;
                }
            }
        }//end of else if(is_a_line(scom[1]))
        else
        {
            cout<<"Invalid command"<<endl;
            return 0;
        }
        //print_hall();
        return 0;
    }//end of else if(scom[0]="cancel")
    else if(scom[0]=="show")
    {
        if(slen==3 and scom[1]=="all")
        {
            //cout<<"The result of command["<<command<<"]:"<<endl;
            print_host_hall(scom[2]);
            h_timepass(t_list[3]);
            return t_list[3];
        }
        else
        {
            cout<<"Invalid command"<<endl;
            return 0;
        }
    }//end of else if(scom[0]="show")
    else
    {
        cout<<"Invalid command"<<endl;
        return 0;
    }
    return 0;
}
