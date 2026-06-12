function x=myjacobi(a,b,e,N)
%雅克比迭代法求解线性方程组
%x=myjacobi(a,b,e,N)
%a为系数矩阵，b为右端向量
%e为精度，不小于1e-6，N为最大迭代次数，不大于100
%默认初始向量为零向量
%返回迭代后的解向量
n=length(b);
if N>100
    N=100;
end
if e<1e-6
    e=1e-6;
end
x0=zeros(n,1);
x=x0;
x0=x+2*e;
k=0;
d=diag(diag(a));
l=-tril(a,-1);
u=-triu(a,1);
while norm(x0-x,inf)>e & k<N
    k=k+1;
    x0=x;
    x=inv(d)*(l+u)*x+inv(d)*b;
    k
    disp(x)
end
if k==N warning('已达最大迭代系数仍未找合适的到解');end
end