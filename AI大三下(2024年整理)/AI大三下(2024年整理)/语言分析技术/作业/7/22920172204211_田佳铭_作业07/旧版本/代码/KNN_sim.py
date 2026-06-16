import re
import pickle
from operator import itemgetter
import math

#计算一个word在文章除出现次数
def num_count(text,word):
    num=0
    for i in range(len(text)):
        if text[i]==word:
            num=num+1
    return num

#计算tf
def make_tf(text,word_list):
    rate=[]
    for j in range(len(word_list)):
        rate.append(num_count(text,word_list[j]))
    return rate

#计算内积
def amulb(a,b):
    if len(a)!=len(b):
        print('error')
        exit
    for i in range(len(a)):
        a[i]=a[i]*b[i]
    return a

#计算长度
def mylen(a):
    res=0
    for i in range(len(a)):
        res=res+a[i]*a[i]
    res=math.sqrt(res)
    return res

#计算欧式距离的平方
def distance(a,b):
    if len(a)!=len(b):
        print('error')
        exit
    res=0
    for i in range(len(a)):
        res=res+(a[i]-b[i])*(a[i]-b[i])
    return res

#长度归一化
def sim(a,b):
    return distance(a,b)/(mylen(a)*mylen(b))
    

#读入测试文件
#filename='D:\\my_world\\class\\大三\\大三下\\主修课\\语言分析技术\\作业\\7\\代码\\data\\test\\tra.txt'
filename='D:\\my_world\\class\\大三\\大三下\\主修课\\语言分析技术\\作业\\7\\代码\\data\\test\\sim.txt'
with open(filename, 'r',encoding='gbk') as f:
    text=f.read()
    text=re.findall(r'[\u4e00-\u9fa5]',text)

#读入变量
word_list=pickle.load(open('word_list.txt', 'rb'))
word_rate=pickle.load(open('word_rate.txt', 'rb'))
sim_tfidf=pickle.load(open('sim_tfidf.txt', 'rb'))
tra_tfidf=pickle.load(open('tra_tfidf.txt', 'rb'))

#计算本文档的tfidf值
tf=make_tf(text,word_list)
tfidf=amulb(tf,word_rate)

#计算向量之间的举例
dis_list=[]
for i in range(len(sim_tfidf)):
    dis_list.append((sim(tfidf,sim_tfidf[i]),1))
for i in range(len(tra_tfidf)):
    dis_list.append((sim(tfidf,tra_tfidf[i]),2))

#排序
dis_list=sorted(dis_list,key=itemgetter(0))
#print(dis_list)

#定义K值
k=3
num1=0
num2=0

for i in range(k):
    if dis_list[i][1]==1:
        num1=num1+1
    else:
        num2=num2+1

#判断与输出
if num1>num2:
    print('简体')
else:
    print('繁体')
