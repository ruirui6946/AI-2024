function r=myrbg(fname,a,b,e)
%龙贝格求积
%r=myrbg(fname,a,b,e)
%fname为被积函数，a为上界，b为下界，e为精度，不小于1e-6
%函数展示积分过程，返回积分结果
if e<1e-6
    e=1e-6;
end
i=1;
j=1;
h=b-a;
T(i,1)=h/2*(feval(fname,a)+feval(fname,b));
T(i+1,1)=T(i,1)/2+sum(feval(fname,a+h/2:h:b-h/2+0.001*h))*h/2;
T(i+1,j+1)=4^j*T(i+1,j)/(4^j-1)-T(i,j)/(4^j-1);
while abs(T(i+1,i+1)-T(i,i))>e
    i=i+1;
    h=h/2;
    T(i+1,1)=T(i,1)/2+sum(feval(fname,a+h/2:h:b-h/2+0.001*h))*h/2;
    for j=1:i
        T(i+1,j+1)=4^j*T(i+1,j)/(4^j-1)-T(i,j)/(4^j-1);
    end
end
T
r=T(i+1,j+1);
end