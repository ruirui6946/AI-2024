import numpy as np
from scipy.linalg import solve

# # 例1
# A = np.array([[12,-3,3],[-18,3,-1],[1,1,1]])
# b = np.array([15,-15,6]).T
# 例2
A = np.array([[5,5327],[5327,6952099]])
b = np.array([271.4,369321.5]).T

# 求解方程组
x = solve(A, b)
print('the root from scipy:\n',x)

def myself(L,U,b):
    # 解Ly = b
    row = len(b)
    y = np.zeros(row)
    y[0] = b[0]/L[0,0]
    for k in range(1,row):
        y[k] = (b[k] - np.sum(L[k,:k]*y[:k]))/L[k,k]
    # 解Ux = y
    x = np.zeros(row)
    k = row-1
    x[k] = y[k]/U[k,k]
    for k in range(row-2,-1,-1):
        x[k] = (y[k] - np.sum(x[k+1:]*U[k,k+1:]))/U[k,k]    
    return x

def Dolittle(A):
    row = A.shape[0]
    L = np.zeros((row,row))
    U = np.zeros((row,row))

    U[0,:] = A[0,:]
    for i in range(row):
        L[i,i] = 1
        L[i,0] = A[i,0] / U[0,0]
    for i in range(1,row):
        for j in range(i,row):
            U[i,j] = A[i,j] - np.dot(L[i,:i],U[:i,j])
            if(j+1<row):
                L[j+1,i] = (A[j+1,i] - np.dot(L[j+1,:i],U[:i,i]))/U[i,i]
    return L,U

L,U=Dolittle(A)
print('the root from scipy:\n',myself(L,U,b))