import numpy
import scipy.integrate as si

#f函数定义
def f(x):
    return numpy.sin(x) / x

#复化梯形算法
def Comp_trape(a, b, f, n):
    if type(f) is numpy.ndarray:
        y = 2 * numpy.sum(f) - f[0] - f[-1]
        ans = ((b - a) / (f.shape[0] - 1)) * y / 2
    else:
        x = numpy.linspace(a, b, n + 1)
        y = 2 * numpy.sum(f(x)) - f(a) - f(b)
        ans = ((b - a) / n) * y / 2
    return ans

if __name__ =='__main__':
    a = 1e-32
    b = 1
    n = 8
    print('自编复化梯形求积函数结果为:', Comp_trape(a, b, f, n))
    area = si.quad(f, a, b)
    print('api自带函数结果为:', si.quad(f, a, b)[0])
    