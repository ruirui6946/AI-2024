function t=mytrapz(fname,a,b,n)
%复化梯形求积
%r=mytrapz(fname,a,b,n)
%fname为被积函数，a为上界，b为下界，n为等分数
%函数返回积分结果
h=(b-a)/n;
fa=feval(fname,a);
fb=feval(fname,b);
f=feval(fname,a+h:h:b-h+0.001*h);
t=h*(0.5*(fa+fb)+sum(f));
end