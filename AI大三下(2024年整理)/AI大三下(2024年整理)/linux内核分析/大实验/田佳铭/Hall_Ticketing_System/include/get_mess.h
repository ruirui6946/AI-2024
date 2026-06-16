//用于传输输入信息
struct in_mess
{
    int n,m,k,rvt;
    vector<vector<string> > com_set;
    //vector<string> stmp;
    vector<vector<int> > time_list;
    //vector<int> itmp;
};

struct in_mess get_mess(string filename)
{
    ifstream ifs;
    ifs.open(filename.c_str()); //打开文件，文件存在判断在主程序中
    struct in_mess res;
    int rt,tt,ct,ckt;
    string tstr;
    vector<string> stmp;
    vector<int> itmp;           //临时变量声明
    ifs>>res.n>>res.m>>res.k>>res.rvt;
    res.rvt*=60;                //单位：s
    getline(ifs,tstr);          // a blank line
    for(int i=1;i<=res.k;i++)
    {
        itmp.clear();
        stmp.clear();
        getline(ifs,tstr);
        string sstr="";
        stringstream ss;
        ss<<i;
        ss>>sstr;               //string转int
        sstr="agent "+sstr;
        if(tstr!=sstr)          //判断是否是此agent的交易列表
        {
            //cout<<"1"<<endl;
            cout<<"imput error"<<endl;
            ifs.close();
            exit(0);
        }

        //时间参数获取
        getline(ifs,tstr);
        sstr="reserve ";
        if(tstr.compare(0,8,sstr,0,8))
        {
            cout<<"imput error"<<endl;
            ifs.close();
            exit(0);
        }
        string subs;
        subs=tstr.substr(8,tstr.length()-8);
        rt=atoi(subs.c_str());
        itmp.push_back(rt);

        getline(ifs,tstr);
        sstr="ticket ";
        if(tstr.compare(0,7,sstr,0,7))
        {
            cout<<"imput error"<<endl;
            ifs.close();
            exit(0);
        }
        subs=tstr.substr(7,tstr.length()-7);
        tt=atoi(subs.c_str());
        itmp.push_back(tt);

        getline(ifs,tstr);
        sstr="cancel ";
        if(tstr.compare(0,7,sstr,0,7))
        {
            cout<<"imput error"<<endl;
            ifs.close();
            exit(0);
        }
        subs=tstr.substr(7,tstr.length()-7);
        ct=atoi(subs.c_str());
        itmp.push_back(ct);

        getline(ifs,tstr);
        sstr="check_customer ";
        if(tstr.compare(0,15,sstr,0,15))
        {
            cout<<"imput error"<<endl;
            ifs.close();
            exit(0);
        }
        subs=tstr.substr(15,tstr.length()-15);
        ckt=atoi(subs.c_str());
        itmp.push_back(ckt);
        res.time_list.push_back(itmp);

        //交易列表获取
        getline(ifs,tstr);
        while(tstr!="end")
        {
            stmp.push_back(tstr);
            getline(ifs,tstr);
        }
        res.com_set.push_back(stmp);
    }
    ifs.close();
    return res;
}
