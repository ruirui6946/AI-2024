import numpy
import scipy.integrate as si
def f(x):
    return numpy.sin(x) / x

#三点Gauss-Legendre算法
def gauss_points3(a, b, f):
    p1 = -(b - a) * 0.6 ** 0.5 / 2 + (b + a) / 2
    p2 = (b + a) / 2
    p3 = (b - a) * 0.6 ** 0.5 / 2 + (b + a) / 2
    return  ((5 * f(p1) + 8 * f(p2) + 5 * f(p3)) / 9) * (b - a) / 2

if __name__ =='__main__':
    a = 1e-32
    b = 1
    print('自编高斯求积函数结果为:', gauss_points3(a, b, f))
    area = si.quad(f, a, b)
    print('api自带函数结果为:', si.quad(f, a, b)[0])
