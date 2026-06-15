import time
extract_relationship = {}
real_relationship = {}
extract_core_person = []
real_core_person = []
all_people = 2053
def get_real_core_person():
    global real_core_person
    with open('./really_work/correct/core_person.txt','r',encoding='utf8') as f:
        lines = f.readlines()
        for i in range(len(lines)):
            if i == 0 or i == 1:
                continue
            name_and_freq = lines[i].split(' ')
            real_core_person.append(name_and_freq[0])

def get_real_relationship():
    global real_relationship
    with open('./really_work/correct/correct_relationship.txt','r',encoding='utf8') as f:
        lines = f.readlines()
        for i in lines:
            i = i.replace('\n','')
            i = i.split(' ')
            name1_and_name2 = i
            name1 = name1_and_name2[0]
            name2 = name1_and_name2[1]
            if real_relationship.get(name1) == None:
                real_relationship[name1] = [name2]
            elif name2 not in real_relationship[name1]:
                real_relationship[name1].append(name2)
                        
            if real_relationship.get(name2) == None:
                real_relationship[name2] = [name1]
            elif name2 not in real_relationship[name2]:
                real_relationship[name2].append(name1) 
    # id=0
    # for i,j in extract_relationship.items():
    #     id+=1
    #     print(i+':')
    #     for k in j:
    #         print(k)
    #     print('over')
    #     if id==5:
    #         return 
       
def get_extract_core_person():
    global extract_core_person
    with open('./really_work/output/sorted.txt','r',encoding='utf8') as f:
        lines = f.readlines()
        for i in range(len(lines)):
            if i == 0:
                continue
            name_and_score = lines[i].split(' ')
            extract_core_person.append(name_and_score[0])

def get_extract_relationship():
    global extract_relationship
    with open('./really_work/output/edge.txt','r',encoding='gbk') as f:
        lines = f.readlines()
        for i in range(len(lines)):
            if i == 0:
                continue
            line = lines[i]
            line = line.replace('\n','').split(' ')
            name1, name2 = line[0], line[1]
            if extract_relationship.get(name1) == None:
                extract_relationship[name1] = [name2]
            elif name2 not in extract_relationship[name1]:
                extract_relationship[name1].append(name2)
                        
            if extract_relationship.get(name2) == None:
                extract_relationship[name2] = [name1]
            elif name2 not in extract_relationship[name2]:
                extract_relationship[name2].append(name1)    
    # id=0
    # for i,j in extract_relationship.items():
    #     id+=1
    #     print(i+':')
    #     for k in j:
    #         print(k)
    #     print('over')
    #     if id==5:
    #         return 

def compute_core_people():
    tp, tn, fp, fn = 0, 0, 0, 0
    global extract_core_person,real_core_person,all_people
    for i in extract_core_person:
        if i in real_core_person:
            tp+=1
        else:
            fp+=1
    for i in real_core_person:
        if i not in extract_core_person:
            fn+=1
    #一共all_people人，200个是核心人物，all_people-200=fn+tn是被认为不是核心人物
    tn = all_people-200-fn
    # print(tp, tn, fp, fn)
    return tp, tn, fp, fn

def compute_realationship():
    tp, tn, fp, fn = 0, 0, 0, 0
    global extract_relationship,real_relationship,all_people
    num_relationshiped=0 #判断为有关的关系
    for i,j in extract_relationship.items():
        num_relationshiped+=len(j)
        for k in j:
            if (i in real_relationship) and (k in real_relationship[i]):
                tp+=1
            else:
                fp+=1
    for i,j in real_relationship.items():
        for k in j:
            if (i not in extract_relationship) or (k not in extract_relationship[i]):
                fn+=1
    #一共all_people*all_people*2段关系，判断为无关的fn+tn=2*all_people*all_people-num_relationshiped
    tn=all_people*all_people*2-num_relationshiped-fn
    # print(tp, tn, fp, fn)
    return tp, tn, fp, fn 
                

if (__name__ == '__main__'):
    start = time.time()
    get_real_core_person()
    get_real_relationship()
    get_extract_core_person()
    get_extract_relationship()
    tp1, tn1, fp1, fn1 = compute_core_people()
    print(f'核心人物的准确率为:{(tp1+tn1)/(tp1+tn1+fp1+fn1)*100:2.2f}%')
    print(f'核心人物的召回率:{tp1/(tp1+fn1)*100:2.2f}%')
    tp2, tn2, fp2, fn2 = compute_realationship()
    print(f'人物关系的精确率:{(tp2+tn2)/(tp2+tn2+fp2+fn2)*100:2.2f}%')
    print(f'人物关系的召回率:{tp2/(tp2+fn2)*100:2.2f}%')
    end = time.time()
    run_time = end-start
    print(f'程序运行时间为:{run_time:.2f} 秒')