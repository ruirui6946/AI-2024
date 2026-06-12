#多项式拟合
import numpy
import math
def func(x, y, mi):
    a = numpy.zeros((mi + 1, mi + 1))
    b = numpy.zeros((mi + 1, 1))
    for i in range(mi + 1):
        for j in range(mi + 1):
            a[i, j] = numpy.sum(x ** (i + j))
        b[i, 0] = numpy.sum(x ** i * y)
    c = numpy.linalg.inv(a)
    c = numpy.dot(c, b)
    return numpy.transpose(c)

x = numpy.array([2,4,6,8])
y  = numpy.array([2,11,28,40])
print("自编函数升序排列系数为:", func(x, y ,1))
print("自带函数polyfit降幂排列系数为:", numpy.polyfit(x, y, 1))
