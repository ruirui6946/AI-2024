import re

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

#形成tag
tag_list=[]
for i in range(len(text_pinyin)):
    sub_pinyin=text_pinyin[i].split(':')
    #print(sub_pinyin[0])
    pinyin=str(sub_pinyin[0])
    a_list=re.findall(r'[\u4e00-\u9fa5]',sub_pinyin[1])
    for j in range(len(a_list)):
        tup=(pinyin,str(a_list[j]))
        #print(type(tup))
        tag_list.append(tup)

#读入pinyin_train.txt
text_train=[]
dicFile=open('pinyin_test.txt','r')
while True:
    line = dicFile.readline()
    line = line.strip('\n')
    text_train.append(line)
    if not line:
        break
dicFile.close()
del(text_train[-1])

#建立词标注文件
#file=open('word_seg_test.txt','a',encoding='utf-8-sig')
file=open('word_seg_test.txt','a')
#for i in range(10):
for i in range(len(text_train)):
    for j in range(len(text_train[i])):
        flag=0
        for m in range(len(tag_list)):
            if tag_list[m][1]==text_train[i][j]:
                flag=1
                #print(tag_list[m][0]+'/'+text_train[i][j])
                file.write(tag_list[m][0]+'/'+text_train[i][j])
                file.write('\n')
                break
        if flag==0:
            #print(text_train[i][j]+'/'+text_train[i][j])
            file.write(text_train[i][j]+'/'+text_train[i][j])
            file.write('\n')
    #print('\n')
    file.write('\n')
#file.write('hello')
file.close()
