function s=mysim(fname,a,b,n)
%复化辛普生求积
%s=mysim(fname,a,b,n)
%fname为被积函数，a为上界，b为下界，n为等分数
%函数返回积分结果
h=(b-a)/n;
fa=feval(fname,a);
fb=feval(fname,b);
s=fa-fb;
x=a;
for i=1:n
    x=x+h/2;s=s+4*feval(fname,x);
    x=x+h/2;s=s+2*feval(fname,x);
end
s=s*h/6;
end