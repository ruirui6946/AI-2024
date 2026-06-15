f=open('res.txt','r',encoding='utf-8')
names=f.readlines()
for name in names:
    name=name.rstrip()
    print(len(name))
    #print('高斯'==name)