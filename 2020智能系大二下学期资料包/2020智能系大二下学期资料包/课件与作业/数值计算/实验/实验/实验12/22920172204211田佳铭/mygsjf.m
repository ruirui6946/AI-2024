function g=mygsjf(fname,a,b,m)
%高斯求积
%g=mygsjf(fname,a,b,m)
%fname为被积函数，a为上界，b为下界，m为高斯点数
%函数返回积分结果
switch m
    case 1
        t=0;A=2;
    case 2
        t=[-1/sqrt(3),1/sqrt(3)];A=[1,1];
    case 3
        t=[-sqrt(0.6),0,sqrt(0.6)];
        A=[5/9,8/9,5/9];
    case 4
        t=[-0.8611363,-0.3399810,0.3399810,0.8611363];
        A=[0.3478548,0.6521452,0.6521452,0.3478548];
    otherwise
        error('高斯点只能取1,2,3,4');
end
g=0;
g=g+(b-a)/2*sum(A.*feval(fname,(b-a)/2*t+(a+b)/2));
end