import numpy
import scipy.integrate

def f(x):
    return numpy.cos(x)*x

def my_quad(f, a, b, n):#n等分，区间[a,b]
    if type(f) is numpy.ndarray:
        y = 2 * numpy.sum(f) - f[0] - f[-1]
        ans = ((b - a) / (f.shape[0] - 1)) * y / 2
    else:
        x = numpy.linspace(a, b, n + 1)
        y = 2 * numpy.sum(f(x)) - f(a) - f(b)
        ans = ((b - a) / n) * y / 2
    return ans

if __name__ =='__main__':
    a = eval(input("put in the lower bound:\n"))
    b = eval(input("put in the upper bound:\n"))
    n = eval(input("put in the length of area:\n"))
    print('my answer:', my_quad(f, a, b, n))
    print('correct aswer:', scipy.integrate.quad(f, a, b)[0])
    