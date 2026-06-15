function x=my_jacobi(a,b,e,N)
%雅克比迭代法求解线性方程组
%a为系数矩阵，b为右端向量
%e为精度，N为最大迭代次数
%返回迭代后的解向量
n=length(b);
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
if k==N warning('超过最大迭代次数！仍然不能求出满足精度要求的解');end
end