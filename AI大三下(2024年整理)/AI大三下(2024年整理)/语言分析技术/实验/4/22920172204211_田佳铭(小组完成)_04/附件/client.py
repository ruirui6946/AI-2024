import logging
import numpy as np
import time
import os
import re
import pickle
import math
import jieba
import jieba.analyse
import sys

from functools import reduce
from gensim.models import word2vec

from Pinyin2Hanzi import DefaultHmmParams
from Pinyin2Hanzi import viterbi

from langconv import *

#忽略警告
import warnings
warnings.filterwarnings("ignore")
#------------------------------------------
#----------------繁简转换------------------
#------------------------------------------
def ft2jt(sentence):
    '''
    将sentence中的繁体字转为简体字
    :param sentence: 待转换的句子
    :return: 将句子中繁体字转换为简体字之后的句子
    '''
    sentence = Converter('zh-hans').convert(sentence)
    return sentence
#------------------------------------------
#----------------找出字母------------------
#------------------------------------------
def get_alpha_str(s):
    result=re.split(r'[^A-Za-z]', s)
    while '' in result:
        result.remove('')
    #print(result)
    return result
# ------------------------------------------
# -------------查询拼音-汉字表---------------
# ------------------------------------------
def py2hz(x,dict):
    if x in dict:
        return dict[x]
    else :
        return []
# ------------------------------------------
# -------------模型加载函数---------------
# ------------------------------------------
def get_model(str):
    pkl_file = open(str, 'rb')
    model=pickle.load(pkl_file)
    return model
# ------------------------------------------
#选择函数，
#model：模型；candid：候选者列表；context：上下文列表
#函数返回可能性最大的一个候选者
#函数会跳过不在模型词典中的词
# ------------------------------------------
def sim_choose(model,candid,context):
    sim=[]
    tmp=0
    for i in range(len(candid)):
        tmp=0
        if candid[i] not in model:
            sim.append(tmp)
            continue
        for j in range(len(context)):
            if context[j] not in model:
                continue
            tmp=tmp+model.wv.similarity(candid[i],context[j])
        sim.append(tmp)
    return candid[sim.index(max(sim))]
# --------------------------------判断拼音还是英文部分函数-----------------------------------------
#实现一个计算单字母和双字母（因为只做到二元语音模型）频数的函数
def frequency(symbol,corpus):
    l = len(symbol)
    freq = 0
    for word in corpus.keys():
        freq_i = 0
        for i in range(len(word)):
            if l == 1:
                if word[i] == symbol:
                    freq_i += 1
            if l == 2:
                if word[i:i+2] == symbol:
                    # print(word)
                    freq_i += 1
        freq_i = freq_i * corpus[word]
        freq += freq_i
    return freq

#实现一个计算条件概率公式
def condition_prob(w1,w2,corpus):
    freq_w1 = frequency(w1,corpus)
    freq_w2 = frequency(w2,corpus)
    return (float(freq_w2)+1)/(float(freq_w1)+len(corpus.keys()))

# 两数相乘
def mul(x, y) :
    return x * y

#公式实现
def testing(word,corpus):
    cond_probs = []
    cond_p = condition_prob('>','>'+word[0],corpus)
    cond_probs.append(cond_p)

    for i in range(len(word)-1):
        cond_p = condition_prob(word[i],word[i:i+2],corpus)
        cond_probs.append(cond_p)

    cond_p = condition_prob(word[-1],word[-1]+'<',corpus)
    cond_probs.append(cond_p)

    reliability = reduce(mul, cond_probs) * math.pow(10,len(word))
    return reliability

#判断调用
def cnORen(test_word,model):
    test_word = test_word.lower()
    reliability = testing(test_word,model)
    # print(reliability)
    if reliability >= 1e-3:
        print("'%s' is a english word" % test_word)
        return 0
    else:
        print("'%s' is a pinyin" % test_word)
        return 1
# --------------------------------判断拼音还是英文部分函数end-----------------------------------------

# ------------------------------------------
# -------------拆字倒映射表模型加载函数---------------
# ------------------------------------------
def i_chaizi_get(filename):
    i_chaizi=[]
    with open("i_chazi.txt",'r',encoding='utf-8') as f:
        line=f.readline()
        while line:
            text=line.split()
            #print(text)
            i_chaizi.append(text)
            line=f.readline()
    return i_chaizi
# ------------------------------------------
# -------------字典加载函数---------------
# ------------------------------------------
def loadDict(filename):
    dict={}
    for line in  open('规范词典2000条.txt','r',encoding='utf-8') :
        line =str(line.strip('\n'))
        text = line.split()
        #print(text)
        if text[0] in dict :
            dict[text[0]].append(text[1])
        else :
            dict[text[0]] = []
            dict[text[0]].append(text[1])
    return dict

# ------------------------------------------
# -------------拆分拼音函数---------------
# ------------------------------------------
def pinyin_or_word(string):
    string = string.lower()
    stringlen = len(string)
    max_len = 6
    result = []
    n = 0
    while n < stringlen:
        matched = 0
        temp_result = []
        for i in range(max_len, 0, -1):
            s = string[0:i]
            if s in pinyinLib:
                temp_result.append(string[:i])
                matched = i
                break
            if i == 1 and len(temp_result) == 0:
                print("这可能是个英文单词！")
                return []
        result.extend(temp_result)
        string = string[matched:]
        n += matched
    return result

#------------------------------------------
#----------------主函数   -----------------
#------------------------------------------
#读入常用字词-拼音 映射
print("加载模型ing……")
dict=loadDict('规范词典2000条.txt')
webembeding = get_model('model_train.pkl')
pinyin2hanzi = get_model('pinyin_hanzi.pkl')
pinyinORword = get_model('corpus.pkl')
i_chaizi=i_chaizi_get("i_chazi.txt")

pinyinLib = []
for line in open("pinyinLib.txt"):
    linetemp =str(line.strip('\n'))
    pinyinLib.append(linetemp)

print("加载模型成功")
#读入输入，逐行处理
if len(sys.argv)<=1:
    print('error, no input')
    exit
else:
    if str(sys.argv[1])=='-s':
        if len(sys.argv)<=2:
            print('error, no text')
            exit
        else:
            text=str(sys.argv[2])
    else:
        filename=str(sys.argv[1])
        with open(filename, 'r',encoding='utf-8') as f:
            text=f.read()

text =text.split('\n')
#if len(text)==1:
#    text.append("")

correct=0
onum = []
jnum = []
for i ,row in enumerate(text):
    #if i % 2 == 0:
    onum.append(row)
    #else:
    #    jnum.append(row)
#for line, j in zip(onum, jnum):
for i,line in enumerate(onum):
#for line in text:
    result=line
    print("原句："+result)
    # ----------------繁简转换------------
    result = ft2jt(result)
    # ----------------字母符号转换------------
    alpha_str=get_alpha_str(result)#找出字母的str
    if len(alpha_str) != 0 :
        for word in alpha_str:
            #print(word)
            hanzi_list=py2hz(word, pinyin2hanzi)
            # if(len(hanzi_list)==1) :#一个选择
            #     result = result.replace(word,hanzi_list[0])
            #     continue
            if(len(hanzi_list)>0) :#在映射表里
                context =  [seg for seg in jieba.cut(result) if len(seg) >= 0]#用jieba进行分词
                hanzi_list.append(word)#原文加入
                #print(context)
                tempstr=sim_choose(webembeding,hanzi_list,context)
                result = result.replace(word, tempstr)
                continue
            elif(len(hanzi_list)==0) :#不在映射，判断是拼音还是英文
                if cnORen(word , pinyinORword) ==1 :#判断是拼音，这里应该填拼音转中文程序
                    pinyin_str=pinyin_or_word(word)
                    if len(pinyin_str)<=1:
                        continue
                    hmmparams = DefaultHmmParams()
                    result1 = viterbi(hmm_params=hmmparams, observations=(pinyin_str), path_num = 2)
                    item2 = "".join(result1[0].path)
                    hz_list=[]
                    hz_list.append(item2)
                    hz_list.append(word)
                    context =  [seg for seg in jieba.cut(result) if len(seg) >= 0]#用jieba进行分词
                    item2=sim_choose(webembeding,hz_list,context)
                    result = result.replace(word, item2)
                    continue

    # ---------------拆字型转换------------
    for i in range(len(i_chaizi)):
        hanzilist=[]
        if result.find(i_chaizi[i][0])!= -1:
            hanzilist.append(i_chaizi[i][1])
            hanzilist.append(i_chaizi[i][0])  # 原文加入
            context =  [seg for seg in jieba.cut(result) if len(seg) >= 0]#用jieba进行分词
            tempstr = sim_choose(webembeding, hanzilist, context)
            result = result.replace(i_chaizi[i][0], tempstr)

    # ---------------刷字典------------
    for key in dict.keys():
        templist = []
        if result.find(key)!= -1:
            templist=dict[key]
            templist.append(key)  # 原文加入
            context =  [seg for seg in jieba.cut(result) if len(seg) >= 0]#用jieba进行分词
            tempstr = sim_choose(webembeding, templist, context)
            result = result.replace(key, tempstr)

    print("转后：" + result)
    #print("正确：" + j)
    #if result==j :
    #    correct=correct+1

#print("正确个数：")
#print(correct)
#correct=correct/(len(onum))
#print("正确率：" )
#print(correct)

