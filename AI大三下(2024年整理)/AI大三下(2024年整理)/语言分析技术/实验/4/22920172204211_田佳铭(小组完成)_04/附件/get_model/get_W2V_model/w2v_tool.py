import logging
import os
from gensim.models import word2vec
import json
import numpy as np
import time
import pickle

#模型加载函数，默认模型存储在model_train.pkl中
def get_model():
    pkl_file = open('model_train.pkl', 'rb')
    model2=pickle.load(pkl_file)
    return model2

#选择函数，
#model：模型；candid：候选者列表；context：上下文列表
#函数返回可能性最大的一个候选者
#函数会跳过不在模型词典中的词
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

model=get_model()
candid=["炸酱面","热干面"]
context=["赵德汉"]
print(sim_choose(model,candid,context))
