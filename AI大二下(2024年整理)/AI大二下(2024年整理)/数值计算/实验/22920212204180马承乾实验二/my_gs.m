function x=my_gs(a,b,e,N)
%高斯-赛德尔迭代法求解线性方程组
%a为系数矩阵，b为右端向量，e为精度，N为最大迭代次数
%返回迭代后的解向量，若不收敛，则输出警告信息
n=length(b);
x0=zeros(n,1);
x=x0;
x0=x+2*e;
k=0;
a1=tril(a);
a2=inv(a1);
while norm(x0-x,inf)>e & k<N
    k=k+1;
    x0=x;
    x=-a2*(a-a1)*x0+a2*b;
    k
    disp(x)
end
if k==N warning('超过最大迭代次数！仍然不能求出满足精度要求的解');end
end