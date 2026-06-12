clc;clear;close all;

%fun=@(X,xdata)X(1)*exp(X(2)*xdata)+X(3)*xdata.^2;
fun=@(X,xdata)X(1)*xdata+X(2)*xdata.^2.*exp(X(3)*xdata)+X(4)
% @ 函数句柄
xdata=0:0.1:1;
ydata=[3.1 3.27 3.81 4.5 5.18 6 7.05 8.56 9.69 11.25 13.17];
x0=[0 0 0 0];
X=lsqcurvefit(fX(un,x0,xdata,ydata)

syms t;
f=3.0724*exp(0.6741*t)+7.0534*t^2;
plot(xdata,ydata,'or');
hold on;
ezplot(f);
zoom;

%{
非线性曲线拟合： lsqcursefit()
多元线性拟合：regress（z,[x.^2,y.^2,x.*y,c]）
%}







