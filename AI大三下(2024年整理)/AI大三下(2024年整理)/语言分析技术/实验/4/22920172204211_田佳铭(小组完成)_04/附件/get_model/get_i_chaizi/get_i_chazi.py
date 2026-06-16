import re

characters = [] # 创建一个列表用于保存常用汉字字符
for i in range(176, 216):
    s = bytes([i])
    for x in range(161, 255):
        s += bytes([x])
        try:
            c = s.decode("gb2312")
        except:
            break
        characters.append(c)
        #print(c, end="\t") # 打印结果
        s = bytes([i])

#读入拆字词典
ori_chaizi=[]
with open("chaizi-jt.txt",'r',encoding='utf-8') as f: 
    line = f.readline()   
    while line:
        if line[0] not in characters:#非常用汉字字符不处理
            line = f.readline()
            continue
        #找到每个项
        text=re.findall(r'[\u4e00-\u9fa5](?: [\u4e00-\u9fa5])*',line)
        #去除项之间的空格
        for i in range(len(text)):
            tmp=str("")
            for j in range(len(text[i])):
                if j%2==0:
                    tmp=tmp+str(text[i][j])
            text[i]=str(tmp)
        #此时text格式为[原汉字,拆字结果1,拆字结果2...]
        ori_chaizi.append(text)  
        line = f.readline()

#for i in range(1,len(ori_chaizi),1):
#for i in range(len(ori_chaizi)):
    #print(ori_chaizi[i])

#print(len(ori_chaizi))

#倒排输出，格式为[拆字结果，原汉字]
with open("i_chazi.txt",'w',encoding='utf-8') as f:
    for i in range(len(ori_chaizi)):
        for j in range(1,len(ori_chaizi[i]),1):
            if len(ori_chaizi[i][j])==1:
                print(ori_chaizi[i][j])
                continue
            f.write(ori_chaizi[i][j])
            f.write(" ")
            f.write(ori_chaizi[i][0])
            f.write('\n')
