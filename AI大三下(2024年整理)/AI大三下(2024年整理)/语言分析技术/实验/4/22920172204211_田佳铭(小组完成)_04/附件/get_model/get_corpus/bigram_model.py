import os
import re
import pickle
import math
from functools import reduce

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

#获得corpus字典的函数
def load_corpus(file_path):
    corpus = {}
    with open(file_path,'r',encoding='UTF-8') as f:
        for line in f.readlines():
            line_words = line.strip().split()
            if len(line_words) > 1:
                for word in line_words:
                    word = re.findall('[a-z]+', word.lower())
                    if len(word) > 0:
                        word = '>'+word[0]+'<'
                        if word in corpus.keys():
                            corpus[word] += 1
                        else:
                            corpus[word] = 1

    pickle.dump(corpus,open('corpus.pkl','wb'))
    return corpus

#----------------------------------测试
if __name__ == "__main__":
    if os.path.exists('corpus.pkl'):
        corpus = pickle.load(open('corpus.pkl','rb'))
    else:
        corpus = load_corpus('corpus.txt')

    test = 'Changan President Zhu Huarong said the investment will baohan a xuduo of thing'
    test = test.split()
    for test_word in test:
        test_word = test_word.lower()
        reliability = testing(test_word,corpus)
        # print(reliability)
        if reliability >= 1e-3:
            print("'%s' is a english word" % test_word)
        else:
            print("'%s' is a pinyin" % test_word)