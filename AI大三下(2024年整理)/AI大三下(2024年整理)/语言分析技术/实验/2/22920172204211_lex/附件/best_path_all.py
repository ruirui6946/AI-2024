import re
import nltk
from nltk.util import unique_list
from nltk.metrics.scores import recall,precision
import pickle

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
            #symbol = r.search(word).group(1)
            #tag = r.search(word).group(2)
            symbol=subword[0]
            tag=subword[1]
            symbols.add(symbol)
            tags.add(tag)
            new_sentence.append((symbol, tag))
#print(new_sentences)

data=new_sentences

#读入测试集
symbols = set()
tags = set()
new_sentence = []
new_sentences = []

with open('word_seg_test.txt') as f:
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
            #symbol = r.search(word).group(1)
            #tag = r.search(word).group(2)
            symbol=subword[0]
            tag=subword[1]
            symbols.add(symbol)
            tags.add(tag)
            new_sentence.append((symbol, tag))

data_test=new_sentences

#构建hmm模型
tag_set = unique_list(tag for sent in data for (word,tag) in sent)
#print(len(tag_set))
symbols = unique_list(word for sent in data for (word,tag) in sent)
#print(len(symbols))

trainer = nltk.tag.HiddenMarkovModelTrainer(tag_set, symbols)

#准确率
print(len(data))
#hmm=trainer.train_supervised(data[1:int(len(data)/1000)])
hmm=trainer.train_supervised(data)
print(hmm.evaluate(data_test))

#输入、预测
while True:
    pinyin=input('请输入：')
    if pinyin=='exit':
        break
    sub_pinyin=pinyin.split(' ')
    path=hmm.best_path(sub_pinyin)
    print(path)
