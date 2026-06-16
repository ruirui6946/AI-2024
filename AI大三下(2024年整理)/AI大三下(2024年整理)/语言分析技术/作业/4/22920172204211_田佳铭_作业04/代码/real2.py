import re
import nltk
from nltk.util import unique_list
from nltk.metrics.scores import recall,precision

#读入语料库
symbols = set()
tags = set()
new_sentence = []
new_sentences = []

with open('ptb.txt', encoding='utf-8-sig') as f:
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
            symbol = r.search(word).group(1)
            tag = r.search(word).group(2)
            symbols.add(symbol)
            tags.add(tag)
            new_sentence.append((symbol, tag))
#print(new_sentences)

data=new_sentences

#对IN进行处理
prep=['DT','NNP','NN','CD','NNS','VBG','JJ']
prep=set(prep)
for i in range(len(data)):
    for j in range(len(data[i])):
        if data[i][j][1]=='IN':
            tmp=data[i][j][0]
            if j<len(data[i])-1 and data[i][j+1][1] in prep:
                data[i][j]=(tmp,'IN-prep')
            else:
                data[i][j]=(tmp,'IN-conj')

#构建hmm模型
tag_set = unique_list(tag for sent in data for (word,tag) in sent)
#print(len(tag_set))
#print(tag_set)
symbols = unique_list(word for sent in data for (word,tag) in sent)
#print(len(symbols))

trainer = nltk.tag.HiddenMarkovModelTrainer(tag_set, symbols)

#按9：1划分训练集和测试集
size = int(len(data) * 0.1)
train_set = data[size:]
test_set = data[:size]

#准确率
#hmm=trainer.train_supervised(train_set)
hmm=trainer.train_supervised(data) #全样本训练
print(hmm.evaluate(test_set))

#对每一句话进行预测（为了召回率做准备）
pre_result=[]
real_result=[]
right_num_sen=0
pre_sen=[]
real_sen_res=[]
for i in range(len(test_set)):
    test_sen=[]
    real_sen=[]
    for j in range(len(test_set[i])):
        test_sen.append(test_set[i][j][0])
        real_sen.append(test_set[i][j][1])
    real_result=real_result+real_sen
    path=hmm.best_path(test_sen)
    real_sen_res.append('yes')
    if path==real_sen :
        right_num_sen=right_num_sen+1
        pre_sen.append('yes')
    else :
        pre_sen.append('no')
    pre_result=pre_result+path

#召回率
recall_token=recall(set(real_result),set(pre_result))
print(recall_token)

#句子准确率
sen_ac=right_num_sen/len(pre_sen)
print(sen_ac)

#句子召回率（意义不大）
recall_sen=recall(set(real_sen_res),set(pre_sen))
print(recall_sen)
