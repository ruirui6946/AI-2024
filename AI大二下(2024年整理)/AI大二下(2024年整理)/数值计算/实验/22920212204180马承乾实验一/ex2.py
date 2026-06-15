import time
import math
N=500#最大迭代次数
x0=float(input("please put in the original value\n"))
e=float(input('please put in the accuracy\n'))
# # 例1
# def f(x):
#     return pow(x,3)-x-1
# def df(x):
#     return 3*pow(x,2)-1
# 例2
def f(x):
    return pow(math.e,x)*math.sin(pow(x,8))
def df(x):
    return pow(math.e,x)*(math.sin(pow(x,8))+8*pow(x,7)*math.cos(pow(x,8)))
start=time.time()
k=0
x=x0+2*e
while abs(x-x0)>e and k<N:
    x0=x
    x=x0-f(x0)/df(x0)
    k+=1
end=time.time()
print('the root from myself:\n',x)
print('time spent:',end-start)
start=time.time()
from scipy import optimize
root = optimize.newton(f, 1.5, fprime2=df)
print('the root from scipy:\n',root)
end=time.time()
print('time spent',end-start,'s')