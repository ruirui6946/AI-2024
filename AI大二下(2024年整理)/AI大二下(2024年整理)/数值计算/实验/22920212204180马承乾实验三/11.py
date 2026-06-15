import numpy
import scipy.integrate

#f函数定义
def f(x):
    return numpy.cos(x)*x

#龙伯格算法
def my_romberg(f, a, b, eps):
    T = []   # 复化梯形序列
    S = []   # Simpson序列
    C = []   # Cotes序列
    R = []   # Romberg序列
    h = b - a
    T.append(h * (f(a) + f(b)) / 2)
    ep = eps + 1
    m = 0
    mul = 1
    mul2 = 1
    while(ep >= eps):
        m = m + 1
        mul = mul * 4
        t = 0
        for i in range(mul2 - 1):
            t = t + f(a + (2 * (i + 1) - 1) * h / (mul2 * 2)) * h / (mul2 * 2)
        t = t + T[-1] / 2
        T.append(t)
        if m >= 1:
            S.append((mul * T[-1] - T[-2]) / (mul - 1))
        if m >= 2:
            C.append((mul * S[-1] - S[-2]) / (mul - 1))
        if m >= 3:
            R.append((mul * C[-1] - C[-2]) / (mul - 1))
        if m > 4:
            ep = numpy.fabs(10 * (R[-1] - R[-2]))
        mul2 *= 2
    return R

if __name__ == '__main__':
    a = eval(input("put in the lower bound:\n"))
    b = eval(input("put in the upper bound:\n"))
    eps = eval(input("put in the percision:\n"))
    print('my answer:', my_romberg(f, a, b, eps)[-1])
    print('correct aswer:', scipy.integrate.quad(f, a, b)[0])
