import jieba
import jieba.analyse
import logging
import os
from gensim.models import word2vec
import json
import numpy as np
import time

class MyEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, np.integer):
            return int(obj)
        elif isinstance(obj, np.floating):
            return float(obj)
        elif isinstance(obj, np.ndarray):
            return obj.tolist()
        if isinstance(obj, time):
            return obj.__str__()
        else:
            return super(NpEncoder, self).default(obj)

#分词
with open('./in_the_name_of_people2.txt',encoding='utf-8') as f:
    document = f.read()
    
    #document_decode = document.decode('GBK')
    
    document_cut = jieba.cut(document)
    #print  ' '.join(jieba_cut)  #如果打印结果，则分词效果消失，后面的result无法显示
    result = ' '.join(document_cut)
    #print(result)
    #result = result.encode('utf-8')
    #print(result)
with open('./in_the_name_of_people_segment.txt', 'w',encoding='utf-8') as f2:
    f2.write(result)

#训练模型
logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)
sentences = word2vec.LineSentence('./in_the_name_of_people_segment.txt') 
model = word2vec.Word2Vec(sentences, hs=1,min_count=1,window=3,size=100)

#filename = "test1.json"
#with open(filename, "w", encoding="utf-8") as f:
#    tmp=json.dumps(model,cls=MyEncoder)
#    #json.dump(tmp, f, ensure_ascii=False)
#    json.dump(tmp, cls=MyEncoder) 

#计算与一个词最相关的词的集合
req_count = 5 #集合元素数
#for key in model.wv.similar_by_word('炸酱面'.decode('utf-8'), topn =100):
for key in model.wv.similar_by_word('炸酱面', topn =100):
    if len(key[0])==3:
        req_count -= 1
        print(key[0], key[1])
        if req_count == 0:
            break;

x1="炸酱面"
x2="赵德汉"
sim1=model.wv.similarity(x1,x2)
print(sim1)

