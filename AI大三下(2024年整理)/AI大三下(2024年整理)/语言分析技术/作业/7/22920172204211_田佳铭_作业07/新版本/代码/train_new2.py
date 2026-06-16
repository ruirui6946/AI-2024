import re
import nltk
from nltk.util import unique_list
import math
import pickle
import os
import numpy
 
# 遍历指定目录，显示目录下的所有文件名
def eachFile(filepath):
    pathDir =  os.listdir(filepath)
    #print(pathDir)
    path=[]
    for allDir in pathDir:
        child = os.path.join('%s%s' % (filepath, allDir))
        path.append(child)
    return path
        #print child.decode('gbk') # .decode('gbk')是解决中文显示乱码问题

#一个词是否在文档中出现
def word_exist(text,word):
    for i in range(len(text)):
        if text[i]==word:
            return True
    return False

#计算出现t的文档个数
def make_nt(text_list,word_list):
    word_rate=[]
    for i in range(len(word_list)):
        rate=0
        for j in range(len(text_list)):
            if word_exist(text_list[j],word_list[i])==True:
                rate=rate+1
        word_rate.append(rate)
    return word_rate

#计算idf
def make_idf(x,N):
    for i in range(len(x)):
        x[i]=math.log(N/x[i])
    return x

#计算一个word在文章除出现次数
def num_count(text,word):
    num=0
    for i in range(len(text)):
        if text[i]==word:
            num=num+1
    return num

#计算tf
def make_tf(text_list,word_list):
    res=[]
    for i in range(len(text_list)):
        rate=[]
        for j in range(len(word_list)):
            rate.append(num_count(text_list[i],word_list[j]))
        res.append(rate)
    return res

#计算内积
def amulb(a,b):
    if len(a)!=len(b):
        print('error')
        exit
    for i in range(len(a)):
        a[i]=a[i]*b[i]
    return a

#计算tfidf
def tfidf(tf_list,df):
    res=[]
    for i in range(len(tf_list)):
        res.append(amulb(tf_list[i],df))
    return res

#计算每一维的均值
def vec_mean(vec_list):
    res=[]
    for i in range(len(vec_list[0])):
        tmp=[]
        for j in range(len(vec_list)):
            tmp.append(vec_list[j][i])
        res.append(numpy.mean(tmp))
    return res

#计算每一维的标准差
def vec_var(vec_list):
    res=[]
    for i in range(len(vec_list[0])):
        tmp=[]
        for j in range(len(vec_list)):
            tmp.append(vec_list[j][i])
        res.append(numpy.var(tmp))
    return res

#idf过小消去阈值
del_range=4

#读取训练集
#sim_dir='D:\\my_world\\class\\大三\\大三下\\主修课\\语言分析技术\\作业\\7\\代码\\data\\train\\sim\\'
#tra_dir='D:\\my_world\\class\\大三\\大三下\\主修课\\语言分析技术\\作业\\7\\代码\\data\\train\\tra\\'
#sim_flist=eachFile(sim_dir)
#tra_flist=eachFile(tra_dir)
text_list=[]
sim_list=[]
tra_list=[]

#for i in range(len(sim_flist)):
#    with open(sim_flist[i], 'r',encoding='gbk') as f:
#        text=f.read()
#        text=re.findall(r'[\u4e00-\u9fa5]',text)
#        text_list.append(text)
#        sim_list.append(text)

#for i in range(len(tra_flist)):
#    with open(tra_flist[i], 'r',encoding='gbk') as f:
#        text=f.read()
#        text=re.findall(r'[\u4e00-\u9fa5]',text)
#        text_list.append(text)
#        tra_list.append(text)

with open('xinhua_j.txt', 'r',encoding='utf-8') as f:
    text=f.read()
find2=0
while True:
    find1=text.find('<text title',find2,len(text))
    if find1==-1:
        break;
    find2=text.find('</text>',find1,len(text))
    if find2==-1:
        break;
    new_text=re.findall(r'[\u4e00-\u9fa5]',text[find1:find2])
    text_list.append(new_text)
    sim_list.append(new_text)
#print(text_list[1:3])
print(len(text_list))

#msn_f.txt
sim_num=0
with open('msn_f.txt', 'r',encoding='utf-8') as f:
    text=f.read()
find2=0
while True:
    sim_num=sim_num+1
    if sim_num>len(sim_list):
        break
    find1=text.find('<text title',find2,len(text))
    if find1==-1:
        break
    find2=text.find('</text>',find1,len(text))
    if find2==-1:
        break
    new_text=re.findall(r'[\u4e00-\u9fa5]',text[find1:find2])
    text_list.append(new_text)
    tra_list.append(new_text)

#获取所有word，即获取向量维数        
word_list=[]
for i in range(len(text_list)):
    word_list=word_list+unique_list(text_list[i])
    word_list=unique_list(word_list)

#计算tfidf
word_rate=make_nt(text_list,word_list)
word_rate=make_idf(word_rate,len(text_list))

#print(word_rate)

#word_tf=make_tf(text_list,word_list)
#word_tfidf=tfidf(word_tf,word_rate)
#print(len(word_list))
#print(len(word_tfidf[1]))

#print(len(word_list))

#删除idf太低的字
for i in range(len(word_rate)-1,-1,-1):
    #print(i)
    #print(len(word_rate))
    if word_rate[i]<=del_range:
        del word_rate[i]
        del word_list[i]

#print(len(word_rate))
#print(len(word_list))

sim_tf=make_tf(sim_list,word_list)
tra_tf=make_tf(tra_list,word_list)
sim_tfidf=tfidf(sim_tf,word_rate)
tra_tfidf=tfidf(tra_tf,word_rate)
#tfidf=sim_tfidf+tra_tfidf

#计算均值与方差
sim_mean=vec_mean(sim_tfidf)
tra_mean=vec_mean(tra_tfidf)
sim_var=vec_var(sim_tfidf)
tra_var=vec_var(tra_tfidf)
#print(sim_mean)
#print(tra_mean)
#print(sim_var)
#print(tra_var)

#保存变量
pickle.dump(word_list,open('word_list.txt', 'wb') )
pickle.dump(word_rate,open('word_rate.txt', 'wb') )
pickle.dump(sim_tfidf,open('sim_tfidf.txt', 'wb') )
pickle.dump(tra_tfidf,open('tra_tfidf.txt', 'wb') )
pickle.dump(sim_mean,open('sim_mean.txt', 'wb') )
pickle.dump(tra_mean,open('tra_mean.txt', 'wb') )
pickle.dump(sim_var,open('sim_var.txt', 'wb') )
pickle.dump(tra_var,open('tra_var.txt', 'wb') )
