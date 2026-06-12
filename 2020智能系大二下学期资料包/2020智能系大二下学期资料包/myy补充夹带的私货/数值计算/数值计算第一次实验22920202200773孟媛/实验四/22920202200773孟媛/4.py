#三角法求解线性方程组
import numpy
from scipy.optimize import fsolve

#处理矩阵
def matrix_deal(matrix):
    l = len(matrix)
    # 创建两个大小与原矩阵相同的全零矩阵
    Matrix_L = numpy.zeros((l, l), dtype = float)
    Matrix_U = numpy.zeros((l, l), dtype = float)
    # 将左矩阵对角线元素变为零 将右矩阵首行元素赋值 左矩阵首列赋值
    for i in range(l):
        Matrix_L[i][i] = 1
        Matrix_U[0][i] = matrix[0][i]
        Matrix_L[i][0] = matrix[i][0] / Matrix_U[0][0]
    for i in range(1, l):
        # 对右矩阵的第i行赋值
        Matrix_U[i, i:] = matrix[i, i:] - \
            numpy.sum(Matrix_L[i, :i].reshape(i, 1) * Matrix_U[:i, i:], axis = 0)
        # 对左矩阵的第i列赋值
        if i == l - 1:
            continue
        Matrix_L[i + 1:, i] = (matrix[i + 1:, i]-numpy.sum(Matrix_L[i + 1:, :i]
                             * Matrix_U[:i, i].reshape(1, i), axis = 0))/Matrix_U[i][i]
    # 返回左右矩阵
    return Matrix_L, Matrix_U

#计算线性方程组的解
def answer(Matrix_L, Matrix_U, matrix_an):
    # 求得 L * y = B 的解
    Matrix_L = numpy.hstack([Matrix_L, matrix_an])
    m, n = Matrix_L.shape
    for i in range(n - 1):
        Matrix_L[i + 1:, :] = Matrix_L[i + 1:, :] - \
            Matrix_L[i]*Matrix_L[i + 1:, i].reshape(m - i - 1, 1)
    # 求得 U * x = y 的解
    matrix_an = Matrix_L[:, -1].reshape(m, 1)
    Matrix_U = numpy.hstack([Matrix_U, matrix_an])
    for i in range(m - 1):
        i = m - i - 1
        Matrix_U[:i, :] = Matrix_U[:i, :] - Matrix_U[i] * \
            Matrix_U[:i, i].reshape(i, 1) / Matrix_U[i][i]
    for i in range(m):
        Matrix_U[i][-1] = Matrix_U[i][-1] / Matrix_U[i][i]
        Matrix_U[i][i] = 1
    return Matrix_U[:, -1].reshape(m, 1)

if __name__ == '__main__':
    A = numpy.array([2, 1, 5, 4, 1, 12, -2, -4, 5],
                        dtype = float).reshape(3, 3)
    b = numpy.array([11, 27, 12], dtype=float).reshape(3, 1)
    print('原增广矩阵为:')
    print(numpy.hstack([A, b]))
    Matrix_L, Matrix_U = matrix_deal(A)
    matrix_a = answer(Matrix_L, Matrix_U, b)
    print('L为:')
    print(Matrix_L)
    print('U为:')
    print(Matrix_U)
    print('自编函数求得的解集为:')
    print(matrix_a)
    print("自带函数求得的解集为:\n", numpy.linalg.solve(A, b))
