import jieba
import jieba.analyse
import logging
import os
from gensim.models import word2vec
import json
import numpy as np
import time

def eachFile(filepath):
    pathDir =  os.listdir(filepath)
    #print(pathDir)
    path=[]
    for allDir in pathDir:
        child = os.path.join('%s%s' % (filepath, allDir))
        path.append(child)
    return path

root = os.getcwd()#获取当前路径
sim_dir=root+"\\train\\"
#print(sim_dir)
#sim_dir="D:\\my_world\\class\\大三\\大三下\\主修课\\语言分析技术\\实验\\4\\代码\\W2V1\\train\\"
sim_flist=eachFile(sim_dir)#获取所有训练文件夹的路径
#print(sim_flist)

for i in range(len(sim_flist)):
    sim_flist[i]=sim_flist[i]+"\\"

train_list=[]#获取所有训练文件路径
for i in range(len(sim_flist)):
    train_list=train_list+eachFile(sim_flist[i])

text=""
for i in range(len(train_list)):
#    if i>=2:#测试用小训练集
#        continue
    with open(train_list[i],encoding='ansi') as f:
        #print(train_list[i])
        text=text+f.read()
#用jieba进行分词
document_cut = jieba.cut(text)
result = ' '.join(document_cut)

#保存分词
with open('./segment.txt', 'w',encoding='utf-8') as f2:
    f2.write(result)
