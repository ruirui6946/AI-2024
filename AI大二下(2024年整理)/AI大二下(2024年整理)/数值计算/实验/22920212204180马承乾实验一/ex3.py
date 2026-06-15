import numpy as np
from scipy.linalg import solve
# # 例1
# A = np.array([[12,-3,3],[-18,3,-1],[1,1,1]])
# b = np.array([15,-15,6]).T
# 例2
A = np.array([[1,4,3],[0,2,2],[1,4,4]])
b = np.array([9,4,10]).T
x = solve(A, b)
print('the root from scipy:\n',x)

def myself(A, b):
    row, col = A.shape #记录未知数个数
    ax = np.zeros((col,col))
    for k in range(col - 1):
        temp = 0#在每次计算前，找到最大主元，进行换行
        for i in range(k, col):
            if temp < np.fabs(A[i][k]):
                temp = A[i][k]
                maxn = i
        A[[k, maxn], :] = A[[maxn, k], :]
        b[k], b[maxn] = b[maxn], b[k]
        #计算
        for i in range(k + 1, col):
            ax[i][k] = A[i][k] / A[k][k] 
            for j in range(row): 
                A[i][j] = A[i][j] - ax[i][k] * A[k][j]
            b[i] = b[i] - ax[i][k] * b[k]

    res = np.zeros(col)
    res[col - 1] = b[col - 1] / A[col - 1][col - 1] #求解

    for i in range(col - 2, -1, -1): #回代
        for j in range(i + 1, col):
            b[i] -= A[i][j] * res[j]
        res[i] = b[i] / A[i][i]
    return res
    
print('the root from myself:\n',myself(A,b))
#例子4.2.5