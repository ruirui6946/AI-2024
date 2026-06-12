function f=mylag(x0,y0,x)
%拉格朗日插值
%x=mylag(x0,y0,x)
%x0为节点向量，y0为节点上的函数值
%x为插值节点，返回插值
n=length(x0);
m=length(x);
s=0.0;
for k=1:n
    p=1.0;
    for j=1:n
        if j~=k
            p=p*(x-x0(j))/(x0(k)-x0(j));
        end
    end
    s=p*y0(k)+s;
end
f=s;
end