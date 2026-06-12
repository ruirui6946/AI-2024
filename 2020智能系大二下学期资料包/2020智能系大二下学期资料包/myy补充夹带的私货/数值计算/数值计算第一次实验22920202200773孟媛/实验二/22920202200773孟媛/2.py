#牛顿迭代法求方程的根
import numpy
import math
from scipy import optimize

cnt = 0 #计数迭代次数

def func(x): #原函数
    return math.pow(x ,3) - x - 1

def func_(x): #导函数
    return 3 * math.pow(x, 2) - 1

def check(l, r, st): #参数依次为左区间,右区间,精度
    global cnt
    x = r
    while(numpy.fabs((func(x) / func_(x))) > st):
        cnt += 1
        x -= func(x) / func_(x)
    return x

if __name__ == '__main__':
    l = float(input("请输入左区间，以回车结束:"))
    r = float(input("请输入右区间，以回车结束:"))
    st = float(input("请输入精度:"))
    print("自编函数求得方程的解为:", check(l, r, st), ", 迭代进行了:", cnt, "次")
    print("自带函数求得方程的解为:", optimize.newton(func, x0 = l, x1 = r, tol = st))
