#定义输入empty代表ε，用->代表→
import sys
newright=[]#全局变量

#查找子串函数
def issub(s1,s2):
    return s1 in s2

#递归空替换函数
def myadd(cur,mode,begin):
    if issub(mode,cur[begin:]):
        local=begin+cur[begin:].find(mode)
        sub1=cur
        sub2=cur[:local]+cur[local+len(mode):]
        newright.append(sub1)
        newright.append(sub2)
        myadd(sub1,mode,local+len(mode))
        myadd(sub2,mode,local)

#输入
text=input("包含ε产生式:")

#输入分割
subtext=text.split('->',1)
left=subtext[0]
lenleft=len(left)
right=subtext[1]

#无ε，直接输出
flag=0
subright=right.split('|')
for i in range(len(subright)):
    if subright[i]=='empty':
        flag=flag+1
if flag==0:
    print(text)
    sys.exit(0)

#first 文法
print(left+'\'->'+left+'|empty')

subright.remove('empty')
newright=subright
for i in range(len(subright)):
    cur=subright[i]
    myadd(cur,left,0)

newright=list(set(newright))#去重
if 'empty' in newright:
    newright.remove('empty')#去empty
if '' in newright:
    newright.remove('')#去空
#for i in range(len(newright)):
#    print(newright[i])
    
#second 文法
out='|'
outstr=out.join(newright)
print(left+"->"+outstr)
