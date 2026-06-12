#拉格朗日插值法求插值函数
import math

from numpy import interp
def lagrange(x0, y0, x):
    l = len(x0)
    ans = 0.000
    for i in range(l):
        p = 1.000
        for j in range(l):
            if (i != j):
                p = p * (x - x0[j]) / (x0[i] - x0[j])
        ans += p * y0[i]
    return ans

x0 = [100, 121, 144]
y0 = [10, 11, 12]
f = lagrange(x0, y0, 115)
print("拉格朗日插值为:", f)
print("精确解为:", math.sqrt(115))