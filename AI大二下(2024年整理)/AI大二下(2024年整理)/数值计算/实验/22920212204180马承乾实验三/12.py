import numpy
import scipy.integrate
def f(x):
    return numpy.cos(x)*x

#Gauss-Legendre算法
def my_gauss(f, a, b):
    p1 = -(b - a) * 0.6 ** 0.5 / 2 + (b + a) / 2
    p2 = (b + a) / 2
    p3 = (b - a) * 0.6 ** 0.5 / 2 + (b + a) / 2
    return  ((5 * f(p1) + 8 * f(p2) + 5 * f(p3)) / 9) * (b - a) / 2

if __name__ =='__main__':
    a = eval(input("put in the lower bound:\n"))
    b = eval(input("put in the upper bound:\n"))
    print('my answer:', my_gauss(f, a, b))
    print('correct aswer:', scipy.integrate.quad(f, a, b)[0])
