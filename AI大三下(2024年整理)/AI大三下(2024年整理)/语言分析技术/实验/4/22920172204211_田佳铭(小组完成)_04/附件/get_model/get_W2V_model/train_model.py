import jieba
import jieba.analyse
import logging
import os
from gensim.models import word2vec
import json
import numpy as np
import time
import pickle

#训练模型
logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)
sentences = word2vec.LineSentence('./segment.txt') 
model = word2vec.Word2Vec(sentences, hs=1,min_count=1,window=3,size=100)

#保存变量
with open('model_train.pkl', 'wb') as f:
    pickle.dump(model,f)
