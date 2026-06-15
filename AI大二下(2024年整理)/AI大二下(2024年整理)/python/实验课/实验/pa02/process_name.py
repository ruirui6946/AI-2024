import random
f = open('names.txt','r',encoding='utf-8')
names = f.readlines()
names = set(names)
names = list(names)
random.shuffle(names)
file = open('res.txt','w',encoding='utf-8')
for name in names:
    file.write(name)