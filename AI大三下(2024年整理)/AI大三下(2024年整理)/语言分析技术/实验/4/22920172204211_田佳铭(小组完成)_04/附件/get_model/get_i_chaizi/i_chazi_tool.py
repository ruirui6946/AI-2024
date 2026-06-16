#从filename中读取拆字倒映射表
def i_chaizi_get(filename):
    i_chaizi=[]
    with open("i_chazi.txt",'r',encoding='utf-8') as f:
        line=f.readline()
        while line:
            text=line.split()
            #print(text)
            i_chaizi.append(text)
            line=f.readline()
    return i_chaizi

#查询x的倒映射结果，y是倒映射表
def iquery(x,y):
    text=[]
    for i in range(len(y)):
        #print(y[i][0])
        if x==y[i][0]:
            text.append(y[i][1])
    return text

i_chaizi=i_chaizi_get("i_chazi.txt")
#print(i_chaizi[0])
#print(len(i_chaizi))
print(iquery("禾斗",i_chaizi))
