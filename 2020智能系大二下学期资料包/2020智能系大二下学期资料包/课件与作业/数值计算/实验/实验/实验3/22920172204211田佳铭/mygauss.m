function x=mygauss(a,b)
%列主元高斯消去法解线性方程组ax=b
n=length(b);
a=[a b];
for k=1:(n-1)
    %选取列主元
    [ar,r]=max(abs(a(k:n,k)));
    r=r+k-1;
    if r>k
        t=a(k,:);
        a(k,:)=a(r,:);
        a(r,:)=t;
    end
    %消元
    a((k+1):n,(k+1):(n+1))=a((k+1):n,(k+1):(n+1))-a((k+1):n,k)/a(k,k)*a(k,(k+1):(n+1));
    a((k+1):n,k)=zeros(n-k,1);
    a
end
%回带
x=zeros(n,1);
x(n)=a(n,n+1)/a(n,n);
for k=n-1:-1:1
  x(k,:)=(a(k,n+1)-a(k,(k+1):n)*x((k+1):n))/a(k,k);
end
end