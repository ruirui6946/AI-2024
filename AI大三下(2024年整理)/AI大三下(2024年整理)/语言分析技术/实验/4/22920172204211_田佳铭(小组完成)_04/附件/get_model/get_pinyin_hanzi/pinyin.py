import pypinyin
import pickle

from pypinyin import lazy_pinyin

#-------------------------训练部分------------------------------
characters = [] # 创建一个列表用于保存常用汉字字符
pinyin = {}
#获取常用字
for i in range(176, 216):
    s = bytes([i])
    for x in range(161, 255):
        s += bytes([x])
        try:
            c = s.decode("gb2312")
        except:
            break
        characters.append(c)
        temp=lazy_pinyin(c)[0]
        if temp in pinyin :
            pinyin[temp].append(c)
        else :
            pinyin[temp] = []
            pinyin[temp].append(c)
        # print(c, end="\t") # 打印结果
        # print(lazy_pinyin(c))
        s = bytes([i])

# 函数：获取常用词拼音不带声调的(style=pypinyin.NORMAL)
def cipinyin(word):
    s = ''
    for i in pypinyin.pinyin(word, style=pypinyin.NORMAL):
        s += ''.join(i)
    return s

#获取常用词拼音
for line in open("out.txt"):
    linetemp =str(line.strip('\n'))
    temppinyin=cipinyin(linetemp)
    # print(linetemp)
    # print(temppinyin)
    if temppinyin in pinyin:
        pinyin[temppinyin].append(linetemp)
    else:
        pinyin[temppinyin] = []
        pinyin[temppinyin].append(linetemp)


for line in  open('jianxie.txt','r',encoding='utf-8') :
    line =str(line.strip('\n'))
    text = line.split()
    #print(text)
    if text[0] in pinyin :
        pinyin[text[0]].append(text[1])
    else :
        pinyin[text[0]] = []
        pinyin[text[0]].append(text[1])


#a ['啊', '阿']
# ai ['埃', '挨', '哎', '唉', '哀', '皑', '癌', '蔼', '矮', '艾', '碍', '爱', '隘']
# an ['鞍', '氨', '安', '俺', '按', '暗', '岸', '胺', '案']
# shenmewenti ['什么问题']
# for i in pinyin:
#     print(i,pinyin[i])

#存储
with open('pinyin_hanzi.pkl', 'wb') as f:
    pickle.dump(pinyin, f)


#---------------------------以下是调用部分--------------------

#函数：查询x的倒映射结果，y是倒映射表
def py2hz(x,dict):
    if x in dict:
        return pinyin[x]
    else :
        return []

#测试主程序
pkl_file = open('pinyin_hanzi.pkl', 'rb')
dict = pickle.load(pkl_file)
print(py2hz("JS",dict))
print(py2hz("ziliaoxiazai",dict))