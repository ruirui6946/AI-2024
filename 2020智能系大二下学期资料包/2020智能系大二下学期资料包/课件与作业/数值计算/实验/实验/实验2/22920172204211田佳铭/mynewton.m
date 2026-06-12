function x=mynewton(fname,dfname,x0,tol)
%牛顿法求方程根
N=100;
x=x0;
x0=x+2*tol;
k=0;
while abs(x0-x)>tol & k<N
    k=k+1
    x0=x;
    x=x0-feval(fname,x0)/feval(dfname,x0)
end
if k==N
    warning('迭代100次，累了，不再迭下去了');
end
end