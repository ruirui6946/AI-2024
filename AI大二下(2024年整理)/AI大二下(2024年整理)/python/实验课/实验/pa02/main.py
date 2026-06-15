import pymysql
from work import *
import queue
import time
from multiprocessing.dummy import Pool
import threading
import datetime
# 互斥锁
lock = threading.Lock()
start = time.time()
q = queue.Queue()
# 爬取个数
num = 0
f = open('res.txt','r',encoding='utf8')
names = f.readlines()
def pre():
    l = []
    for name in names:
        l.append(name.rstrip())
    print("一共{}个人，现在开始".format(len(l)))
    return l

# 查询函数
def crawl(nam): 
    # 获取互斥锁 
    lock.acquire() 
    # 连接数据库
    conn = pymysql.connect(host="localhost", user="root", password="0000", db="baidu_person", charset="utf8")

    # 获取游标对象
    cur = conn.cursor()
    global q
    global num
    url = get_url(nam)
    q.put(url)
    while q.qsize():
        url = q.get()
        name,content,soup,q,found = pre_process(nam,url,q)
        if found:
            url = q.get()
            name,content,soup = pre_process_weak(name,url)
        information,exist_the_people = basic_infomation(soup) # 简介
        num += 1

        if exist_the_people:
            people_id,message,relation,experience = get_id(content),basic_message(content),basic_relationship(content,name),basic_experience(soup) # 基本信息, 关系,履历
        else:
            people_id,message,relation,experience = '\'\'','\'\'','\'\'','\'\''
        
        # 构造插入语句
        sql = "insert into result (id, people_name, people_id, basic_information, basic_message, basic_relationship, basic_experience) values (0, %s, %s, %s, %s, %s, %s);"
        
        # 执行插入语句
        try:
            cur.execute(sql % ('\''+name+'\'', '\''+people_id+'\'', '\''+information+'\'', '\''+message+'\'', '\''+relation+'\'', '\''+experience+'\''))
            print("成功插入第{}个数据:{}的相关信息,插入时间:".format(num,name)+str(datetime.datetime.now()))
            conn.commit()
        except Exception as e:
            print("插入失败:", e)

    cur.close() 
    conn.close() 
    # 释放互斥锁 
    lock.release()

names = pre()
pool = Pool(10)
pool.map(crawl,names)

# 关闭文件
f.close()
end = time.time()
print("访问%d个数据，耗时%d秒" % (num,end-start))