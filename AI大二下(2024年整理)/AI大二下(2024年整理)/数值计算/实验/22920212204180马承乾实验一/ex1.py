from scipy import optimize
import math
# # 例1
# def f(x):
#     return math.sin(x)-x*x/4
# 例2
def f(x):
    return pow(math.cos(x),3)*pow(x,3)
print('please put in the range:')
a,b=map(float,input().split())
while f(a)*f(b) > 0:
    print("please put in again:")
    a,b=map(float,input().split())
root = optimize.bisect(f, a, b)
print('the root from scipy:\n',root)

e=float(input('please put in the accuracy:\n'))
k=0
while abs(f(b)-f(a))>2*e:
    k+=1
    mid=(b+a)/2
    if f(mid)*f(a)<0:
        b=mid
        root=b
    elif f(mid)*f(b)<0:
        a=mid
        root=a
print('the root from myself:\n',root)
print('the times spent:',k)