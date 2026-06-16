import re
import nltk
from nltk.util import unique_list
from nltk.metrics.scores import recall,precision
import pickle
from operator import itemgetter

def sim_able_path(sub_text,pinyin_list):
    #sub_text=text.split(' ')
    old_list=[]
    cur_list=[]
    for len_text in range(len(sub_text)):
        #第一个字符
        if len_text==0:
            flag=False
            for i in range(len(pinyin_list)):
                if pinyin_list[i][0]==sub_text[0]:
                    flag=True
                    for j in range(len(pinyin_list[i][1])):
                        cur_list.append((sub_text[0],pinyin_list[i][1][j]))
            if flag==False:
                error_put=[]
                return error_put
        else:
            flag=False
            old_list=cur_list
            cur_list=[]
            for i in range(len(pinyin_list)):
                if pinyin_list[i][0]==sub_text[len_text]:
                    flag=True
                    for j in range(len(pinyin_list[i][1])):
                        for k in range(len(old_list)):
                            #new_one=old_list[k]
                            new_one=[]
                            new_one.append(old_list[k])
                            new_one.append((sub_text[len_text],pinyin_list[i][1][j]))
                            #tuple(sub_text[len_text],pinyin_list[i][1][j])
                            cur_list.append(new_one)
            if flag==False:
                error_put=[]
                return error_put
    return cur_list

#读入pinyin.py
text_pinyin=[]
dicFile=open('pinyin.txt','r')
while True:
    line = dicFile.readline()
    line = line.strip('\n')
    text_pinyin.append(line)
    if not line:
        break
dicFile.close()
del(text_pinyin[-1])

num_pinyin=0

#读入训练集
symbols = set()
tags = set()
new_sentence = []
new_sentences = []

with open('word_seg.txt', encoding='utf-8-sig') as f:
    r = re.compile(r'(.+)/(.+)')
    i = 0
    while(True):
        word = f.readline().replace('\n', '')
        if not word:
            if not new_sentence:
                break
            new_sentences.append(new_sentence)
            # print(new_sentence)
            new_sentence = []
        else:
            #print(word)
            subword=word.split('/')
            if(len(subword))==1:
                continue
            symbol=subword[0]
            tag=subword[1]
            if len(symbol)<1:
                continue
            new_sentence.append((symbol[0], tag))
            if len(symbol)<=0:
                continue
            if symbol[0]>='a' and symbol[0]<='z':
                num_pinyin=num_pinyin+1
#print(new_sentences)

data=new_sentences

#构建hmm模型
tag_set = unique_list(tag for sent in data for (word,tag) in sent)
#print(len(tag_set))
symbols = unique_list(word for sent in data for (word,tag) in sent)
#print(len(symbols))

trainer = nltk.tag.HiddenMarkovModelTrainer(tag_set, symbols)

#形成tag
pinyin_list=[]
for i in range(len(symbols)):
    if symbols[i][0]<'a' or symbols[i][0]>'z':
        continue
    a_list=[]
    for j in range(len(text_pinyin)):
        if text_pinyin[j][0]==symbols[i][0]:
            sub_pinyin=text_pinyin[j].split(':')
            #a_list.append(sub_pinyin[1])
            if len(a_list)==0:
                a_list=str(sub_pinyin[1])
            else:
                a_list=str(a_list)+str(sub_pinyin[1])
    pinyin_list.append((symbols[i],a_list))
#print(pinyin_list[0])

pinyin_pro=[]
for i in range(len(pinyin_list)):
    pro_ori=[]
    for j in range(len(pinyin_list[i][1])):
        pro_ori.append(0)
    pinyin_pro.append((i,pro_ori))

#计算发射概率矩阵（直接按照出现频率来计算）
symbols = set()
tags = set()
new_sentence = []
new_sentences = []

with open('word_seg.txt', encoding='utf-8-sig') as f:
    r = re.compile(r'(.+)/(.+)')
    i = 0
    while(True):
        word = f.readline().replace('\n', '')
        if not word:
            if not new_sentence:
                break
            new_sentences.append(new_sentence)
            # print(new_sentence)
            new_sentence = []
        else:
            #print(word)
            subword=word.split('/')
            if(len(subword))==1:
                continue
            symbol=subword[0]
            tag=subword[1]
            if len(symbol)<1:
                #print(symbol)
                continue
            new_sentence.append((symbol[0], tag))
            if len(symbol)<=0:
                continue
            if symbol[0]>='a' and symbol[0]<='z':
                for i in range(len(pinyin_list)):
                    if pinyin_list[i][0]==symbol[0]:
                        for j in range(len(pinyin_list[i][1])):
                            if tag==pinyin_list[i][1][j]:
                                pinyin_pro[i][1][j]=pinyin_pro[i][1][j]+1
#print(new_sentences)

#训练
#print(len(data))
#hmm=trainer.train_supervised(data[1:int(len(data)/1000)])
hmm=trainer.train_supervised(data)
#print(hmm.evaluate(data_test))

#测试
while True:
    pinyin=input('请输入：')
    pinyin_ori=pinyin

    #exit
    if pinyin=='exit':
        break

    #空串
    if len(pinyin)==0:
        continue

    #拼音分割，并且单拼音单独处理
    #pinyin=pinyin.split(' ')
    if len(pinyin)==1:
        #需要补充代码
        #res=hmm.best_path(pinyin)
        #print(res)
        one_able=[]
        local=0
        for i in range(len(pinyin_list)):
            if pinyin[0]==pinyin_list[i][0]:
                local=i
                for j in range(len(pinyin_list[i][1])):
                    one_able.append((j,pinyin_pro[i][1][j]))
        one_able=sorted(one_able,key=itemgetter(1),reverse=True)
        #print(len(one_able))
        if len(one_able)==0:
            print(pinyin_ori)
        for i in range(len(one_able)):
            print(pinyin_list[local][1][one_able[i][0]],one_able[i][1]/num_pinyin,sep="|") 
        continue

    #获取全部可能路径
    able_list=[]
    path_list=sim_able_path(pinyin,pinyin_list)
    
    #返回空串（有非拼音），直接输出
    if len(path_list)==0:
        print(pinyin_ori)
        #print('输入错误，请重新输入')
        continue

    #计算可能性并排序    
    for i in range(len(path_list)):
        res=hmm.probability(path_list[i])
        #启用减少选项
        if res==0:
            continue
        able_list.append((i,res))
    able_list=sorted(able_list,key=itemgetter(1),reverse=True)

    #结果输出
    for i in range(len(able_list)):
        #print(path_list[able_list[i][0]])
        a_path=[]
        for j in range(len(path_list[able_list[i][0]])):
            a_path.append(path_list[able_list[i][0]][j][1])
        a_path=''.join(a_path)
        print(a_path,able_list[i][1],sep="|")
        
