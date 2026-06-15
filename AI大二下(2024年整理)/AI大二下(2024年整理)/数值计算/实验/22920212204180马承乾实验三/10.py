import numpy
import scipy.integrate

def f(x):
    return numpy.cos(x)*x

# 复合simpson 算法
def my_simpson(f, a, b, n):
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
    a = eval(input("put in the lower bound:\n"))
    b = eval(input("put in the upper bound:\n"))
    n = eval(input("put in the length of area:\n"))
    print('my answer:', my_simpson(f, a, b, n))
    print('correct aswer:', scipy.integrate.quad(f, a, b)[0])
