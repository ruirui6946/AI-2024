#二分法求方程的根
import numpy
from scipy import optimize

cnt = 0 #计数二分次数

def func(x):
    return numpy.sin(x) - x * x / 4

def check(l, r, st): #参数依次为左区间,右区间,精度
    if (func(l) * func(r) > 0 or l > r):
        print("无根或输入异常!")
        exit(0)
    global cnt
    while (numpy.fabs(r - l) >= st):
        cnt += 1
        mid = (l + r) / 2
        if (func(l) * func(mid) < 0):
            r = mid
        else:
            l = mid
    return l

if __name__ == '__main__':
    l = float(input("请输入左区间，以回车结束:"))
    r = float(input("请输入右区间，以回车结束:"))
    st = float(input("请输入精度:"))
    print("自编函数求得方程的解为:", check(l, r, st), ", 二分进行了:", cnt, "次")
    print("自带函数求得方程的解为:", optimize.newton(func, x0 = l, x1 = r, tol = st))
