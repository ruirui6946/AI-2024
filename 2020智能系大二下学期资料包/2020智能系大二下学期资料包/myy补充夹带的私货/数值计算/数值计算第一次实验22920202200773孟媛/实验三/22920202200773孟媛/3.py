#高斯列主元消去法求线性方程组的解
import numpy
from scipy.optimize import fsolve

def fgauss(A, b):
    n = A.shape[0]
    m_U = numpy.hstack((A, b.T)) #增广矩阵
    ra = numpy.linalg.matrix_rank(A) #系数矩阵的秩
    rb = numpy.linalg.matrix_rank(m_U) #增广矩阵的秩
    if rb - ra > 0:
        print("系数矩阵与增广矩阵秩不同,因而无解")
        return
    if ra == rb:
        if ra == n:
            x = numpy.mat(numpy.zeros(m_U.shape[0], dtype=float))
            for p in range(0, n):
                print('此次行变换前增广矩阵为:')
                print(m_U)
                j = (abs(m_U[p:n, p])).argmax()
                print(f'第{p + 1}行与第{j + p + 1}行交换')
                temp = m_U[p, :].copy()
                m_U[p, :] = m_U[j + p, :]
                m_U[j + p, :] = temp
                print('行变换后的增广矩阵为:')
                print(m_U)
                print()
                for k in range(p + 1, n):
                    m = m_U[k, p] / m_U[p, p]
                    m_U[k, p:n + 1] = m_U[k, p:n + 1] - m * m_U[p, p:n + 1]
            b1 = m_U[0:n, n]
            a1 = m_U[0:n, 0:n]
            print('增广矩阵为')
            print(m_U)
            x[0, n-1] = b1[n-1]/a1[n-1, n-1]
            for i in range(n - 2, -1, -1):
                try:
                    x[0, i] = (b1[i] - numpy.sum(numpy.multiply(a1[i, i + 1:n], x[0, i + 1:n]))) / (a1[i, i])
                except:
                    print("异常错误!")
            print('自编函数求得线性方程组的解为：')
            print(x)

if __name__ == '__main__':
    A = numpy.mat([[12, -3, 3],
                [-18, 3, -1],
                [1, 1, 1]],
                dtype=float)
    b = numpy.mat([15, -15, 6])
    fgauss(A, b)
    A = numpy.array([12, -3, 3, -18, 3, -1, 1, 1, 1],
                    dtype = float).reshape(3, 3)
    b = numpy.array([15, -15, 6])
    print("自带函数求得方程组的解为:\n", numpy.linalg.solve(A, b))
