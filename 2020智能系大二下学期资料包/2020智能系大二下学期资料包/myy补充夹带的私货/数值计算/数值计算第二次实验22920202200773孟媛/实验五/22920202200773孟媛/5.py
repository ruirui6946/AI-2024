# 雅可比迭代法求解线性方程组
import numpy
def Jacobi(A, b, x, e, times=100):
    x0 = numpy.linalg.solve(A, b)
    length, width = numpy.shape(A)
    D = numpy.mat(numpy.diag(numpy.diag(A)))
    L = -1*numpy.triu(A, 1)
    U = -1*numpy.tril(A, -1)
    H = numpy.eye(length)-D.I*A
    eig, _ = numpy.linalg.eig(H)
    spectral_radius = max(abs(eig))
    if spectral_radius < 1:
        print('此方程组收敛,谱半径为', round(spectral_radius, 5))
        k = 0
        while k < times:
            if max(abs(x-x0).A1) > e:
                x = H*x + D.I*b
                k += 1
            else:
                print('当精度为', e, '时,Jacobi在%d次内收敛' % k)
                break
        print('自编函数Jacobi迭代解为:\n', x)
    else:
        print('Jacobi迭代法不收敛,谱半径为', round(spectral_radius, 5))


A = numpy.mat([[10, -1, -2], [-1, 10, -2], [-1, -1, 5]])
b = numpy.mat('72;83;42')
x = numpy.mat('0;0;0')
e = 1e-6
times = 100
Jacobi(A, b, x, e, times)
print("自带函数求得的解集为:\n", numpy.linalg.solve(A, b))
