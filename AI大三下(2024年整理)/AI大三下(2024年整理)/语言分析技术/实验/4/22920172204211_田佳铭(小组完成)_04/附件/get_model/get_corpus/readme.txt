文件介绍：
bigram_model.py : 获取英语的二元语言模型
corpus.txt：英文语料库，这里使用“哈利波特小说第一部”

函数介绍：
frequency(symbol,corpus)：实现一个计算单字母和双字母（因为只做到二元语音模型）频数的函数
condition_prob(w1,w2,corpus)：实现一个计算条件概率公式
mul(x, y)：两数相乘
testing(word,corpus)：公式实现
load_corpus(file_path)：获得corpus字典的函数

运行：
直接运行bigram_model.py即可