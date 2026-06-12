function x=mybisect(fname,a,b,tol)
%二分法求方程根
fa=feval(fname,a);
fb=feval(fname,b);
if fa*fb>0
    error('两端函数值同号，不能保证有解');
end
k=0
x=(a+b)/2
while (b-a)>(2*tol)
    fx=feval(fname,x);
    if fa*fx<0
        b=x;
        fb=fx;
    elseif fb*fx<0
        a=x;
        fa=fx;
    else
        k=k+1;
        x=(a+b)/2;
        break;
    end
    k=k+1
    x=(a+b)/2
end