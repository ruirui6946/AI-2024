function [l,u,y,x]=mylu(a,b)
%矩阵的直接三角分解法
n=length(a);
%求l与u
u=zeros(n,n);
l=eye(n,n);
u(1,:)=a(1,:);
l(2:n,1)=a(2:n,1)/u(1,1);
for k=2:n
    u(k,k:n)=a(k,k:n)-l(k,1:(k-1))*u(1:(k-1),k:n);
    l((k+1):n,k)=(a((k+1):n,k)-l((k+1):n,1:k-1)*u(1:(k-1),k))/u(k,k);
end
l
u
%解方程ly=b
y=zeros(n,1);
y(1)=b(1);
for k=2:n
    y(k)=b(k)-l(k,1:(k-1))*y(1:(k-1));
end
y
%解ux=y
x=zeros(n,1);
x(n)=y(n)/u(n,n);
for k=(n-1):-1:1
    x(k)=(y(k)-u(k,(k+1):n)*x((k+1):n))/u(k,k);
end
x