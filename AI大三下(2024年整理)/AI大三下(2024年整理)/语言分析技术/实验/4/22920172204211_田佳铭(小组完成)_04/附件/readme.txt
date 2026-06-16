中间代码文件：
langconv.py、zh_wiki.py：和繁体转简体有关的中间代码

模型文件：
（1）i_chazi.txt：拆字倒映射模型文件
（2）model_train.pkl：Word2Vec模型
（3）pinyin_hanzi.pkl：拼音-汉字常用词映射
（4）pinyinLib.txt：本地拼音库
（5）规范词典2000条.txt：实验本身就有的规范词典
（6）corpus.pkl：英语二元语言模型

测试文件：
（1）testdata.txt：一个测试文件
（2）规范化测试集1000条.txt：实验本身的测试集

主函数文件：
（1）client.py：实现实验要求的文件
（2）test.py：测试识别率的文件

文件夹：
get_model：获取各种模型的文件夹