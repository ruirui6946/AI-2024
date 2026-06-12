import numpy
import scipy.integrate as si

#f函数定义
def f(x):
    return numpy.sin(x) / x

# 复化 Simpson 算法
def simpson_fh(a, b, f, n):
    if type(f) is numpy.ndarray:
        f1 = f[1::2].copy()
        f2 = f[0::2].copy()
        y = 4 * numpy.sum(f1) + 2 * numpy.sum(f2) - f[0] - f[-1]
        ans = 2 * ((b - a) / (f.shape[0] - 1)) * y / 6
    else:
        x = numpy.linspace(a, b, 2 * n + 1)
        x1 = x[1::2].copy()
        x2 = x[0::2].copy()
        y = 4*numpy.sum(f(x1)) + 2 * numpy.sum(f(x2)) - f(a) - f(b)
        ans = ((b - a) / n) * y / 6
    return ans

if __name__ =='__main__':
    a = 1e-32
    b = 1
    n = 4
    print('自编复化辛普生求积函数结果为:', simpson_fh(a, b, f, n))
    area = si.quad(f, a, b)
    print('api自带函数结果为:', si.quad(f, a, b)[0])
