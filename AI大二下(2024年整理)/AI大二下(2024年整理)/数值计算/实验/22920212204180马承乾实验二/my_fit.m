function p=my_fit(x,y,m)
%函数逼近的多项式拟合
%x，y为数据向量，m为多项式的次数
%函数返回多项式的系数
A=zeros(m+1,m+1);
for i=0:m
    for j=0:m
        A(i+1,j+1)=sum(x.^(i+j));
    end
    b(i+1)=sum(x.^i.*y);
end
    c=A\b';
    p=c';
end