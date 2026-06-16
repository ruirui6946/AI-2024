from pypinyin import pinyin
import unicodedata

def is_Chinese(word):
    for ch in word:
        if '\u4e00' <= ch <= '\u9fff':
            return True
    return False

#读入pinyin_train.txt/pinyin_test.txt
text_train=[]
#dicFile=open('pinyin_train.txt','r',encoding='utf-8-sig')
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
print(len(text_train))
#for i in range(31691,len(text_train)):
for i in range(len(text_train)):
    #print(i)
    res=pinyin(text_train[i])
    #去音调
    k=0
    for j in range(len(res)):
        res[j]=unicodedata.normalize('NFKD', str(res[j])).encode('ascii','ignore')
        res[j]=res[j].decode()
        res[j]=res[j][2:len(res[j])-2]
        if len(res[j])==0:
            k=k+1
            continue
        if res[j][0]<'a' or res[j][0]>'z':
            while not is_Chinese(text_train[i][k]):
                file.write(text_train[i][k]+'/'+text_train[i][k]+'\n')
                k=k+1
                if k==len(text_train[i]):
                    break
            continue
        file.write(res[j]+'/'+text_train[i][k])
        file.write('\n')
        k=k+1
    file.write('\n')
file.close()
