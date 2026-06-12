# 高斯-赛德尔迭代法求解线性方程组
import numpy
def GS(A, b, x, e, times):
    x0 = numpy.linalg.solve(A, b)
    D = numpy.mat(numpy.diag(numpy.diag(A)))
    L = -1*numpy.triu(A, 1)
    U = -1*numpy.tril(A, -1)
    H = (D-L).I*U
    k = 0
    while k < times:
        if max(abs(x-x0).A1) > e:
            x = H*x + (D-L).I*b
            k += 1
        else:
            print('当精度为', e, '时,G-S迭代法在%d次内收敛' % k)
            break
    print('G-S迭代解为:\n', x)

A = numpy.mat([[-10, 1, -2], [1, 10, -2], [-1, 1, 5]])
b = numpy.mat('72;-83;42')
x = numpy.mat('0;0;0')
e = 1e-6
times = 100
GS(A, b, x, e, times)
print("自带函数求得的解集为:\n", numpy.linalg.solve(A, b))
